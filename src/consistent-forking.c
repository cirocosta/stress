#include "./common.h"
#include <inttypes.h>
#include <sys/wait.h>

/**
 * Description:
 *   Forks some processes.
 *
 * Usage
 *   consistent-forking -n <size of the forking ring buffer>
 *
 * How it works:
 *      -       parent forks a process
 *      -       the parent takes this process pid
 *              and puts it into a list of pids.
 *      -       child just sleeps.
 *      -       after a while the parent kills pids
 *              from the list in a ringbuffer manner.
 */

void
fork_so_much(const int n)
{
	int ndx = 0;
	int fp = 0;
	uint64_t nprocs = 0;

	pid_t* pid_buffer;
	pid_t pid;

	_STRESS_MUST(pid_buffer = malloc(sizeof(pid_t) * n),
	             "couldn't allocate pid_buffer - size=%d",
	             n);

	while (1) {
		printf("pid = %d\n", pid_buffer[fp]);

		if (pid_buffer[fp] != 0) {
			_STRESS_MUST_P(kill(pid_buffer[fp], SIGKILL) == 0,
			               "kill",
			               "failed killing child");
			waitpid(pid_buffer[fp], NULL, 0);
		}

		switch (pid = fork()) {
			case -1:
				_STRESS_MUST_P(0, "fork", "unexpected pid -1.");
				break;

			case 0:
				sleep(36000);
				break;

			default:
				fp = ndx % n;
				nprocs++;
				ndx++;

				pid_buffer[fp] = pid;
				printf("forks - %" PRIu64 "\n", nprocs);
		}
	}
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };

	stress_parse_args(argc, argv, &args);
	_STRESS_MUST(args.n > 0, "'n' argument must be at least > 0");
	fork_so_much(args.n);

	return 0;
}
