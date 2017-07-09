#ifndef STRESS__COMMON_H
#define STRESS__COMMON_H

#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define _STRESS_KB(x) ((size_t)(x) << 10)
#define _STRESS_MB(x) ((size_t)(x) << 20)
#define _STRESS_TRUE 1
#define _STRESS_FALSE 0

#define _STRESS_MUST(condition, message, ...)                                  \
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
stress_is_num(char str[])
{
	int i = 0;
	for (; str[i] != 0; i++) {
		if (!isdigit(str[i]))
			return _STRESS_FALSE;
	}

	return _STRESS_TRUE;
}

typedef struct args_t {
	int n;
} stress_args_t;

void
stress_parse_args(int argc, char** argv, stress_args_t* args)
{
	char* nvalue = NULL;
	int pids = 0;
	int c;

	_STRESS_MUST(argc > 1, "%s\n", "Argument -n must be specified.");

	opterr = 0;
	while ((c = getopt(argc, argv, "n:")) != -1) {
		switch (c) {
			case 'n':
				nvalue = optarg;
				_STRESS_MUST(stress_is_num(nvalue),
				             "Argument (n) must be an int");
				break;
		}
	}

	_STRESS_MUST(nvalue, "argument '-n' must be specified.");
	_STRESS_MUST(nvalue > 0, "At least 1MB must be written");

	args->n = atoi(nvalue);
}

void
stress_handle_sigint(int signum)
{
	printf("SIGINT received.\n");
}

void
stress_wait_until_signalized()
{
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = stress_handle_sigint;
	sigaction(SIGINT, &action, NULL);

	printf("[%d] Waiting for SIGINT\n", getpid());
	sleep(36000);
}

#endif // ! STRESS_COMMON_H
