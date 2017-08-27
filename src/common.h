#ifndef STRESS__COMMON_H
#define STRESS__COMMON_H

#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define _STRESS_KB(x) ((size_t)(x) << 10)
#define _STRESS_MB(x) ((size_t)(x) << 20)
#define _STRESS_TRUE 1
#define _STRESS_FALSE 0
#define ARG_VALUE_MAX 256

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

#define _STRESS_MUST_P(condition, pmessage, message, ...)                      \
	do {                                                                   \
		if (!(condition)) {                                            \
			fprintf(stderr, "Error:\n  ");                         \
			fprintf(stderr, "\n  ");                               \
			fprintf(stderr, message, ##__VA_ARGS__);               \
			fprintf(stderr, "\n\n");                               \
			fprintf(stderr, "  File: %s \n", __FILE__);            \
			fprintf(stderr, "  Line: %d \n", __LINE__);            \
			fprintf(stderr, "\n");                                 \
			fprintf(stderr, "System Error:\n  ");                  \
			perror(pmessage);                                      \
			fprintf(stderr, "\n  ");                               \
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

/**
 * stress_args_t represents arguments that are set
 * to be parsed from CLI.
 *
 *      -b      whether to block or not
 *      -n      a number of something
 *      -d      base directory
 *      -f      filename
 *
 * None of them are required by default (that's
 * something to be enforced at a higher level).
 *
 * Note.: type is enforced.
 */
typedef struct args_t {
	int n;
	char b;
	char f[FILENAME_MAX];
	char d[FILENAME_MAX];
} stress_args_t;

void
stress_parse_args(int argc, char** argv, stress_args_t* args)
{
	int c;
	size_t arglen;

	_STRESS_MUST(argc > 1, "%s\n", "Argument -n must be specified.");

	opterr = 0;
	while ((c = getopt(argc, argv, "bd:f:n:")) != -1) {
		arglen = 0;
		if (optarg != NULL) {
			arglen = strlen(optarg);
			_STRESS_MUST(arglen < ARG_VALUE_MAX,
			             "-f option can't take more than "
			             "%d characters",
			             ARG_VALUE_MAX);
		}

		switch (c) {
			case 'n':
				_STRESS_MUST(stress_is_num(optarg),
				             "Argument (n=%s) must be an int",
				             optarg);
				args->n = atoi(optarg);
				break;
			case 'b':
				args->b = 1;
				break;
			case 'f':
				strncpy(args->f, optarg, arglen);
				break;
			case 'd':
				strncpy(args->d, optarg, arglen);
				break;
		}
	}
}

void
stress_handle_sigint(int signum)
{
	printf("Signal (%d) received.\n", signum);
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

/**
 * Tries to write 'n' bytes from a 'vptr' to a
 * file descriptor indicated by 'fd'.
 */
ssize_t
stress_write_n(int fd, const void* vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char* ptr;

	ptr = vptr;
	nleft = n;

	while (nleft > 0) {
		nwritten = write(fd, ptr, nleft);
		if (nwritten < 0) {
			if (errno == EINTR) {
				nwritten = 0;
				break;
			}

			return nwritten;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}

	return n - nleft;
}

/**
 * Tries to read 'n' bytes from a file descriptor
 * 'fd' to a buffer indicated by 'vptr'.
 */
ssize_t
stress_read_n(int fd, void* vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char* ptr;

	ptr = vptr;
	nleft = n;

	while (nleft > 0) {
		nread = read(fd, ptr, nleft);
		if (nread < 0) {
			if (errno == EINTR) {
				nread = 0;
				break;
			}

			return nread;
		}

		nleft -= nread;
		ptr += nread;
	}

	return n - nleft;
}

#endif // ! STRESS_COMMON_H
