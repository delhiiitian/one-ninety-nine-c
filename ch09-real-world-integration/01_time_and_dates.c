/*
 * Chapter 9.1 - Time and dates with <time.h>
 */
#define _POSIX_C_SOURCE 200809L   /* for localtime_r, gmtime_r, clock_gettime */
#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t now = time(NULL);                     /* seconds since 1970-01-01 UTC */
    printf("Unix timestamp: %lld\n", (long long)now);

    struct tm local, utc;
    localtime_r(&now, &local);                   /* broken into year, month, day... */
    gmtime_r(&now, &utc);

    char buf[64];
    strftime(buf, sizeof buf, "%A %d %B %Y, %H:%M:%S", &local);
    printf("Local time:     %s\n", buf);
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", &utc);    /* ISO 8601, used in logs and APIs */
    printf("UTC (ISO 8601): %s\n", buf);

    /* date arithmetic: build a date, let mktime normalise it */
    struct tm due = { 0 };
    due.tm_year = 2026 - 1900;                   /* years since 1900 */
    due.tm_mon = 11 - 1;                         /* months 0-11 */
    due.tm_mday = 25 + 30;                       /* "30 days after 25 November" */
    due.tm_hour = 12;
    due.tm_isdst = -1;                           /* let the library decide summer time */
    time_t due_time = mktime(&due);              /* fixes day 55 into the right month */
    strftime(buf, sizeof buf, "%d %b %Y (%a)", &due);
    printf("Invoice of 25 Nov 2026, due in 30 days: %s\n", buf);

    struct tm newyear = { 0 };
    newyear.tm_year = 2027 - 1900;
    newyear.tm_mday = 1;
    newyear.tm_isdst = -1;
    double seconds = difftime(mktime(&newyear), due_time);
    printf("From due date to 1 Jan 2027: %.1f days\n", seconds / 86400);

    /* measuring elapsed time precisely */
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    volatile double x = 0;
    for (int i = 0; i < 1000000; i++)
        x += i * 0.5;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double ms = (t1.tv_sec - t0.tv_sec) * 1e3 + (t1.tv_nsec - t0.tv_nsec) / 1e6;
    printf("A million additions took %.2f ms\n", ms);
    return 0;
}
