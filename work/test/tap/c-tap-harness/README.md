# C TAP Harness

[![Build
status](https://github.com/rra/c-tap-harness/workflows/build/badge.svg)](https://github.com/rra/c-tap-harness/actions)

Copyright 2000-2001, 2004, 2006-2022 Russ Allbery <eagle@eyrie.org>.
Copyright 2006-2009, 2011-2013 The Board of Trustees of the Leland
Stanford Junior University.  This software is distributed under a
BSD-style license.  Please see the section [License](#license) below for
more information.

## Blurb

C TAP Harness is a pure-C implementation of TAP, the Test Anything
Protocol.  TAP is the text-based protocol used by Perl's test suite.  This
package provides a harness similar to Perl's Test::Harness for running
tests, with some additional features useful for test suites in packages
that use Autoconf and Automake, and C and shell libraries to make writing
TAP-compliant test programs easier.

## Description

This package started as the runtests program I wrote for INN in 2000 to
serve as the basis for a new test suite using a test protocol similar to
that used for Perl modules.  When I started maintaining additional C
packages, I adopted runtests for the test suite driver of those as well,
resulting in further improvements but also separate copies of the same
program in different distributions.  The C TAP Harness distribution merges
all the various versions into a single code base that all my packages can
pull from.

C TAP Harness provides a full TAP specification driver (apart from a few
possible edge cases) and has additional special features for supporting
builds outside the source directory.  It's mostly useful for packages
using Autoconf and Automake and because it doesn't assume or require Perl.

The runtests program can be built with knowledge of the source and build
directory and pass that knowledge on to test scripts, and will search for
test scripts in both the source and build directory.  This makes it easier
for packages using Autoconf and Automake and supporting out-of-tree builds
to build some test programs, ship others, and run them all regardless of
what tree they're in.  It also makes it easier for test cases to find
their supporting files when they run.

Also included in this package are C and shell libraries that provide
utility functions for writing test scripts that use TAP to report results.
The C library also provides a variety of utility functions useful for test
programs running as part of an Automake-built package: finding test data
files, creating temporary files, reporting output from external programs
running in the background, and similar common problems.

## Requirements

C TAP Harness requires a C compiler to build.  Any ISO C89 or later C
compiler on a system supporting the Single UNIX Specification, version 3
(SUSv3) should be sufficient.  This should not be a problem on any modern
system.  The test suite and shell library require a Bourne-compatible
shell.  Outside of the test suite, C TAP Harness has no other
prerequisites or requirements.

C TAP Harness can also be built with a C++ compiler and should be
similarly portable to any recent C++ compiler, although it is only tested
with g++.

To bootstrap from a Git checkout, or if you change the Automake files and
need to regenerate Makefile.in, you will need Automake 1.11 or later.  For
bootstrap or if you change configure.ac or any of the m4 files it includes
and need to regenerate configure or config.h.in, you will need Autoconf
2.64 or later.  Perl is also required to generate manual pages from a
fresh Git checkout.

## Building

You can build C TAP Harness with the standard commands:

```
    ./configure
    make
```

If you are building from a Git clone, first run `./bootstrap` in the
source directory to generate the build files.  Building outside of the
source directory is also supported, if you wish, by creating an empty
directory and then running configure with the correct relative path.

Pass `--enable-silent-rules` to configure for a quieter build (similar to
the Linux kernel).  Use `make warnings` instead of `make` to build with
full GCC compiler warnings (requires either GCC or Clang and may require a
relatively current version of the compiler).

Installing C TAP Harness is not normally done.  Instead, see the section
on using the harness below.

## Testing

C TAP Harness comes with a comprehensive test suite, which you can run
after building with:

```
    make check
```

If a test fails, you can run a single test with verbose output via:

```
    tests/runtests -b $(pwd)/tests -s $(pwd)/tests -o <name-of-test>
```

Do this instead of running the test program directly since it will ensure
that necessary environment variables are set up.  You may need to change
the `-s` option argument if you build with a separate build directory from
the source directory.

To run the test suite, you will need Perl 5.10 or later.  The following
additional Perl modules will be used by the test suite if present:

* Test::Pod
* Test::Spelling

All are available on CPAN.  Those tests will be skipped if the modules are
not available.

To enable tests that don't detect functionality problems but are used to
sanity-check the release, set the environment variable `RELEASE_TESTING`
to a true value.  To enable tests that may be sensitive to the local
environment or that produce a lot of false positives without uncovering
many problems, set the environment variable `AUTHOR_TESTING` to a true
value.

## Using the Harness

While there is an install target that installs runtests in the default
binary directory (`/usr/local/bin` by default) and installs the man pages,
one normally wouldn't install anything from this package.  Instead, the
code is intended to be copied into your package and refreshed from the
latest release of C TAP Harness for each release.

You can obviously copy the code and integrate it however works best for
your package and your build system.  Here's how I do it for my packages as
an example:

* Create a tests directory and copy tests/runtests.c into it.  Create a
  `tests/tap` subdirectory and copy the portions of the TAP library (from
  `tests/tap`) that I need for that package into it.  The TAP library is
  designed to let you drop in additional source and header files for
  additional utility functions that are useful in your package.

* Add code to my top-level `Makefile.am` (I always use a non-recursive
  Makefile with `subdir-objects` set) to build `runtests` and the test
  library:

  ```make
      check_PROGRAMS = tests/runtests
      tests_runtests_CPPFLAGS = \
              -DC_TAP_SOURCE='"$(abs_top_srcdir)/tests"' \
              -DC_TAP_BUILD='"$(abs_top_builddir)/tests"'
      check_LIBRARIES = tests/tap/libtap.a
      tests_tap_libtap_a_CPPFLAGS = -I$(abs_top_srcdir)/tests
      tests_tap_libtap_a_SOURCES = tests/tap/basic.c tests/tap/basic.h \
              tests/tap/float.c tests/tap/float.h tests/tap/macros.h
  ```

  Omit `float.c` and `float.h` from the last line if your package doesn't
  need the `is_double` function.  Building the build and source
  directories into runtests will let `tests/runtests -o <test>` work for
  users without requiring that they set any other variables, even if
  they're doing an out-of-source build.

  Add additional source files and headers that should go into the TAP
  library if you added extra utility functions for your package.

* Add code to `Makefile.am` to run the test suite:

  ```make
      check-local: $(check_PROGRAMS)
            cd tests && ./runtests -l $(abs_top_srcdir)/tests/TESTS
  ```

  See the `Makefile.am` in this package for an example.

* List the test programs in the `tests/TESTS` file.  This should have the
  name of the test executable with the trailing "-t" or ".t" (you can use
  either extension as you prefer) omitted.

  Test programs must be executable.

  For any test programs that need to be compiled, add build rules for them
  in `Makefile.am`, similar to:

  ```make
      tests_libtap_c_basic_LDADD = tests/tap/libtap.a
  ```

  and add them to `check_PROGRAMS`.  If you include the `float.c` add-on
  in your libtap library, you will need to add `-lm` to the `_LDADD`
  setting for all test programs linked against it.

  A more complex example from the remctl package that needs additional
  libraries:

  ```make
      tests_client_open_t_LDFLAGS = $(GSSAPI_LDFLAGS)
      tests_client_open_t_LDADD = client/libremctl.la tests/tap/libtap.a \
              util/libutil.la $(GSSAPI_LIBS)
  ```

  If the test program doesn't need to be compiled, add it to `EXTRA_DIST`
  so that it will be included in the distribution.

* If you have test programs written in shell, copy `tests/tap/libtap.sh`
  the tap subdirectory of your tests directory and add it to `EXTRA_DIST`.
  Shell programs should start with:

  ```sh
      . "${C_TAP_SOURCE}/tap/libtap.sh"
  ```

  and can then use the functions defined in the library.

* Optionally copy `docs/writing-tests` into your package somewhere, such
  as `tests/README`, as instructions to contributors on how to write tests
  for this framework.

If you have configuration files that the user must create to enable some
of the tests, conventionally they go into `tests/config`.

If you have data files that your test cases use, conventionally they go
into `tests/data`.  You can then find the data directory relative to the
`C_TAP_SOURCE` environment variable (set by `runtests`) in your test
program.  If you have data that's compiled or generated by Autoconf, it
will be relative to the `BUILD` environment variable.  Don't forget to add
test data to `EXTRA_DIST` as necessary.

For more TAP library add-ons, generally ones that rely on additional
portability code not shipped in this package or with narrower uses, see
[the rra-c-util
package](https://www.eyrie.org/~eagle/software/rra-c-util/).  There are
several additional TAP library add-ons in the `tests/tap` directory in
that package.  It's also an example of how to use this test harness in
another package.

## Support

The [C TAP Harness web
page](https://www.eyrie.org/~eagle/software/c-tap-harness/) will always
have the current version of this package, the current documentation, and
pointers to any additional resources.

For bug tracking, use the [issue tracker on
GitHub](https://github.com/rra/c-tap-harness/issues).  However, please be
aware that I tend to be extremely busy and work projects often take
priority.  I'll save your report and get to it as soon as I can, but it
may take me a couple of months.

## Source Repository

C TAP Harness is maintained using Git.  You can access the current source
on [GitHub](https://github.com/rra/c-tap-harness) or by cloning the
repository at:

https://git.eyrie.org/git/devel/c-tap-harness.git

or [view the repository on the
web](https://git.eyrie.org/?p=devel/c-tap-harness.git).

The eyrie.org repository is the canonical one, maintained by the author,
but using GitHub is probably more convenient for most purposes.  Pull
requests are gratefully reviewed and normally accepted.

## License

The C TAP Harness package as a whole is covered by the following copyright
statement and license:

> Copyright 2000-2001, 2004, 2006-2022
>     Russ Allbery <eagle@eyrie.org>
>
> Copyright 2006-2009, 2011-2013
>     The Board of Trustees of the Leland Stanford Junior University
>
> Permission is hereby granted, free of charge, to any person obtaining a
> copy of this software and associated documentation files (the "Software"),
> to deal in the Software without restriction, including without limitation
> the rights to use, copy, modify, merge, publish, distribute, sublicense,
> and/or sell copies of the Software, and to permit persons to whom the
> Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in
> all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
> THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
> FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
> DEALINGS IN THE SOFTWARE.

Some files in this distribution are individually released under different
licenses, all of which are compatible with the above general package
license but which may require preservation of additional notices.  All
required notices, and detailed information about the licensing of each
file, are recorded in the LICENSE file.

Files covered by a license with an assigned SPDX License Identifier
include SPDX-License-Identifier tags to enable automated processing of
license information.  See https://spdx.org/licenses/ for more information.

For any copyright range specified by files in this package as YYYY-ZZZZ,
the range specifies every single year in that closed interval.
