#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./common.h"

#define MEGABYTE 1 << 20

int
main(int argc, char** argv)
{
	void* alloc_mem_p;
	int megabytes_remaining = 50;

	if (argc > 1) {
		_must(is_num(argv[1]),
		      "Argument must be an int (# of MB to allocate)");
		megabytes_remaining = atoi(argv[1]);
	}

	printf("Starting. Will allocated %d MB\n", megabytes_remaining);

	while (megabytes_remaining-- > 0) {
		alloc_mem_p = malloc(MEGABYTE);

		_must(alloc_mem_p, "Couldn't allocate a megabyte.");
		memset(alloc_mem_p, 0, MEGABYTE);

		printf("%d MB remaining.\n", megabytes_remaining);
	}

	printf("Done!\n");
	exit(0);
}
