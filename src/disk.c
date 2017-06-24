#include "./common.h"

#define CHUNK_SIZE KB(4)

const char* USAGE = "\n\
  Description:\n\
    Writes stuff to the disk.\n\
\n\
  Usage:\n\
    disk -n <number of megabytes>\n\
\n\
";

void
write_to_disk(int count)
{
	FILE* f = fopen("disk-out.txt", "w");
	char file_buffer[CHUNK_SIZE] = { 0 };
	unsigned long writes = count * MB(1) / CHUNK_SIZE;

	while (writes-- > 0) {
		fwrite(file_buffer, CHUNK_SIZE, 1, f);
	}

	fclose(f);
}

int
main(int argc, char** argv)
{
	args_t args = { 0 };

	parse_args(argc, argv, &args);
	write_to_disk(args.n);
	wait_until_signalized();

	return 0;
}
