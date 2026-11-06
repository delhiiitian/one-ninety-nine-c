/* server.h - the socket side: listen, accept, one thread per client. */
#ifndef SERVER_H
#define SERVER_H

/* Serves files from root on port until SIGINT or SIGTERM.
 * Returns 0 after a clean shutdown, 1 if it could not start. */
int run_server(const char *root, int port);

#endif
