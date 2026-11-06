/*
 * Chapter 9.4 - Running other programs: system(), popen(), fork() + exec()
 */
#define _POSIX_C_SOURCE 200809L   /* for popen, fork, waitpid */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    /* 1. system(): run a shell command, get its exit status */
    int status = system("echo '[system] hello from the shell'");
    printf("system() returned exit code %d\n", WEXITSTATUS(status));

    /* 2. popen(): read another program's output like a file */
    FILE *p = popen("ls /usr/include | head -5", "r");
    if (p != NULL) {
        char line[256];
        printf("first headers in /usr/include:\n");
        while (fgets(line, sizeof line, p))
            printf("  %s", line);
        printf("popen exit code %d\n", WEXITSTATUS(pclose(p)));
    }

    /* 3. fork() + exec(): full control, no shell in between */
    pid_t pid = fork();                          /* now there are TWO processes */
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {                              /* child */
        char *args[] = { "date", "+[child] it is %H:%M", NULL };
        execvp("date", args);                    /* replaces the child with 'date' */
        perror("execvp");                        /* only reached if exec failed */
        _exit(127);
    }
    int child_status;
    waitpid(pid, &child_status, 0);              /* parent waits for the child */
    printf("parent %d: child %d finished with code %d\n",
           (int)getpid(), (int)pid, WEXITSTATUS(child_status));
    return 0;
}
