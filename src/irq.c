#include "./common.h"

#define _UNUSED(x) x __attribute__((__unused__))


/**
 * Description:
 *   Forces large number of interrupts via a high-precision
 *   timer that delivers signals to the process all the
 *   time.
 *
 *
 * Usage
 *   irq
 *
 */

void
interrupt_so_much()
{
        struct timespec timer;

        while (1) {
	        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer);
        }
}

static void
handler(int sig, siginfo_t* _UNUSED(xi), void* _UNUSED(uc))
{
        signal(sig, SIG_IGN);
}

int
main()
{
        timer_t timer;

        struct sigevent sev;
        struct sigaction sa;
        sigset_t signal_mask;

        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = handler;



        interrupt_so_much();

	return 0;
}
