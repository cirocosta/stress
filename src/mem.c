#include "./common.h"

void
allocate_memory(int n)
{
	const unsigned int chunksize = MB(1);
	void* alloc_mem_p;

	printf("%d MB will be allocated\n", n);
	while (n-- > 0) {
		alloc_mem_p = malloc(chunksize);

		_must(alloc_mem_p, "Couldn't allocate.");
		memset(alloc_mem_p, 0, chunksize);

		printf("%d MB remaining.\n", n);
	}
}

int
main(int argc, char** argv)
{
	args_t args = { 0 };

	setbuf(stdout, NULL);
	parse_args(argc, argv, &args);
	allocate_memory(args.n);
	wait_until_signalized();

	return 0;
}
