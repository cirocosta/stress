#include "./common.h"
#include <sys/file.h>

/**
 * Description:
 *      Creates N files and keeps a lock on each of them
 *      using `flock(2)`
 *
 * Usage:
 *      locks -n <number of files to take a lock to>
 *
 * How it works:
 *      1.      allocates N chunks of 1MB, keeping pointers
 *              to the locations of these chunks in memory.
 *      2.      contiguously goes over all the chunks and
 *              performs a read followed by a write.
 */

static const char files_directory[] = "./stress-files";

void
create_file(const char* base, const int i)
{
	FILE* fp;
	char* filename;

	_STRESS_MUST(asprintf(&filename, "%s/file%d", base, i) > 0,
	             "Couldn't create name for file %d",
	             i);
	_STRESS_MUST(
	  (fp = fopen(filename, "a")), "Couldn't create file %s", filename);

	_STRESS_MUST_P(!flock(fileno(fp), LOCK_EX),
	               "flock",
	               "Couldn't acquire lock on %s",
	               filename);

	fprintf(fp, "dummy");
	free(filename);
}

void
create_files(int n)
{
	struct stat st = { 0 };
	int i = 0;

	printf("creating directory for files at '%s'\n", files_directory);
	_STRESS_MUST(stat(files_directory, &st) == -1,
	             "Directory %s already exists. Please delete it and rerun.",
	             files_directory);
	_STRESS_MUST(mkdir(files_directory, 0700) != -1,
	             "Couldn't create %s directory",
	             files_directory);

	printf("%d files will be created\n", n);
	for (; i++ < n;) {
		create_file(files_directory, i);
	}

	printf("\nDone.\n");
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };

	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);

	create_files(args.n);
	stress_wait_until_signalized();

	return 0;
}
