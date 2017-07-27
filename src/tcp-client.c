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
 *   tcp -f <ip>
 *
 */

void
str_echo(int sock_fd)
{
	ssize_t wn;
	ssize_t rn;
	char buf[MAXLINE];
	char chunk[MAXLINE] = { 0 };

	memset(chunk, 65, MAXLINE);
	write(sock_fd, chunk, MAXLINE);

	while ((wn = write(sock_fd, buf, MAXLINE)) > 0) {
		_STRESS_MUST(wn >= 0, "unexpected error writing");
	}

	while ((rn = read(sock_fd, buf, MAXLINE)) > 0) {
		_STRESS_MUST(rn >= 0, "unexpected error reading");
	}
}

int
main(int argc, char** argv)
{
	stress_args_t args = {.b = 0 };

	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);

	int sock_fd;
	struct sockaddr_in server_addr;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, sizeof(server_addr), 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(1337);

	inet_pton(AF_INET, args.f, &server_addr.sin_addr);
	connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	str_echo(sock_fd);

	return 0;
}
