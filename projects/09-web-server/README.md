# Project 9: Tiny Web Server (tinyweb)

A real, multi-threaded HTTP server in plain C with no libraries beyond POSIX. Open it in your browser and it serves the files in `public/`, answers two small JSON APIs, logs every request, refuses to leak files outside its folder, and shuts down cleanly on Ctrl+C. It uses almost every chapter 9 lesson at once: sockets, threads, a mutex, signals, environment variables and time.

## Files

| File | Job |
| --- | --- |
| `http.h` / `http.c` | The pure part: parse the request line, decode `%20`, pick a MIME type, guard paths. No sockets, so it is easy to test |
| `server.h` / `server.c` | The socket part: listen, accept, one detached thread per client, routes, files, logging, SIGINT |
| `main.c` | Reads the folder and port (argument, then `PORT`, then 8080) |
| `public/` | The website: `index.html`, `style.css`, `app.js` (which calls `/api/time`) |
| `secret.txt` | Sits outside `public/` so the tests can prove it is never served |
| `test_http.c` | 35 unit checks for the parser, decoder, MIME table and path guard |
| `test_server.sh` | Starts the server and checks 11 real HTTP behaviours with `curl` |
| `Makefile` | `make`, `make run`, `make test`, `make clean` |

## Build and run

```sh
make
./tinyweb                 # serves public/ on http://localhost:8080
./tinyweb public 9000     # another port
PORT=3000 ./tinyweb
make test
```

Then open <http://localhost:8080>, or from another terminal:

```sh
curl -i http://localhost:8080/api/time
curl "http://localhost:8080/api/hello?name=Asha"
curl -i --path-as-is http://localhost:8080/../secret.txt     # 403 Forbidden
```

## Routes

| Request | Answer |
| --- | --- |
| `GET /` | `public/index.html` |
| `GET /any/file.ext` | That file from `public/`, with the right `Content-Type`, or 404 |
| `GET /api/time` | `{"utc": "2026-11-06T06:51:35Z", "epoch": 1793947895}` |
| `GET /api/hello?name=X` | `{"message": "Hello, X!"}` |
| Paths with `..` or `\` | 403 Forbidden |
| Any method but GET | 405 Method Not Allowed |
| Garbage | 400 Bad Request |

## Limits (on purpose)

One request per connection (`Connection: close`), GET only, no HTTPS, no keep-alive, no range requests, a thread per client. Fine for learning and a home network, not for the open internet. Use nginx, Caddy or a framework there.
