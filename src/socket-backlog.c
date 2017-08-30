#include "./common.h"
#include <netinet/in.h>

#define LISTEN_BACKLOG 1024
#define MAXLINE 4096

/**
 * Description:
 *   Dumb echo server that writes back to
 *   the client whatever is sent to it.
 *
 * Usage
 *   socket-buffer-size -n <size>
 *
 */

void
str_echo(int sockfd)
{
	ssize_t rn;
	char buf[MAXLINE];

	while ((rn = read(sockfd, buf, MAXLINE)) > 0) {
		_STRESS_MUST(rn >= 0, "unexpected error reading");
		_STRESS_MUST(stress_write_n(sockfd, buf, rn) > 0,
		             "couldn't write back data sent");
	}
}

void
terminate(int dummy)
{
	(void)(dummy);
	exit(0);
}

int
main(int argc, char** argv)
{
	stress_args_t args = {.b = 0 };

	int listen_fd;
	int conn_fd;
	pid_t child_pid;

	socklen_t client_len;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;

	signal(SIGINT, terminate);
	setbuf(stdout, NULL);

	stress_parse_args(argc, argv, &args);

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&server_addr, sizeof(server_addr), 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(1337);

	bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	listen(listen_fd, LISTEN_BACKLOG);

	for (;;) {
		client_len = sizeof(client_addr);
		conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr,
		                 &client_len);

		if ((child_pid = fork()) == 0) {
			close(listen_fd);
			str_echo(conn_fd);
			exit(0);
		}

		close(conn_fd);
	}

	return 0;
}
