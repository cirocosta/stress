#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./common.h"

#define KB(x) ((size_t)(x) << 10)
#define MB(x) ((size_t)(x) << 20)
#define CHUNK_SIZE KB(4)

const char* USAGE = "\n\
  Description:\n\
    Writes stuff to the disk.\n\
\n\
  Usage:\n\
    disk -n <number of megabytes>\n\
\n\
";

typedef struct args_t {
	int n;
} args_t;

void
parse_args(int argc, char** argv, args_t* args)
{
	char* nvalue = NULL;
	int pids = 0;
	int c;

	_must(argc > 1, "%s\n  %s", "Argument -n must be specified.", USAGE);

	opterr = 0;
	while ((c = getopt(argc, argv, "n:")) != -1) {
		switch (c) {
			case 'n':
				nvalue = optarg;
				_must(is_num(nvalue), "Argument (n) must be an "
				                      "int (# of pids to "
				                      "create)");
				break;
			case '?':
			default:
				printf("%s", USAGE);
				abort();
		}
	}

	_must(nvalue, "'n' arguments (number of pids') must be specified.");
	_must(nvalue > 0, "At least 1MB must be written");

	args->n = atoi(nvalue);
}

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

	return 0;
}
