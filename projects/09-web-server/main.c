/* main.c - tinyweb: a tiny multi-threaded static file server.
 * Usage: ./tinyweb [root-folder] [port]   (defaults: public 8080)
 * The port can also come from the PORT environment variable. */
#include "server.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    const char *root = argc > 1 ? argv[1] : "public";
    const char *env_port = getenv("PORT");
    long port = argc > 2 ? strtol(argv[2], NULL, 10)
              : env_port ? strtol(env_port, NULL, 10) : 8080;
    if (port < 1 || port > 65535) {
        fprintf(stderr, "bad port: %ld\n", port);
        return 1;
    }
    return run_server(root, (int)port);
}
