#include "./common.h"

#define CHUNK_SIZE _STRESS_KB(4)

/**
 * Description:
 *   Writes stuff to the disk.
 *
 * Usage
 *   disk -n <number of megabytes>
 *
 * Arguments:
 *   -n         number of megabytes
 *   -b         block after running
 */

void
write_to_disk(char* filename, int count)
{
	FILE* f = fopen(filename, "w");
	char file_buffer[CHUNK_SIZE] = { 0 };
	unsigned long writes = count * _STRESS_MB(1) / CHUNK_SIZE;

	while (writes-- > 0) {
		fwrite(file_buffer, CHUNK_SIZE, 1, f);
	}

	fclose(f);
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };

	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);

	_STRESS_MUST(strlen(args.f), "a filename must be specified");

	write_to_disk(args.f, args.n);

	if (args.b)
		stress_wait_until_signalized();
	return 0;
}
