#ifndef MEM__COMMON_H
#define MEM__COMMON_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _must(condition, message, ...)                                         \
	do {                                                                   \
		if (!(condition)) {                                            \
			fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n",          \
			        __LINE__);                                     \
			fprintf(stderr, message, ##__VA_ARGS__);               \
			fprintf(stderr, "%s\n", strerror(errno));              \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

#endif // ! MEM_COMMON_H
