#include "./common.h"

int
main(int argc, char** argv)
{
	stress_args_t args = { 0 };
	stress_parse_args(argc, argv, &args);

	return 0;
}
