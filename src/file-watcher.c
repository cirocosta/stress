#include "./common.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/types.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

/**
 * Description:
 *   Listens for notifications across a large number of files.
 *
 * Usage
 *   file-watcher -n <number of files to create and watch>
 *
 */

static const char files_directory[] = "./stress-files";

void
create_file_and_watch(const char* base, const int i, int fd)
{
	FILE* fp;
	char* filename;
        int wd;

	_STRESS_MUST(asprintf(&filename, "%s/file%d", base, i) > 0,
	             "Couldn't create name for file %d", i);
	_STRESS_MUST((fp = fopen(filename, "a")), "Couldn't create file %s",
	             filename);
	fprintf(fp, "dummy");
	fclose(fp);

	wd = inotify_add_watch(fd, files_directory, IN_CREATE);
	free(filename);
}

void
create_files_and_watch(int n, int fd)
{
	struct stat st = { 0 };
	int i = 0;

	_STRESS_INFO("creating directory for files at '%s'", files_directory);

	_STRESS_MUST_P(stat(files_directory, &st) == -1, "stat",
	               "Directory %s already exists.", files_directory);

	_STRESS_MUST_P(mkdir(files_directory, 0700) != -1, "mkdir",
	               "Couldn't create %s directory", files_directory);

	_STRESS_INFO("%d files will be created", n);

	while (i++ < n) {
		create_file_and_watch(files_directory, i, fd);
	}

	_STRESS_INFO("Finished creating files and their watches");
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };
        int fd;

	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);

	_STRESS_MUST_P((fd = inotify_init()) >= 0, "inotify_init",
	               "couldn't initialize inotify");

	create_files_and_watch(args.n, fd);
	stress_wait_until_signalized();
}
