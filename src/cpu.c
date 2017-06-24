#include "./common.h"

const char* USAGE = "\n\
  Description:\n\
    Runs infinite loop on N processes.\n\
\n\
  Usage:\n\
    cpu -n <number of processes>\n\
\n\
";

void
start_workers(int n)
{
	int pid;

	while (n--) {
		switch (pid = fork()) {
			case -1:
				_must(0, "Fork returned -1.");
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
	args_t args = { 0 };

	parse_args(argc, argv, &args);
	start_workers(args.n);
	wait_until_signalized();

	return 0;
}
