/*
 * Chapter 9.5 - Signals: notifications from the operating system
 * Run it and press Ctrl+C during the countdown: it stops cleanly instead of dying.
 */
#define _POSIX_C_SOURCE 200809L   /* for sigaction, nanosleep */
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t stop_requested = 0;   /* the only safe kind of shared flag */
static volatile sig_atomic_t alarms = 0;

static void on_interrupt(int sig)
{
    (void)sig;
    stop_requested = 1;                  /* do almost nothing inside a handler */
}

static void on_alarm(int sig)
{
    (void)sig;
    alarms++;
}

int main(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_interrupt;
    sigaction(SIGINT, &sa, NULL);        /* Ctrl+C */
    sigaction(SIGTERM, &sa, NULL);       /* "kill <pid>" */
    sa.sa_handler = on_alarm;
    sigaction(SIGALRM, &sa, NULL);

    printf("pid %d: working for 3 seconds (Ctrl+C to stop early)\n", (int)getpid());
    fflush(stdout);
    for (int i = 3; i > 0 && !stop_requested; i--) {
        printf("  %d...\n", i);
        fflush(stdout);
        struct timespec half = { 0, 300000000L };
        nanosleep(&half, NULL);          /* a signal can cut a sleep short */
    }

    raise(SIGALRM);                      /* send a signal to ourselves */
    printf("alarm handler ran %d time(s)\n", (int)alarms);

    raise(SIGTERM);                      /* behave as if someone ran "kill" */
    if (stop_requested)
        printf("stop requested: saving work and exiting cleanly\n");
    return 0;
}
