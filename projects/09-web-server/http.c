/* http.c - request parsing, MIME types and path safety. */
#define _POSIX_C_SOURCE 200809L
#include "http.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

static int hex_value(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    c = (char)tolower((unsigned char)c);
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

int url_decode(char *s)
{
    char *out = s;
    for (; *s; s++) {
        if (*s == '+') {
            *out++ = ' ';
        } else if (*s == '%') {
            int hi = hex_value(s[1]);
            int lo = hi < 0 ? -1 : hex_value(s[2]);
            if (lo < 0) return -1;
            if (hi == 0 && lo == 0) return -1;   /* %00 would hide the rest */
            *out++ = (char)(hi * 16 + lo);
            s += 2;
        } else {
            *out++ = *s;
        }
    }
    *out = '\0';
    return 0;
}

int parse_request(const char *raw, Request *req)
{
    char target[MAX_PATH * 2];
    char version[16];
    memset(req, 0, sizeof *req);

    /* Field widths stop sscanf from overflowing our buffers. */
    if (sscanf(raw, "%7s %511s %15s", req->method, target, version) != 3)
        return -1;
    if (strncmp(version, "HTTP/1.", 7) != 0 || target[0] != '/')
        return -1;

    char *q = strchr(target, '?');
    if (q) {
        *q = '\0';
        snprintf(req->query, sizeof req->query, "%s", q + 1);
    }
    if (strlen(target) >= sizeof req->path) return -1;
    strcpy(req->path, target);
    if (url_decode(req->path) != 0) return -1;
    return 0;
}

const char *mime_type(const char *path)
{
    static const struct { const char *ext, *type; } table[] = {
        { ".html", "text/html; charset=utf-8" },
        { ".css",  "text/css; charset=utf-8" },
        { ".js",   "text/javascript; charset=utf-8" },
        { ".json", "application/json" },
        { ".txt",  "text/plain; charset=utf-8" },
        { ".png",  "image/png" },
        { ".jpg",  "image/jpeg" },
        { ".svg",  "image/svg+xml" },
        { ".bmp",  "image/bmp" },
    };
    const char *dot = strrchr(path, '.');
    if (dot) {
        for (size_t i = 0; i < sizeof table / sizeof table[0]; i++)
            if (strcasecmp(dot, table[i].ext) == 0)
                return table[i].type;
    }
    return "application/octet-stream";
}

int safe_join(const char *root, const char *path, char *out, size_t out_size)
{
    if (path[0] != '/') return -1;
    if (strstr(path, "..") || strchr(path, '\\')) return -1;

    const char *suffix = strcmp(path, "/") == 0 ? "/index.html" : path;
    int n = snprintf(out, out_size, "%s%s", root, suffix);
    if (n < 0 || (size_t)n >= out_size) return -1;
    return 0;
}

const char *status_text(int code)
{
    switch (code) {
    case 200: return "OK";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    default:  return "Internal Server Error";
    }
}
