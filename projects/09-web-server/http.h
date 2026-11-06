/* http.h - the pure, testable part of the tiny web server.
 * Nothing in here touches a socket, so test_http.c can check it all. */
#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

#define MAX_METHOD 8
#define MAX_PATH   256

typedef struct {
    char method[MAX_METHOD];   /* "GET" */
    char path[MAX_PATH];       /* "/index.html" after %-decoding, no query */
    char query[MAX_PATH];      /* "name=asha" or "" */
} Request;

/* Parses "GET /path?q HTTP/1.1\r\n...". Returns 0 on success, -1 if bad. */
int parse_request(const char *raw, Request *req);

/* Decodes %20 and + in place. Returns -1 on a broken escape or %00. */
int url_decode(char *s);

/* "page.html" -> "text/html; charset=utf-8". Unknown -> octet-stream. */
const char *mime_type(const char *path);

/* Joins root and a request path into out, refusing anything that could
 * escape root ("..", backslashes, NUL). "/" becomes "/index.html".
 * Returns 0 if the path is safe, -1 if it must be rejected. */
int safe_join(const char *root, const char *path, char *out, size_t out_size);

/* Human text for a status code: 200 -> "OK". */
const char *status_text(int code);

#endif
