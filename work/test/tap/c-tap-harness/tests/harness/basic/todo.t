#!/bin/sh
echo 1..4
echo not ok 1 '# todo'
echo not ok2'#TODO'
echo ok 3 '# todofoo'
echo ok 4 '    #    TODO foo bar baz'
