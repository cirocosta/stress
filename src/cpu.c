#include "./common.h"

/**
 * Description:
 *   Loads CPUS with a bunch of usage.
 *
 * Usage
 *   cpu -n <number of processes to fork>
 *
 */

void
start_workers(int n)
{
	int pid;

	while (n--) {
		switch (pid = fork()) {
			case -1:
				_STRESS_MUST(0, "Fork returned -1.");
				break;
			case 0:
				printf("[%d] - child started\n", n);
				while (1) {
				}
				break;
		}
	}
}

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };

	setbuf(stdout, NULL);
	stress_parse_args(argc, argv, &args);
	start_workers(args.n);
	stress_wait_until_signalized();

	return 0;
}
