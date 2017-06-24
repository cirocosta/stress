#ifndef MEM__COMMON_H
#define MEM__COMMON_H

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define KB(x) ((size_t)(x) << 10)
#define MB(x) ((size_t)(x) << 20)
#define TRUE 1
#define FALSE 0

#define _must(condition, message, ...)                                         \
	do {                                                                   \
		if (!(condition)) {                                            \
			fprintf(stderr, "Error:\n  ");                         \
			fprintf(stderr, "\n  ");                               \
			fprintf(stderr, message, ##__VA_ARGS__);               \
			fprintf(stderr, "\n\n");                               \
			fprintf(stderr, "  File: %s \n", __FILE__);            \
			fprintf(stderr, "  Line: %d \n", __LINE__);            \
			fprintf(stderr, "\n");                                 \
			fprintf(stderr, "Aborting.\n");                        \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

char
is_num(char str[])
{
	int i = 0;
	for (; str[i] != 0; i++) {
		if (!isdigit(str[i]))
			return FALSE;
	}

	return TRUE;
}

typedef struct args_t {
	int n;
} args_t;

void
parse_args(int argc, char** argv, args_t* args)
{
	char* nvalue = NULL;
	int pids = 0;
	int c;

	_must(argc > 1, "%s\n", "Argument -n must be specified.");

	opterr = 0;
	while ((c = getopt(argc, argv, "n:")) != -1) {
		switch (c) {
			case 'n':
				nvalue = optarg;
				_must(is_num(nvalue), "Argument (n) must be an "
				                      "int (# of pids to "
				                      "create)");
				break;
		}
	}

	_must(nvalue, "'n' arguments (number of pids') must be specified.");
	_must(nvalue > 0, "At least 1MB must be written");

	args->n = atoi(nvalue);
}

void
handle_sigint(int signum)
{
	printf("SIGINT received.\n");
}

void
wait_until_signalized()
{
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = handle_sigint;
	sigaction(SIGINT, &action, NULL);

	printf("[%d] Waiting for SIGINT\n", getpid());
	sleep(36000);
}

#endif // ! MEM_COMMON_H
