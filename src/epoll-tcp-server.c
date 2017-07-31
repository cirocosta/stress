#include "./common.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXEVENTS 64

/**
 * Turns a socket (open file descriptor) into
 * a non-blocking file descriptor by usnig 'fcntl'.
 *
 *
 * 1.   retrieve the flags associated with the
 *      socket fd.
 *
 * 2.   modify the flags such that it includes
 *      O_NONBLOCK.
 *
 *      TODO use _STRESS_MUST_P
 */
static int
make_socket_non_blocking(int sfd)
{
	int flags;
	int s;

	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1) {
		perror("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl(sfd, F_SETFL, flags);
	if (s == -1) {
		perror("fcntl");
		return -1;
	}

	return 0;
}

/**
 * Note:
 * -    Specifying AI_PASSIVE flag in hints.ai_flags
 *      and having node as NULL the returned socket
 *      addresses is suitable for  bind(2)ing  a
 *      socket  that will  accept(2)  connections containing
 *      the INADDR_ANY for IPv4 addresses and IN6ADDR_ANY_INIT
 *      for IPv6 addresses.
 *
 *      TODO use _STRESS_MUST_P
 */
static int
create_and_bind(char* port)
{
	struct addrinfo hints;
	struct addrinfo* result;
	struct addrinfo* rp;
	int s;
	int sfd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;     // IPV4 and IPV6
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;     // All Ifaces

	s = getaddrinfo(NULL, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0) {
			break;
		}

		close(sfd);
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		return -1;
	}

	freeaddrinfo(result);

	return sfd;
}

/**
 * Here we make use of edge-triggered mode which
 * delivers events only when changes occur on the
 * monitored file descriptor. It is particularly nice
 * with a multithreaded server on a multicore machine
 * as we can run one thread per core and have all of
 * them call epoll_wait on the same epfd. When data
 * comes in on an fd, exactly one thread will be woken
 * to handle it.
 */
int
main(int argc, char* argv[])
{
	int sfd, s;
	int efd;
	struct epoll_event event;
	struct epoll_event* events;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sfd = create_and_bind(argv[1]);
	if (sfd == -1)
		abort();

	s = make_socket_non_blocking(sfd);
	if (s == -1)
		abort();

	s = listen(sfd, SOMAXCONN);
	if (s == -1) {
		perror("listen");
		abort();
	}

	efd = epoll_create1(0);
	if (efd == -1) {
		perror("epoll_create");
		abort();
	}

	event.data.fd = sfd;
	event.events = EPOLLIN | EPOLLET; // wait for read; be edge-triggered
	s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
	if (s == -1) {
		perror("epoll_ctl");
		abort();
	}

	// Buffer where events are returned
	events = calloc(MAXEVENTS, sizeof event);

	// TODO use an event loop per thread.
	// The ev loop
	while (1) {
		int n;
		int i;

		n = epoll_wait(efd, events, MAXEVENTS, -1);
		for (i = 0; i < n; i++) {
			if ((events[i].events & EPOLLERR) ||
			    (events[i].events & EPOLLHUP) ||
			    (!(events[i].events & EPOLLIN))) {
				// An error has occured on this fd, or the
				// socket is not ready for reading.
				fprintf(stderr, "epoll error\n");
				close(events[i].data.fd);
				continue;
			}

			// Notification on the listening socket.
			// - one or more incoming connections.
			else if (sfd == events[i].data.fd) {
				while (1) {
					struct sockaddr in_addr;
					socklen_t in_len;
					int infd;
					char hbuf[NI_MAXHOST];
					char sbuf[NI_MAXSERV];

					in_len = sizeof in_addr;
					infd = accept(sfd, &in_addr, &in_len);
					if (infd == -1) {
						if ((errno == EAGAIN) ||
						    (errno == EWOULDBLOCK)) {
							/* We have processed all
							   incoming
							   connections. */
							break;
						} else {
							perror("accept");
							break;
						}
					}

					// TODO remove this.
					// It's unnecesary to get the name
					s = getnameinfo(
					  &in_addr, in_len, hbuf, sizeof hbuf,
					  sbuf, sizeof sbuf,
					  NI_NUMERICHOST | NI_NUMERICSERV);
					if (s == 0) {
						printf("Accepted connection on "
						       "descriptor %d "
						       "(host=%s, port=%s)\n",
						       infd, hbuf, sbuf);
					}

					/* Make the incoming socket non-blocking
					   and add it to the
					   list of fds to monitor. */
					s = make_socket_non_blocking(infd);
					if (s == -1)
						abort();

					event.data.fd = infd;
					event.events = EPOLLIN | EPOLLET;
					s = epoll_ctl(efd, EPOLL_CTL_ADD, infd,
					              &event);
					if (s == -1) {
						perror("epoll_ctl");
						abort();
					}
				}
				continue;
			}

			// data on a fd waiting to be read.
			// - read all of the data that is
			//   available in it (we won't get
			//   another notification for this
			//   same data as we're running
			//   edge-triggered mode).
			else {
				int done = 0;

				while (1) {
					ssize_t count;
					char buf[512];

					// TODO use recv such that we can
					// count but drop the entirity of the
					// content. This allows us to avoid
					// copying from kernel space to
					// userspace.
					count = read(events[i].data.fd, buf,
					             sizeof buf);
					if (count == -1) {
						// We have read all data.
						// Go back to the main loop.
						if (errno != EAGAIN) {
							perror("read");
							done = 1;
						}
						break;
					} else if (count == 0) {
						// EOF
						done = 1;
						break;
					}

					// TODO write back the data that we
					// already have in kernel space.
					s = write(1, buf, count);
					if (s == -1) {
						perror("write");
						abort();
					}
				}

				if (done) {
					printf("Closed connection on "
					       "descriptor %d\n",
					       events[i].data.fd);

					/* Closing the descriptor will make
					   epoll remove it
					   from the set of descriptors which are
					   monitored. */
					close(events[i].data.fd);
				}
			}
		}
	}

	free(events);
	close(sfd);

	return EXIT_SUCCESS;
}
