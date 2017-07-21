#define _GNU_SOURCE
#define NTHREADS 50

#include "./common.h"

#if defined(__APPLE__) && defined(__MACH__)
int
main()
{
	printf("ERROR: 'context-switches' is Linux only. Aborting.\n");
	exit(1);
}
#else

int
stick_self_to_core(int cpu)
{
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	return sched_setaffinity(0, sizeof(cpuset), &cpuset);
}

void*
thread_handler(void* arg)
{
	(void)arg;
	int num_cores = sysconf(_SC_NPROCESSORS_ONLN);

	while (1) {
		int r = rand() % num_cores;
		int ndx = 0;

		while (++ndx < num_cores) {
			int n = 1000;
			_STRESS_MUST((stick_self_to_core(r) != -1),
			             "couldn't stick thread to cpu %d", ndx);
			while (n-- > 0) {
				printf("Thread %lu is running on CPU %3d\n",
				       (unsigned long)pthread_self(),
				       sched_getcpu());
			}
		}
	}

	return NULL;
}

int
main()
{
	pthread_t threads[NTHREADS];

	srand(time(NULL));
	for (int i = 0; i < NTHREADS; i++) {
		_STRESS_MUST(
		  (!pthread_create(&threads[i], NULL, &thread_handler, NULL)),
		  "Unexpected error creating thread %d", i);
	}

	for (int i = 0; i < NTHREADS; i++) {
		_STRESS_MUST((!pthread_join(threads[i], NULL)),
		             "Unexpected error waiting for thread %d", i);
	}

	return 0;
}

#endif
