#include "./common.h"

/**
 * Description:
 *   Forks some processes.
 *
 * Usage
 *   pid -n <number of processes to fork>
 *
 */

void
fork_so_much(int n)
{
	int pid;

	printf("Starting to spawn %d blocking children\n", n);
	while (--n) {
		switch (pid = fork()) {
			case -1:
				_STRESS_MUST(0, "Fork returned -1.");
				break;
			case 0:
				printf("[%d] - blocking\n", n);
				stress_wait_until_signalized();
				return;
		}
	}
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };

	stress_parse_args(argc, argv, &args);
	fork_so_much(args.n);
	stress_wait_until_signalized();

	return 0;
}
