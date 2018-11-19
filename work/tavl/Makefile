# Copyright 2018 Jeffrey Kegler
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

CFLAGS = -g -W -Wall -ansi -pedantic # enable many GCC warnings
LDFLAGS = -g

all: tavl-test

tavl-test: marpa_tavl.c tavl-test.c test.c marpa_tavl.h test.h 
	$(CC) $(CFLAGS) -DTESTING_TAVL $(LDFLAGS) test.c tavl-test.c marpa_tavl.c -o $@

pfaff_version = 2.0.3

clean:
	rm -f tavl-test
	rm -f *.o *.tmp
	rm -f a.out foo*

.PHONY: all
.PHONY: clean
