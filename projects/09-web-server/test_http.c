/* test_http.c - unit tests for the parser, MIME table and path guard. */
#include "http.h"

#include <stdio.h>
#include <string.h>

static int checks = 0, failures = 0;

#define CHECK(cond) do {                                          \
        checks++;                                                 \
        if (!(cond)) {                                            \
            failures++;                                           \
            printf("FAIL line %d: %s\n", __LINE__, #cond);        \
        }                                                         \
    } while (0)

static void test_parse(void)
{
    Request r;
    CHECK(parse_request("GET /index.html HTTP/1.1\r\nHost: x\r\n\r\n", &r) == 0);
    CHECK(strcmp(r.method, "GET") == 0);
    CHECK(strcmp(r.path, "/index.html") == 0);
    CHECK(r.query[0] == '\0');

    CHECK(parse_request("GET /api/hello?name=Asha HTTP/1.0\r\n\r\n", &r) == 0);
    CHECK(strcmp(r.path, "/api/hello") == 0);
    CHECK(strcmp(r.query, "name=Asha") == 0);

    CHECK(parse_request("GET /my%20notes.txt HTTP/1.1\r\n\r\n", &r) == 0);
    CHECK(strcmp(r.path, "/my notes.txt") == 0);

    CHECK(parse_request("", &r) == -1);
    CHECK(parse_request("GET\r\n", &r) == -1);
    CHECK(parse_request("GET index.html HTTP/1.1\r\n", &r) == -1);
    CHECK(parse_request("GET / SPDY/3\r\n", &r) == -1);
    CHECK(parse_request("GET /a%2 HTTP/1.1\r\n", &r) == -1);
    CHECK(parse_request("GET /a%00.html HTTP/1.1\r\n", &r) == -1);
}

static void test_decode(void)
{
    char a[] = "hello+world%21";
    CHECK(url_decode(a) == 0 && strcmp(a, "hello world!") == 0);
    char b[] = "%E2%82%B9";                       /* the rupee sign in UTF-8 */
    CHECK(url_decode(b) == 0 && strcmp(b, "\xE2\x82\xB9") == 0);
    char c[] = "bad%zz";
    CHECK(url_decode(c) == -1);
}

static void test_mime(void)
{
    CHECK(strcmp(mime_type("/index.html"), "text/html; charset=utf-8") == 0);
    CHECK(strcmp(mime_type("/STYLE.CSS"), "text/css; charset=utf-8") == 0);
    CHECK(strcmp(mime_type("/data.json"), "application/json") == 0);
    CHECK(strcmp(mime_type("/archive.tar.gz"), "application/octet-stream") == 0);
    CHECK(strcmp(mime_type("/README"), "application/octet-stream") == 0);
}

static void test_safe_join(void)
{
    char out[64];
    CHECK(safe_join("public", "/", out, sizeof out) == 0);
    CHECK(strcmp(out, "public/index.html") == 0);
    CHECK(safe_join("public", "/css/site.css", out, sizeof out) == 0);
    CHECK(strcmp(out, "public/css/site.css") == 0);

    CHECK(safe_join("public", "/../secret.txt", out, sizeof out) == -1);
    CHECK(safe_join("public", "/a/../../etc/passwd", out, sizeof out) == -1);
    CHECK(safe_join("public", "/..\\win.ini", out, sizeof out) == -1);
    CHECK(safe_join("public", "relative", out, sizeof out) == -1);

    char tiny[8];
    CHECK(safe_join("public", "/index.html", tiny, sizeof tiny) == -1);
}

static void test_status(void)
{
    CHECK(strcmp(status_text(200), "OK") == 0);
    CHECK(strcmp(status_text(404), "Not Found") == 0);
    CHECK(strcmp(status_text(999), "Internal Server Error") == 0);
}

int main(void)
{
    test_parse();
    test_decode();
    test_mime();
    test_safe_join();
    test_status();
    printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
