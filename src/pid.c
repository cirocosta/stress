#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./common.h"

const char* USAGE = "Usage:\n\
  pid -n <number-of-pids>\n\
";

void
fork_so_much(int n)
{
	sigset_t signalmask;
	int pid;

	printf("Starting to spawn %d blocking children\n", n);
	sigemptyset(&signalmask);
	sigaddset(&signalmask, SIGTERM);
	sigaddset(&signalmask, SIGINT);

	while (n--) {
		switch (pid = fork()) {
			case -1:
				_must(0, "Fork returned -1.");
				break;
			case 0:
				printf("[%d] - blocking\n", n);
				sigsuspend(&signalmask);
				break;
			default:
				printf("[%d] - child creted\n", n);
				break;
		}
	}

	sigsuspend(&signalmask);
	printf("Finished creating children. Now blocking\n");
}

int
main(int argc, char** argv)
{
	char* nvalue = NULL;
	int pids = 0;
	int index;
	int c;

	if (argc < 2) {
		printf("%s", USAGE);
		abort();
	}

	opterr = 0;

	while ((c = getopt(argc, argv, "n:")) != -1) {
		switch (c) {
			case 'n':
				nvalue = optarg;
				_must(is_num(nvalue), "Argument (n) must be an "
				                      "int (# of pids to "
				                      "create)");
				break;
			case '?':
				printf("%s", USAGE);
				return 1;
			default:
				printf("%s", USAGE);
				abort();
		}
	}

	_must(nvalue, "'n' arguments (number of pids') must be specified.");
	fork_so_much(atoi(nvalue));

	return 0;
}
