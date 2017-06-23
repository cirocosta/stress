#ifndef MEM__COMMON_H
#define MEM__COMMON_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define _must(condition, message, ...)                                         \
	do {                                                                   \
		if (!(condition)) {                                            \
			fprintf(stderr, "Error:\n  ");                         \
			fprintf(stderr, message, ##__VA_ARGS__);               \
			fprintf(stderr, "\n\n");                               \
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

#endif // ! MEM_COMMON_H
