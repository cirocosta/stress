#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./common.h"

#define CHUNK_SIZE 4096

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

	_must(argc < 2, "%s", USAGE);

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
}

void
write_to_disk(int count)
{
	FILE* f = fopen("disk-out.txt", "w");
	char file_buffer[CHUNK_SIZE + 64];
	int buffer_count = 0;
	int i = 0;

	while (i < count) {
		buffer_count += sprintf(&file_buffer[buffer_count],
		                        "%d %d %d\n", 1234, 5678, 9012);
		i++;

		if (buffer_count >= CHUNK_SIZE) {
			fwrite(file_buffer, buffer_count, 1, f);
			buffer_count = 0;
		}
	}

	if (buffer_count > 0) {
		fwrite(file_buffer, 1, buffer_count, f);
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
