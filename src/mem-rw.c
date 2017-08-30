#include "./common.h"

#define CHUNK_SIZE _STRESS_MB(1)

/**
 * Description:
 *      Allocates and read-writes into/from memory.
 *
 * Usage:
 *      mem-rw -n <number of megabytes to allocate>
 *
 * How it works:
 *      1.      allocates N chunks of 1MB, keeping pointers
 *              to the locations of these chunks in memory.
 *      2.      contiguously goes over all the chunks and
 *              performs a read followed by a write.
 */

static char __attribute__((always_inline)) randoml26()
{
	long l;
	do {
		l = random();
	} while (l >= (RAND_MAX / 26) * 26);
	return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[(unsigned)(l % 26)];
}

void
allocate_and_use(int n)
{
	void** p;

	_STRESS_MUST((p = malloc((n + 1) * sizeof(void*))),
	             "couldn't allocate memory array");

	_STRESS_INFO("%d MB will be allocated", n);

	for (int i = 0; i++ < n;) {
		_STRESS_MUST((p[i] = malloc(CHUNK_SIZE * sizeof(char))),
		             "Couldn't allocate.");
		memset(p[i], randoml26(), CHUNK_SIZE);
		_STRESS_INFO("Allocated 1MB at position %p", p[i]);
	}

	_STRESS_INFO("Memory allocated.");
	_STRESS_INFO("Starting dumb memory setting.");

	while (1) {
		for (int i = 0; i++ < n;) {
			memset(p[i], randoml26(), CHUNK_SIZE);
		}
	}
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };

	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);
	allocate_and_use(args.n);

	return 0;
}
