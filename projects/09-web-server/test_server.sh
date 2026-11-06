#!/bin/sh
# Starts tinyweb on a spare port, checks real HTTP responses with curl,
# then stops it with SIGINT and checks it shut down cleanly.
set -e
PORT=${PORT:-18089}
./tinyweb public "$PORT" > server.log 2>&1 &
PID=$!
trap 'kill $PID 2>/dev/null || true' EXIT

i=0
until curl -s -o /dev/null "http://127.0.0.1:$PORT/"; do
    i=$((i + 1)); [ $i -lt 50 ] || { echo "server did not start"; exit 1; }
    sleep 0.1
done

code() { curl -s -o /dev/null -w '%{http_code}' --path-as-is "http://127.0.0.1:$PORT$1"; }
pass=0
expect() {  # expect <what> <got> <wanted>
    if [ "$2" = "$3" ]; then pass=$((pass + 1)); echo "ok   $1"
    else echo "FAIL $1: got '$2', wanted '$3'"; exit 1; fi
}

expect "GET / is 200"            "$(code /)"                 200
expect "index has heading"       "$(curl -s http://127.0.0.1:$PORT/ | grep -c 'Hello from C')" 1
expect "css MIME type"           "$(curl -s -o /dev/null -w '%{content_type}' http://127.0.0.1:$PORT/style.css)" "text/css; charset=utf-8"
expect "missing file is 404"     "$(code /missing.html)"     404
expect "traversal is 403"        "$(code /../secret.txt)"    403
expect "encoded traversal 403"   "$(code /%2e%2e/secret.txt)" 403
expect "POST is 405"             "$(curl -s -o /dev/null -w '%{http_code}' -X POST http://127.0.0.1:$PORT/)" 405
expect "hello API"               "$(curl -s "http://127.0.0.1:$PORT/api/hello?name=Asha")" '{"message": "Hello, Asha!"}'
expect "time API is JSON"        "$(curl -s http://127.0.0.1:$PORT/api/time | grep -c '"utc": "20')" 1

# 20 requests at once: one thread each.
before=$(grep -c 'GET / -> 200' server.log)
for n in $(seq 1 20); do curl -s -o /dev/null "http://127.0.0.1:$PORT/" & done
for job in $(jobs -p); do [ "$job" = "$PID" ] || wait "$job"; done
sleep 0.2
after=$(grep -c 'GET / -> 200' server.log)
expect "20 parallel requests"    "$((after - before))" 20

kill -INT $PID
wait $PID
expect "clean exit on Ctrl+C"    "$(tail -1 server.log | cut -d' ' -f1-2)" "shutting down"
echo "$pass HTTP checks passed"
