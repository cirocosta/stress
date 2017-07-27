#include "./common.h"
#include <arpa/inet.h>
#include <netinet/in.h>

#define LISTEN_BACKLOG 128
#define MAXLINE 4096

/**
 * Description:
 *   Dumb echo server that writes back to
 *   the client whatever is sent to it.
 *
 * Usage
 *   tcp -f <ip> -n <port>
 *
 */

static volatile int run = 1;

void
send_chunks(int sock_fd)
{
	char buf[MAXLINE];
	char chunk[MAXLINE] = { 0 };

	memset(chunk, 65, MAXLINE);

	while (run) {
		_STRESS_MUST(stress_write_n(sock_fd, chunk, MAXLINE) >= 0,
		             "couldn't write contents to socket");
		_STRESS_MUST(stress_read_n(sock_fd, buf, MAXLINE) >= 0,
		             "unexpected error while reading");
	}
}

void
terminate(int dummy)
{
	(void)(dummy);
	run = 0;
}

int
main(int argc, char** argv)
{
	stress_args_t args = {.b = 0 };

	int sock_fd;
	struct sockaddr_in server_addr;

	signal(SIGINT, terminate);
	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);

	_STRESS_MUST(args.n, "a port (n) must be specified");
	_STRESS_MUST(strlen(args.f), "an ip (f) must be specified");

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, sizeof(server_addr), 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(args.n);

	inet_pton(AF_INET, args.f, &server_addr.sin_addr);
	connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	send_chunks(sock_fd);

	return 0;
}
