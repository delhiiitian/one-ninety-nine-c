/*
 * Chapter 9.7 - Sockets: talking over a network (TCP)
 * The program starts a tiny "uppercase" server on 127.0.0.1, then connects to it as a client.
 * Real servers and clients are separate programs; one process here keeps the demo self-contained.
 */
#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int listen_fd;

static void *server(void *arg)
{
    (void)arg;
    int client = accept(listen_fd, NULL, NULL);          /* wait for one caller */
    if (client < 0) { perror("accept"); return NULL; }
    char buf[256];
    ssize_t n = recv(client, buf, sizeof buf, 0);        /* read their message */
    for (ssize_t i = 0; i < n; i++)
        buf[i] = (char)toupper((unsigned char)buf[i]);
    send(client, buf, (size_t)n, 0);                     /* reply */
    close(client);
    return NULL;
}

int main(void)
{
    /* --- server side: socket, bind, listen --- */
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);         /* TCP over IPv4 */
    if (listen_fd < 0) { perror("socket"); return 1; }
    int yes = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);       /* 127.0.0.1 only */
    addr.sin_port = htons(0);                            /* 0 = let the OS pick a free port */
    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof addr) < 0) { perror("bind"); return 1; }
    listen(listen_fd, 8);
    socklen_t len = sizeof addr;
    getsockname(listen_fd, (struct sockaddr *)&addr, &len);
    printf("server listening on 127.0.0.1:%d\n", ntohs(addr.sin_port));

    pthread_t t;
    pthread_create(&t, NULL, server, NULL);

    /* --- client side: socket, connect, send, recv --- */
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(fd, (struct sockaddr *)&addr, sizeof addr) < 0) { perror("connect"); return 1; }
    const char *msg = "hello over tcp";
    send(fd, msg, strlen(msg), 0);
    char reply[256];
    ssize_t n = recv(fd, reply, sizeof reply - 1, 0);
    reply[n > 0 ? n : 0] = '\0';
    printf("client sent \"%s\", server replied \"%s\"\n", msg, reply);
    close(fd);

    pthread_join(t, NULL);
    close(listen_fd);
    return 0;
}
