#!/bin/sh
ulimit -c unlimited
echo 1..4
echo 'ok   4'
kill -ABRT $$
