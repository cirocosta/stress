#include "./common.h"

/**
 * Description:
 *   Allocates some memory.
 *
 * Usage
 *   mem -n <number of megabytes to allocate>
 *
 */

void
allocate_memory(int n)
{
	const unsigned int chunksize = _STRESS_MB(1);
	int i = 0;
	void* p;

	printf("%d MB will be allocated\n", n);
	for (; i++ < n;) {
		_STRESS_MUST((p = malloc(chunksize * sizeof(char))),
		             "Couldn't allocate.");
		memset(p, 0xA, chunksize);
		printf("\rAllocated: %d", i);
	}
	printf("\nDone.");
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };

	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);
	allocate_memory(args.n);
	stress_wait_until_signalized();

	return 0;
}
