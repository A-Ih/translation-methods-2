#!/bin/sh

while read line
do
    echo "Testing '$line'..."
    if echo "$line" | ./parser >/dev/null; then
        echo "OK: parser returned 0"
    else
        echo "FAIL: parser failed"
        exit 1
    fi
done <examples

while read line
do
    echo "Testing '$line'..."
    if echo "$line" | ./parser >/dev/null; then
        echo "FAIL: error expected"
        exit 1
    else
        echo "OK: parser failed"
    fi
done <invalid-examples

echo "==============================================================="
echo "==========================SUCCESS=============================="
echo "==============================================================="
