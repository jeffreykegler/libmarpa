#!/bin/sh
echo 1..1
if [ -z "$C_TAP_VALGRIND" ]; then
    echo ok 1
else
    echo not ok 1
fi
