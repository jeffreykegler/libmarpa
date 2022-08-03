#!/bin/sh
echo 1..10
echo ok 1
echo ok 2
echo some other nonsense ok 3
echo ok 3 >&2
echo ok 3
echo '   ok 4  ok 5'
echo 'ok 4  ok 5'
echo ok 5
echo ok 6
echo ok 7
echo ok'   ' 8
echo '    ok    9'
echo ok 9
echo ok10
