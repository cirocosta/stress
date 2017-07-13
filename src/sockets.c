#include "./common.h"
#include <sys/socket.h>
#include <sys/types.h>

int
main()
{
	int sockets[2];

	_STRESS_MUST((!socketpair(AF_LOCAL, SOCK_STREAM, 0, sockets)),
	             "Couldn't create pair of sockets");
	sleep(600);

	return 0;
}
