#include "./common.h"

#define CHUNK_SIZE _STRESS_KB(4)

/**
 * Description:
 *   Writes N megabytes of garbage to stdout and stderr (each).
 *
 * Warnings:
 *   - this is not throttled
 *
 * Usage
 *   print -n <number of megabytes>
 *
 */

typedef struct thread_arg_t {
	int count;
	int fd;
} thread_arg_t;

void*
thread_handler(void* arg)
{
	thread_arg_t* targ = (thread_arg_t*)arg;
	char file_buffer[CHUNK_SIZE] = { 0 };
	unsigned long writes = targ->count * _STRESS_MB(1) / CHUNK_SIZE;

	memset(file_buffer, 65, CHUNK_SIZE);
	while (writes-- > 0) {
		_STRESS_MUST(write(targ->fd, file_buffer, CHUNK_SIZE) != -1,
		             "Couldn't properly write chunk to fd %d",
		             targ->fd);
	}

	return NULL;
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };
	thread_arg_t stdout_arg = {.count = 0, .fd = STDOUT_FILENO };
	thread_arg_t stderr_arg = {.count = 0, .fd = STDERR_FILENO };
	pthread_t threads[2];

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	stress_parse_args(argc, argv, &args);

	stdout_arg.count = args.n;
	stderr_arg.count = args.n;

	_STRESS_MUST(
	  (!pthread_create(&threads[0], NULL, &thread_handler, &stdout_arg)),
	  "Unexpected error creating stdout thread");
	_STRESS_MUST(
	  (!pthread_create(&threads[1], NULL, &thread_handler, &stderr_arg)),
	  "Unexpected error creating stderr thread");

	_STRESS_MUST((!pthread_join(threads[0], NULL)),
	             "Unexpected error waiting for stdout thread");

	_STRESS_MUST((!pthread_join(threads[1], NULL)),
	             "Unexpected error waiting for stderr thread");

	stress_wait_until_signalized();

	return 0;
}
