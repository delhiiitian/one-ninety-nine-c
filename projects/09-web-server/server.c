/* server.c - sockets, threads, signals and logging. */
#define _POSIX_C_SOURCE 200809L
#include "server.h"
#include "http.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t stop_requested = 0;
static const char *doc_root;
static pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
static unsigned long request_count = 0;

static void on_signal(int sig)
{
    (void)sig;
    stop_requested = 1;          /* only flip a flag inside a handler */
}

/* Writes all of buf, even if send() only takes part of it. */
static int send_all(int fd, const char *buf, size_t len)
{
    while (len > 0) {
        ssize_t n = send(fd, buf, len, MSG_NOSIGNAL);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        buf += n;
        len -= (size_t)n;
    }
    return 0;
}

static void send_head(int fd, int code, const char *type, long length)
{
    char head[256];
    int n = snprintf(head, sizeof head,
                     "HTTP/1.1 %d %s\r\n"
                     "Content-Type: %s\r\n"
                     "Content-Length: %ld\r\n"
                     "Connection: close\r\n"
                     "Server: tinyweb\r\n\r\n",
                     code, status_text(code), type, length);
    send_all(fd, head, (size_t)n);
}

static void send_text(int fd, int code, const char *type, const char *body)
{
    send_head(fd, code, type, (long)strlen(body));
    send_all(fd, body, strlen(body));
}

static void send_error(int fd, int code)
{
    char body[256];
    snprintf(body, sizeof body,
             "<!doctype html><title>%d</title><h1>%d %s</h1>\n",
             code, code, status_text(code));
    send_text(fd, code, "text/html; charset=utf-8", body);
}

static int send_file(int fd, const char *file_path)
{
    struct stat st;
    if (stat(file_path, &st) != 0 || !S_ISREG(st.st_mode))
        return 404;
    FILE *f = fopen(file_path, "rb");
    if (!f) return 403;

    send_head(fd, 200, mime_type(file_path), (long)st.st_size);
    char chunk[8192];
    size_t got;
    while ((got = fread(chunk, 1, sizeof chunk, f)) > 0)
        if (send_all(fd, chunk, got) != 0) break;
    fclose(f);
    return 200;
}

static int api_time(int fd)
{
    time_t now = time(NULL);
    struct tm tm;
    char stamp[32], body[128];
    gmtime_r(&now, &tm);
    strftime(stamp, sizeof stamp, "%Y-%m-%dT%H:%M:%SZ", &tm);
    snprintf(body, sizeof body, "{\"utc\": \"%s\", \"epoch\": %ld}\n",
             stamp, (long)now);
    send_text(fd, 200, "application/json", body);
    return 200;
}

static int api_hello(int fd, const char *query)
{
    char name[64] = "world";
    if (strncmp(query, "name=", 5) == 0 && query[5] != '\0') {
        snprintf(name, sizeof name, "%.60s", query + 5);  /* cap length */
        if (url_decode(name) != 0) { send_error(fd, 400); return 400; }
        for (char *p = name; *p; p++)      /* keep the JSON valid */
            if (*p == '"' || *p == '\\' || (unsigned char)*p < 32) *p = '_';
    }
    char body[128];
    snprintf(body, sizeof body, "{\"message\": \"Hello, %s!\"}\n", name);
    send_text(fd, 200, "application/json", body);
    return 200;
}

static void log_request(const char *ip, const Request *req, int code)
{
    char stamp[16];
    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);
    strftime(stamp, sizeof stamp, "%H:%M:%S", &tm);

    pthread_mutex_lock(&log_lock);
    request_count++;
    printf("%s %s %s %s -> %d\n", stamp, ip,
           req->method[0] ? req->method : "-",
           req->path[0] ? req->path : "-", code);
    fflush(stdout);
    pthread_mutex_unlock(&log_lock);
}

typedef struct {
    int fd;
    char ip[INET_ADDRSTRLEN];
} Client;

static int handle(int fd, Request *req)
{
    char raw[4096];
    ssize_t n = recv(fd, raw, sizeof raw - 1, 0);
    if (n <= 0) return 400;
    raw[n] = '\0';

    if (parse_request(raw, req) != 0) { send_error(fd, 400); return 400; }
    if (strcmp(req->method, "GET") != 0) { send_error(fd, 405); return 405; }

    if (strcmp(req->path, "/api/time") == 0)  return api_time(fd);
    if (strcmp(req->path, "/api/hello") == 0) return api_hello(fd, req->query);

    char file_path[512];
    if (safe_join(doc_root, req->path, file_path, sizeof file_path) != 0) {
        send_error(fd, 403);
        return 403;
    }
    int code = send_file(fd, file_path);
    if (code != 200) send_error(fd, code);
    return code;
}

static void *client_thread(void *arg)
{
    Client *c = arg;
    Request req;
    memset(&req, 0, sizeof req);

    struct timeval tv = { .tv_sec = 5, .tv_usec = 0 };  /* slow clients */
    setsockopt(c->fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);

    int code = handle(c->fd, &req);
    log_request(c->ip, &req, code);
    close(c->fd);
    free(c);
    return NULL;
}

int run_server(const char *root, int port)
{
    doc_root = root;

    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;           /* no SA_RESTART: accept() wakes */
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) { perror("socket"); return 1; }
    int yes = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)port);
    if (bind(server, (struct sockaddr *)&addr, sizeof addr) != 0 ||
        listen(server, 64) != 0) {
        perror("bind/listen");
        close(server);
        return 1;
    }
    printf("tinyweb serving %s on http://localhost:%d (Ctrl+C to stop)\n",
           root, port);
    fflush(stdout);

    while (!stop_requested) {
        struct sockaddr_in peer;
        socklen_t len = sizeof peer;
        int fd = accept(server, (struct sockaddr *)&peer, &len);
        if (fd < 0) {
            if (errno == EINTR) continue;   /* a signal: re-check the flag */
            perror("accept");
            continue;
        }
        Client *c = malloc(sizeof *c);
        if (!c) { close(fd); continue; }
        c->fd = fd;
        inet_ntop(AF_INET, &peer.sin_addr, c->ip, sizeof c->ip);

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_thread, c) != 0) {
            close(fd);
            free(c);
            continue;
        }
        pthread_detach(tid);                /* nobody will join it */
    }

    close(server);
    sleep(1);                               /* let running threads finish */
    printf("shutting down after %lu requests\n", request_count);
    return 0;
}
