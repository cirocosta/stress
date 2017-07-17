#include "./common.h"

#define CHUNK_SIZE _STRESS_KB(4)

/**
 * Description:
 *   Writes and rewrites N MB of garbage to 2 files (each).
 *
 * Usage
 *   write -n <number of megabytes>
 *
 */

typedef struct thread_arg_t {
	char* base_dir;
	int thread_num;
	int count;
} thread_arg_t;

void*
thread_handler(void* arg)
{
	thread_arg_t* targ = (thread_arg_t*)arg;
	char file_buffer[CHUNK_SIZE] = { 0 };
	unsigned long writes = targ->count * _STRESS_MB(1) / CHUNK_SIZE;
	int i = writes;
	char* filename;
	FILE* fp;

	_STRESS_MUST(asprintf(&filename, "%s/file%d", targ->base_dir,
	                      targ->thread_num) > 0,
	             "Couldn't create name for file %d", i);

	_STRESS_MUST((fp = fopen(filename, "w+")), "Couldn't create file %s",
	             filename);

	int fd = fileno(fp);

	while (1) {
		memset(file_buffer, rand() % 255, CHUNK_SIZE);
		while (i-- > 0) {
			_STRESS_MUST(
			  write(fd, file_buffer, CHUNK_SIZE) != -1,
			  "Couldn't properly write chunk to file %s (fd=%d)",
			  filename, fd);
		}
		_STRESS_MUST(
		  fsync(fd) != -1,
		  "Couldn't sync filedescriptor with disk (fname=%s)",
		  filename);
		_STRESS_MUST(lseek(fd, 0, SEEK_SET) != -1,
		             "Couldn't seek to init of file %s", filename);
		i = writes;
	}

	return NULL;
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };
	pthread_t threads[2];

	stress_parse_args(argc, argv, &args);
	_STRESS_MUST(strlen(args.d), "a base directory must be specified");

	thread_arg_t thread1arg = {.count = args.n,
		                   .base_dir = args.d,
		                   .thread_num = 0 };
	thread_arg_t thread2arg = {.count = args.n,
		                   .base_dir = args.d,
		                   .thread_num = 1 };

	srand(time(NULL));

	_STRESS_MUST(
	  (!pthread_create(&threads[0], NULL, &thread_handler, &thread1arg)),
	  "Unexpected error creating stdout thread");
	_STRESS_MUST(
	  (!pthread_create(&threads[1], NULL, &thread_handler, &thread2arg)),
	  "Unexpected error creating stderr thread");

	_STRESS_MUST((!pthread_join(threads[0], NULL)),
	             "Unexpected error waiting for stdout thread");

	_STRESS_MUST((!pthread_join(threads[1], NULL)),
	             "Unexpected error waiting for stderr thread");

	return 0;
}
