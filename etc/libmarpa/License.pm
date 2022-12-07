# Copyright 2022 Jeffrey Kegler
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

package libmarpa::Internal::License;

use 5.010;
use strict;
use warnings;

use English qw( -no_match_vars );
use Fatal qw(open close read);
use File::Spec;
use Text::Diff ();

my $copyright_line = q{Copyright 2022 Jeffrey Kegler};
( my $copyright_line_in_tex = $copyright_line )
    =~ s/ ^ Copyright \s /Copyright \\copyright\\ /xms;

my $closed_license = "$copyright_line\n" . <<'END_OF_STRING';
This document is not part of the Marpa or Libmarpa source.
Although it may be included with a Marpa distribution that
is under an open source license, this document is
not under that open source license.
Jeffrey Kegler retains full rights.
END_OF_STRING

my $license_body = <<'END_OF_STRING';
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
END_OF_STRING

my $license = "$copyright_line\n$license_body";

# License, redone as Tex input
my $license_in_tex =
    "$copyright_line_in_tex\n" . "\\bigskip\\noindent\n" . "$license_body";
$license_in_tex =~ s/^$/\\smallskip\\noindent/gxms;

my $license_file = $license;

my $copying_dicta = <<'EOS';

Most of Libmarpa is licensed under the MIT License, which is given
below and at http://www.opensource.org/licenses/mit-license.html.
Individual libraries and source files may be under other licenses,
including the GNU's LGPL.  For details, see those individual libraries
and source files.

===============
The MIT License
===============
EOS

my $copying_file = join "\n", $copyright_line, $copying_dicta, $license_body;

my $texi_copyright = <<'END_OF_TEXI_COPYRIGHT';
Copyright @copyright{} 2022 Jeffrey Kegler.
END_OF_TEXI_COPYRIGHT

my $texi_license = <<'END_OF_TEXI_LICENSE';
@quotation
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the ``Software''),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
@end quotation
@end copying
END_OF_TEXI_LICENSE

# For texi's auto-generated HTML
my $html_copyright = <<'END_OF_HTML_COPYRIGHT';
Copyright (C) 2022 Jeffrey Kegler.

END_OF_HTML_COPYRIGHT

my $html_license = $html_copyright .  $license_body;
chomp $html_license;
$html_license .= ' -->';

sub hash_comment {
    my ( $text, $char ) = @_;
    $char //= q{#};
    $text =~ s/^/$char /gxms;
    $text =~ s/ [ ]+ $//gxms;
    return $text;
} ## end sub hash_comment

# Assumes $text ends in \n
sub c_comment {
    my ($text) = @_;
    $text =~ s/^/ * /gxms;
    $text =~ s/ [ ] $//gxms;
    return qq{/*\n$text */\n};
} ## end sub c_comment

my $c_license          = c_comment($license);
my $hash_license    = hash_comment($license);
my $xsh_hash_license    = hash_comment($license, q{ #});
my $tex_closed_license = hash_comment( $closed_license, q{%} );
my $tex_license        = hash_comment( $license, q{%} );
my $indented_license   = $license;
$indented_license =~ s/^/  /gxms;
$indented_license =~ s/[ ]+$//gxms;

my $pod_section = <<'END_OF_STRING';
=head1 Copyright and License

=for libmarpa::Display
ignore: 1

END_OF_STRING

$pod_section .= "$indented_license\n";

# Next line is to fake out display checking logic
# Otherwise it will think the lines to come are part
# of a display

=cut

$pod_section .= <<'END_OF_STRING';
=for libmarpa::Display::End

END_OF_STRING

# Next line is to fake out display checking logic
# Otherwise it will think the lines to come are part
# of a display

=cut

my %GNU_file = (
    map {
    (   'work/stage/' . $_, 1,
        'ac_dist/' . $_,   1,
        )
    } qw(
        aclocal.m4
        compile
        config.guess
        config.sub
        configure
        depcomp
        ltmain.sh
        m4/libtool.m4
        m4/lt~obsolete.m4
        m4/ltoptions.m4
        m4/ltsugar.m4
        m4/ltversion.m4
        Makefile.in
        mdate-sh
        missing
        texinfo.tex
        version.m4
    )
);;

sub ignored {
    my ( $filename, $verbose ) = @_;
    my @problems = ();
    if ($verbose) {
        say {*STDERR} "Checking $filename as ignored file" or die "say failed: $ERRNO";
    }
    return @problems;
} ## end sub trivial

sub trivial {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as trivial file" or die "say failed: $ERRNO";
    }
    my $length   = 1000;
    my @problems = ();
    my $actual_length = -s $filename;
    if (not defined $actual_length) {
        my $problem =
            qq{"Trivial" file does not exit: "$filename"\n};
        return $problem;
    }
    if ( -s $filename > $length ) {
        my $problem =
            qq{"Trivial" file is more than $length characters: "$filename"\n};
        push @problems, $problem;
    }
    return @problems;
} ## end sub trivial

sub check_GNU_copyright {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as GNU copyright file" or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text = slurp_top( $filename, 1000 );
    ${$text} =~ s/^[#]//gxms;
    if ( ${$text}
        !~ / \s copyright \s .* Free \s+ Software \s+ Foundation [\s,] /xmsi )
    {
        my $problem = "GNU copyright missing in $filename\n";
        if ($verbose) {
            $problem .= "$filename starts:\n" . ${$text} . "\n";
        }
        push @problems, $problem;
    } ## end if ( ${$text} !~ ...)
    return @problems;
} ## end sub check_GNU_copyright

sub check_X_copyright {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as X Consortium file" or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text = slurp_top( $filename, 1000 );
    if ( ${$text} !~ / \s copyright \s .* X \s+ Consortium [\s,] /xmsi ) {
        my $problem = "X copyright missing in $filename\n";
        if ($verbose) {
            $problem .= "$filename starts:\n" . ${$text} . "\n";
        }
        push @problems, $problem;
    } ## end if ( ${$text} !~ ...)
    return @problems;
} ## end sub check_X_copyright

sub check_tag {
    my ( $tag, $length ) = @_;
    $length //= 250;
    return sub {
        my ( $filename, $verbose ) = @_;
        my @problems = ();
        my $text = slurp_top( $filename, $length );
        if ( ( index ${$text}, $tag ) < 0 ) {
            my $problem = "tag missing in $filename\n";
            if ($verbose) {
                $problem .= "\nMissing tag:\n$tag\n";
            }
            push @problems, $problem;
        } ## end if ( ( index ${$text}, $tag ) < 0 )
        return @problems;
        }
} ## end sub check_tag

my %files_by_type = (
    'LICENSE'      => \&license_problems_in_license_file,
    'META.json'    =>
      \&gnore,ignored,    # not source, and not clear how to add license at top
    'META.yml' =>
      \&ignored,    # not source, and not clear how to add license at top
    'README'                               => \&trivial,
    'TODO'                                 => \&trivial,
    'etc/dovg.sh'                          => \&trivial,
    'etc/compile_for_debug.sh'             => \&trivial,
    'etc/reserved_check.sh'                => \&trivial,
    'work/shared/do_not_edit.c'            => \&trivial,
    'work/shared/do_not_edit.hash'         => \&trivial,
    'work/public/marpa_codes.c.p10'        => \&trivial,
    'work/public/marpa.h.raw'              => \&license_problems_in_c_file,
    'ac_dist/GIT_LOG.txt'                 => \&ignored,
    'cm_dist/GIT_LOG.txt'                 => \&ignored,
    'work/etc/GIT_LOG.txt'                 => \&ignored,
    'ac_dist/LIB_VERSION'                 => \&trivial,
    'cm_dist/LIB_VERSION'                 => \&trivial,
    'work/etc/LIB_VERSION'                 => \&trivial,
    'ac_dist/README.AIX'                 => \&trivial,
    'cm_dist/README.AIX'                 => \&trivial,
    'work/etc/README.AIX'                  => \&trivial,
    'work/etc/libmarpa.pc.in'              => \&license_problems_in_hash_file,
    'work/public/README'                   => \&trivial,
    'work/public/marpa.h.p10'              => \&trivial,
    'work/public/marpa.h-version'          => \&trivial,
    'work/public/marpa.h.p90'              => \&trivial,
    'work/dev/README'                      => \&trivial,
    'work/dev/api.premenu'                 => \&license_problems_in_texi_file,
    'work/dev/internal.premenu'            => \&license_problems_in_texi_file,
    'cmake/config.h.cmake'               => \&license_problems_in_c_file,
    'cm_dist/config.h.cmake'               => \&license_problems_in_c_file,
    'cmake/modules/inline.c' => \&trivial,
    'cm_dist/modules/inline.c' => \&trivial,
    'work/bin/too_long.pl'                 => \&trivial,
    'work/shared/copyright_page_license.w' => \&copyright_page,
    'work/shared/cwebmac.tex'              =>
      \&ignored,    # originally from Cweb, leave it alone
    'work/shared/COPYING'                       => \&license_problems_in_copying_file,
    'ac_dist/COPYING'                       => \&license_problems_in_copying_file,
    'cm_dist/COPYING'                       => \&license_problems_in_copying_file,
    'work/win32/make.bat'          => \&trivial,
    'work/win32/README'            => \&trivial,
    'etc/my_suppressions'          => \&trivial,
    'work/tavl/README'             => \&trivial,

    # Config files
    '.inputrc' => \&trivial,
    '.travis.yml' => \&trivial,
    'etc/indent.pro' => \&trivial,

    # Temporary working file -- will be deleted
    'es_locators.txt' => \&ignored,

    # Leave obstack licensing as is
    'work/obs/marpa_obs.c'      => \&ignored,
    'work/obs/marpa_obs.h'      => \&ignored,
    'work/obs/orig/marpa_obs.c' => \&ignored,
    'work/obs/orig/marpa_obs.h' => \&ignored,
    'work/obs/orig/obstack.c'   => \&ignored,
    'work/obs/orig/obstack.h'   => \&ignored,
    'ac_dist/marpa_obs.c'          => \&ignored,
    'ac_dist/marpa_obs.h'          => \&ignored,
    'cm_dist/marpa_obs.c'       => \&ignored,
    'cm_dist/marpa_obs.h'       => \&ignored,

    # Leave Pfaff's licensing as is
    'ac_dist/marpa_avl.c'    => \&ignored,
    'ac_dist/marpa_avl.h'    => \&ignored,
    'ac_dist/marpa_tavl.c'   => \&ignored,
    'ac_dist/marpa_tavl.h'   => \&ignored,
    'cm_dist/marpa_avl.c'    => \&ignored,
    'cm_dist/marpa_avl.h'    => \&ignored,
    'cm_dist/marpa_tavl.c'   => \&ignored,
    'cm_dist/marpa_tavl.h'   => \&ignored,
    'work/avl/marpa_avl.c'   => \&ignored,
    'work/avl/marpa_avl.h'   => \&ignored,
    'work/tavl/marpa_tavl.c' => \&ignored,
    'work/tavl/marpa_tavl.h' => \&ignored,
    'work/tavl/README.Pfaff' => \&ignored,
    'work/tavl/tavl-test.c'  => \&ignored,
    'work/tavl/test.c'       => \&ignored,
    'work/tavl/test.h'       => \&ignored,

    # Various Lua utilities -- leave licensing as is
    'cm_dist/marpalua/lib/inspect.lua' => \&ignored,
    'cm_dist/marpalua/lib/strict.lua' => \&ignored,
    'cm_dist/marpalua/lib/Test/Builder.lua' => \&ignored,
    'cm_dist/marpalua/lib/Test/More.lua' => \&ignored,
    'marpalua/lib_orig/inspect.lua' => \&ignored,
    'marpalua/lib_orig/Test/Builder.lua' => \&ignored,
    'marpalua/lib_orig/Test/More.lua' => \&ignored,
    'marpalua/lib/strict.lua' => \&ignored,

    # Leave Russ Allbery's licensing as is
    'work/test/tap/c-tap-harness/.clang-format' => \&ignored,
    'work/test/tap/c-tap-harness/.github/workflows/build.yaml' => \&ignored,
    'work/test/tap/c-tap-harness/.gitignore' => \&ignored,
    'work/test/tap/c-tap-harness/LICENSE' => \&ignored,
    'work/test/tap/c-tap-harness/Makefile.am' => \&ignored,
    'work/test/tap/c-tap-harness/NEWS' => \&ignored,
    'work/test/tap/c-tap-harness/README' => \&ignored,
    'work/test/tap/c-tap-harness/README.md' => \&ignored,
    'work/test/tap/c-tap-harness/TODO' => \&ignored,
    'work/test/tap/c-tap-harness/bootstrap' => \&ignored,
    'work/test/tap/c-tap-harness/ci/README' => \&ignored,
    'work/test/tap/c-tap-harness/ci/install' => \&ignored,
    'work/test/tap/c-tap-harness/ci/test' => \&ignored,
    'work/test/tap/c-tap-harness/configure.ac' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/bail.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/bcalloc_type.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/bmalloc.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/breallocarray.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/diag.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/diag_file_add.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/is_int.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/ok.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/plan.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/skip.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/skip_all.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/test_cleanup_register.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/test_file_path.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/api/test_tmpdir.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/docknot.yaml' => \&ignored,
    'work/test/tap/c-tap-harness/docs/runtests.pod' => \&ignored,
    'work/test/tap/c-tap-harness/docs/writing-tests' => \&ignored,
    'work/test/tap/c-tap-harness/m4/cc-flags.m4' => \&ignored,
    'work/test/tap/c-tap-harness/m4/clang.m4' => \&ignored,
    'work/test/tap/c-tap-harness/tests/TESTS' => \&ignored,
    'work/test/tap/c-tap-harness/tests/data/cppcheck.supp' => \&ignored,
    'work/test/tap/c-tap-harness/tests/data/perl.conf' => \&ignored,
    'work/test/tap/c-tap-harness/tests/docs/pod-spelling-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/docs/pod-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/docs/spdx-license-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/abort-one.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/abort-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/abort.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/abort.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/abort.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/badnum-delay.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/badnum.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/bail-silent.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/bail.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/duplicate.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/empty.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/empty.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/fail-count.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/fail.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/fail.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/fail.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/hup.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/missing.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/nocount.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/order.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/pass.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/pass.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/pass.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/plan-last.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/plan-long.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/plan-middle.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/plan-order.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/plan-twice.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/segv.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/skip-all-case.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/skip-all-late.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/skip-all-quiet.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/skip-all.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/skip.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/skip.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/skip.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/status.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/todo.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/too-many.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/basic/zero.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/env-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/env/env.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/env/env.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/env/env.t.in' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/multiple-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/multiple/output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/bad-option.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/bad-option.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/executable.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/libtool' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/lt-executable.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/no-valgrind.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/simple.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/valgrind' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/valgrind-fail.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/valgrind.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/options/valgrind.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search-t.in' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/build/build-no-ext.tap' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/build/build-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/relative-no-ext' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/relative.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/search.list' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/search.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/source/source-no-ext' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/search/source/source.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/single-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/single/test.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/single/test.t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/verbose-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/verbose/list.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/harness/verbose/multiple.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-bail.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-bail.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-basic.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-basic.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-bstrndup.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-bstrndup.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-diag-file.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-diag-file.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-diag.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-extra-one.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-extra-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-extra.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-extra.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-file.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-lazy.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-lazy.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-missing-one.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-missing-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-missing.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-missing.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-skip-reason.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-skip-reason.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-skip.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-skip.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-success-one.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-success-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-success.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-success.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-sysbail.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/c-tmpdir.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-bail' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-bail.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-basic' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-basic.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-diag' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-diag.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-extra' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-extra-one' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-extra-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-extra.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-file' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-lazy' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-lazy.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-missing' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-missing-one' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-missing-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-missing.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-skip' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-skip.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-strip-colon' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-strip-colon.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-success' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-success-one' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-success-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-success.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/basic/sh-tmpdir' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-bail-lazy.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-bail-lazy.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-bail.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-bail.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-fork.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-fork.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-lazy.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-lazy.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-many-fail.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-many-fail.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-many.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-many.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-one-fail.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-one-fail.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-one-with-data.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-one-with-data.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-one.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/libtap/cleanup/c-one.output' => \&ignored,
    'work/test/tap/c-tap-harness/tests/runtests.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/style/obsolete-strings-t' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/basic.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/basic.h' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/float.c' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/float.h' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/libtap.sh' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/macros.h' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/perl/Test/RRA.pm' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/perl/Test/RRA/Automake.pm' => \&ignored,
    'work/test/tap/c-tap-harness/tests/tap/perl/Test/RRA/Config.pm' => \&ignored,
    'work/test/tap/c-tap-harness/tests/valgrind/logs-t' => \&ignored,

    # Leave GNU license as is
    'work/avl/COPYING.LESSER' => \&ignored,
    'work/obs/COPYING.LESSER' => \&ignored,
    'work/tavl/COPYING.LESSER' => \&ignored,
    'ac_dist/COPYING.LESSER' => \&ignored,
    'cm_dist/COPYING.LESSER' => \&ignored,

    # Small files to describe directory contents
    'etc/ABOUT_ME' => \&trivial,
    'cmake/ABOUT_ME' => \&trivial,
    'notes/ABOUT_ME' => \&trivial,
    'tars/ABOUT_ME' => \&trivial,
    'timestamp/ABOUT_ME' => \&trivial,
    'work/ac/ABOUT_ME' => \&trivial,
    'work/ac_doc/ABOUT_ME' => \&trivial,
    'work/ac_doc1/ABOUT_ME' => \&trivial,
    'work/etc/ABOUT_ME' => \&trivial,
    'work/public/ABOUT_ME' => \&trivial,
    'work/timestamp/ABOUT_ME' => \&trivial,

    # We do not add copyright notices to legalese
    'README' => \&ignored,
    'legal/copyright_assignment_rns' => \&ignored,

    # Short and not  very significant
    'notes/HOWTO' => \&trivial,

    # Lists of tests are short and trivial
    'work/test/tap/TESTS' => \&trivial,
    'ac_dist/tests/TESTS' => \&trivial,
    'cmake/tap_test/TESTS.cm' => \&trivial,
    'cm_dist/tap_test/TESTS.ac' => \&trivial,
    'cm_dist/tap_test/TESTS.cm' => \&trivial,

    # MS .def file -- contents trivial
    'work/win32/marpa.def' => \&ignored,
    'ac_dist/win32/marpa.def' => \&ignored,

    # Generated HTML files
    'ac_dist/api_docs/libmarpa_api.html' => \&license_problems_in_html_file,
    'cm_dist/api_docs/libmarpa_api.html' => \&license_problems_in_html_file,

    # Short, auto-generated sh files
    'ac_dist/libmarpa_version.sh' => \&trivial,
    'cm_dist/libmarpa_version.sh' => \&trivial,

    # Short, auto-generated cmake file
    'cm_dist/version.cmake' => \&trivial,

    # Short, auto-generated m4 file
    'ac_dist/version.m4' => \&trivial,

    # The top-level README files in the distributions
    'work/ac/README'     => \&license_problems_in_text_file,
    'ac_dist/README'     => \&license_problems_in_text_file,
    'cm_dist/README'     => \&license_problems_in_text_file,
);

# Files from Russ Allbery
for my $distlib (
    qw(
    cm_dist/marpalua/src
    cm_dist/tap_test/tests
    ac_dist/tests
    test
    )
  )
{
    for my $file (
        qw(
        libtap.sh
        runtests.c
        tap/ABOUT.txt
        tap/basic.c
        tap/basic.h
        tap/float.c
        tap/float.h
        tap/libtap.sh
        tap/macros.h
        tap/runtests.c
        )
      )
    {
        # Leave Russ Allbery's licensing as is
        $files_by_type{"$distlib/$file"} = \&ignored;
    }
}

# Common files in the Lua distribution
for my $distlib (qw( cm_dist/include cm_dist/marpalua/src marpalua/lua-5.4.4/src )) {
    for my $file ( qw(
        lua.h
        luaconf.h
        loslib.c
        lstate.c
        ltable.c
        lfunc.h
        lprefix.h
        llimits.h
        ldo.c
        lstring.h
        lparser.c
        lmathlib.c
        lapi.c
        ldblib.c
        ldo.h
        lopnames.h
        lctype.c
        lobject.h
        ljumptab.h
        lauxlib.c
        llex.c
        lgc.h
        lcorolib.c
        lmem.c
        ldebug.h
        ltablib.c
        ltable.h
        lobject.c
        lbaselib.c
        lzio.h
        lutf8lib.c
        linit.c
        lopcodes.h
        lgc.c
        lfunc.c
        lua.c
        lzio.c
        ltm.c
        lstate.h
        marpalua.c
        lmem.h
        ltm.h
        lcode.h
        liolib.c
        loadlib.c
        luac.c
        lctype.h
        luaconf.h
        lauxlib.h
        lopcodes.c
        lvm.h
        lcode.c
        ldebug.c
        lualib.h
        lvm.c
        lstring.c
        lparser.h
        lstrlib.c
        lundump.h
        llex.h
        lundump.c
        lapi.h
        ldump.c
        ))
    {
        ## Lua has its own license language
        $files_by_type{"$distlib/$file"} = \&ignored;
    }
}

# Common files in the GNU distributions
for my $distlib (
    qw(work/ac ac_dist )
    )
{
    $files_by_type{"$distlib/AUTHORS"}   = \&trivial;
    $files_by_type{"$distlib/NEWS"}      = \&trivial;
    $files_by_type{"$distlib/ChangeLog"} = \&trivial;

    ## GNU standard -- has their license language
    $files_by_type{"$distlib/INSTALL"} = \&ignored;

    $files_by_type{"$distlib/stamp-h1"}   = \&trivial;
    $files_by_type{"$distlib/stamp-1"}   = \&trivial;
    $files_by_type{"$distlib/stamp-vti"}   = \&trivial;
    $files_by_type{"$distlib/install-sh"} = \&check_X_copyright;
    $files_by_type{"$distlib/config.h.in"} =
        check_tag( 'Generated from configure.ac by autoheader', 250 );
} ## end for my $distlib (...)

sub file_type {
    my ($filename) = @_;
    my $closure = $files_by_type{$filename};
    return $closure if defined $closure;
    my ( $volume, $dirpart, $filepart ) = File::Spec->splitpath($filename);
    my @dirs = grep {length} File::Spec->splitdir($dirpart);

    return \&ignored if $filepart =~ /[.]tar\z/xms;

    # info files are generated -- licensing is in source
    return \&ignored if $filepart =~ /[.]info\z/xms;

    # PDF files are generated -- licensing is in source
    return \&ignored if $filepart =~ /[.] (pdf) \z /xms;

    ## GNU license text, leave it alone
    return \&trivial if $filepart eq '.gitignore';
    return \&trivial if $filepart eq '.gitattributes';
    return \&trivial if $filepart eq '.gdbinit';
    return \&check_GNU_copyright
        if $GNU_file{$filename};
    return \&license_problems_in_perl_file
        if $filepart =~ /[.] (t|pl|pm|PL) \z /xms;
    return \&license_problems_in_perl_file
        if $filepart eq 'typemap';
    return \&license_problems_in_texi_file
        if $filepart eq 'internal.texi';
    return \&license_problems_in_texi_file
        if $filepart eq 'api.texi';
    return \&license_problems_in_pod_file if $filepart =~ /[.]pod \z/xms;
    return \&license_problems_in_c_file
        if $filepart =~ /[.] (xs|c|h) \z /xms;
    return \&license_problems_in_xsh_file
        if $filepart =~ /[.] (xsh) \z /xms;
    return \&license_problems_in_sh_file
        if $filepart =~ /[.] (sh) \z /xms;
    return \&license_problems_in_c_file
        if $filepart =~ /[.] (xs|c|h) [.] in \z /xms;
    return \&license_problems_in_tex_file
        if $filepart =~ /[.] (w) \z /xms;
    return \&license_problems_in_hash_file

} ## end sub file_type

sub libmarpa::License::file_license_problems {
    my ( $filename, $verbose ) = @_;
    $verbose //= 0;
    if ($verbose) {
        say {*STDERR} "Checking license of $filename" or die "say failed: $ERRNO";
    }
    my @problems = ();
    return @problems if @problems;
    my $closure = file_type($filename);
    if ( defined $closure ) {
        push @problems, $closure->( $filename, $verbose );
        return @problems;
    }

    # type eq "text"
    push @problems, license_problems_in_text_file( $filename, $verbose );
    return @problems;
} ## end sub libmarpa::License::file_license_problems

sub libmarpa::License::license_problems {
    my ( $files, $verbose ) = @_;
    return
        map { libmarpa::License::file_license_problems( $_, $verbose ) }
        @{$files};
} ## end sub libmarpa::License::license_problems

sub slurp {
    my ($filename) = @_;
    local $RS = undef;
    open my $fh, q{<}, $filename;
    my $text = <$fh>;
    close $fh;
    return \$text;
} ## end sub slurp

sub slurp_top {
    my ( $filename, $length ) = @_;
    $length //= 1000 + ( length $license );
    local $RS = undef;
    open my $fh, q{<}, $filename;
    my $text;
    read $fh, $text, $length;
    close $fh;
    return \$text;
} ## end sub slurp_top

sub files_equal {
    my ( $filename1, $filename2 ) = @_;
    return ${ slurp($filename1) } eq ${ slurp($filename2) };
}

sub tops_equal {
    my ( $filename1, $filename2, $length ) = @_;
    return ${ slurp_top( $filename1, $length ) } eq
        ${ slurp_top( $filename2, $length ) };
}

sub license_problems_in_copying_file {
    my ( $filename, $verbose ) = @_;
    return match_file($filename, $verbose, $copying_file);
}

sub license_problems_in_license_file {
    my ( $filename, $verbose ) = @_;
    return match_file($filename, $verbose, $license_file);
}

sub match_file {
    my ( $filename, $verbose, $file_text ) = @_;
    my @problems = ();
    my $text     = ${ slurp($filename) };
    if ( $text ne $file_text ) {
        my $problem = "$filename is wrong\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( \$text, \$file_text )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( $text ne $file_text )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== $filename should be as follows:\n"
            . $file_text
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_license_file

sub license_problems_in_hash_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as hash style file" or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text = slurp_top( $filename, length $hash_license );
    if ( $hash_license ne ${$text} ) {
        my $problem = "No license language in $filename (hash style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$hash_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( $hash_license ne ${$text} )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $hash_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_hash_file

sub license_problems_in_xsh_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as hash style file"
            or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text = slurp_top( $filename, length $xsh_hash_license );
    if ( $xsh_hash_license ne ${$text} ) {
        my $problem = "No license language in $filename (hash style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$xsh_hash_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( $xsh_hash_license ne ${$text} )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $xsh_hash_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_xsh_file

sub license_problems_in_sh_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as sh hash style file"
            or die "say failed: $ERRNO";
    }
    my @problems = ();
    $DB::single = 1;
    my $ref_text = slurp_top( $filename, 256 + length $hash_license );
    my $text = ${$ref_text};
    $text =~ s/ \A [#][!] [^\n]* \n//xms;
    $text = substr $text, 0, length $hash_license;
    if ( $hash_license ne $text ) {
        my $problem = "No license language in $filename (sh hash style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( \$text, \$hash_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    }
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $hash_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
}

sub license_problems_in_perl_file {
    my ( $filename, $verbose ) = @_;
    $verbose //= 0;
    my @problems = ();
    my $text = slurp_top( $filename, 132 + length $hash_license );

    # Delete hash bang line, if present
    ${$text} =~ s/\A [#][!] [^\n] \n//xms;
    if ( 0 > index ${$text}, $hash_license ) {
        my $problem = "No license language in $filename (perl style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$hash_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( 0 > index ${$text}, $hash_license )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $hash_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_perl_file

sub license_problems_in_c_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as C file" or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text = slurp_top( $filename, 500 + length $c_license );
    ${$text}
        =~ s{ \A [/][*] \s+ DO \s+ NOT \s+ EDIT \s+ DIRECTLY [^\n]* \n }{}xms;
    if ( ( index ${$text}, $c_license ) < 0 ) {
        my $problem = "No license language in $filename (C style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$c_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( ( index ${$text}, $c_license ) < 0 )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $c_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_c_file

sub license_problems_in_tex_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as TeX file" or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text = slurp_top( $filename, 200 + length $tex_license );
    ${$text}
        =~ s{ \A [%] \s+ DO \s+ NOT \s+ EDIT \s+ DIRECTLY [^\n]* \n }{}xms;
    if ( ( index ${$text}, $tex_license ) < 0 ) {
        my $problem = "No license language in $filename (TeX style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$tex_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( ( index ${$text}, $tex_license ) < 0 )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $tex_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_tex_file

# This was the license for the lyx documents
# For the Latex versions, I switched to CC-A_ND
sub tex_closed {
    my ( $filename, $verbose ) = @_;
    my @problems = ();
    my $text = slurp_top( $filename, 400 + length $tex_closed_license );

    if ( ( index ${$text}, $tex_closed_license ) < 0 ) {
        my $problem = "No license language in $filename (TeX style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$tex_closed_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( ( index ${$text}, $tex_closed_license ) < 0 )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $tex_closed_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub tex_closed

sub copyright_page {
    my ( $filename, $verbose ) = @_;

    my @problems = ();
    my $text     = ${ slurp($filename) };
    if ( $text =~ m/ ^ Copyright \s [^J]* \s Jeffrey \s Kegler $ /xmsp ) {
        ## no critic (Variables::ProhibitPunctuationVars);
        my $pos = length ${^PREMATCH};
        $text = substr $text, $pos;
    }
    else {
        push @problems,
            "No copyright and license language in copyright page file: $filename\n";
    }
    if ( not scalar @problems and ( index $text, $license_in_tex ) < 0 ) {
        my $problem = "No copyright/license in $filename\n";
        if ($verbose) {
            $problem .= "Missing copyright/license:\n"
                . Text::Diff::diff( \$text, \$license_in_tex );
        }
        push @problems, $problem;
    } ## end if ( not scalar @problems and ( index $text, $license_in_tex...))
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== copyright/license in $filename should be as follows:\n"
            . $license_in_tex
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub copyright_page

sub license_problems_in_pod_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as POD file" or die "say failed: $ERRNO";
    }

    # Pod files are Perl files, and should also have the
    # license statement at the start of the file
    my @problems = license_problems_in_perl_file( $filename, $verbose );

    my $text = ${ slurp($filename) };
    if ( $text =~ m/ ^ [=]head1 \s+ Copyright \s+ and \s+ License /xmsp ) {
        ## no critic (Variables::ProhibitPunctuationVars);
        my $pos = length ${^PREMATCH};
        $text = substr $text, $pos;
    }
    else {
        push @problems,
            qq{No "Copyright and License" header in pod file $filename\n};
    }
    if ( not scalar @problems and ( index $text, $pod_section ) < 0 ) {
        my $problem = "No LICENSE pod section in $filename\n";
        if ($verbose) {
            $problem .= "Missing pod section:\n"
                . Text::Diff::diff( \$text, \$pod_section );
        }
        push @problems, $problem;
    } ## end if ( not scalar @problems and ( index $text, $pod_section...))
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
            "=== licensing pod section for $filename should be as follows:\n"
            . $pod_section
            . ( q{=} x 30 )
            . "\n"
            ;
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_pod_file

# For texinfo's auoto-generated HTML files
sub license_problems_in_html_file {
    my ( $filename, $verbose ) = @_;
    my @problems = ();
    my $text     = slurp_top($filename, (length $html_license)*2);
    if ( ( index ${$text}, $html_license ) < 0 ) {
        my $problem = "Full language missing in text file $filename\n";
        if ($verbose) {
            $problem .= "\nMissing license language:\n"
                . Text::Diff::diff( $text, \$html_license );
        }
        push @problems, $problem;
    }
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
            "=== licensing pod section for $filename should be as follows:\n"
            . $pod_section
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
}

# In "Text" files, just look for the full language.
# No need to comment it out.
sub license_problems_in_text_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as text file" or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text     = slurp_top($filename, (length $license)*2);
    if ( ( index ${$text}, $license ) < 0 ) {
        my $problem = "Full language missing in text file $filename\n";
        if ($verbose) {
            $problem .= "\nMissing license language:\n"
                . Text::Diff::diff( $text, \$license );
        }
        push @problems, $problem;
    } ## end if ( ( index ${$text}, $license ) < 0 )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
            "=== licensing pod section for $filename should be as follows:\n"
            . $pod_section
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_text_file

sub license_problems_in_texi_file {
    my ( $filename, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as texinfo file"
            or die "say failed: $ERRNO";
    }
    my @problems = ();
    my $text     = slurp_top($filename);
    if ( ( index ${$text}, $texi_copyright ) < 0 ) {
        my $problem = "Copyright missing in texinfo file $filename\n";
        if ($verbose) {
            $problem .= "\nMissing texinfo license language:\n"
                . Text::Diff::diff( $text, \$texi_license );
        }
        push @problems, $problem;
    } ## end if ( ( index ${$text}, $texi_copyright ) < 0 )
    if ( ( index ${$text}, $texi_license ) < 0 ) {
        my $problem = "texinfo language missing in text file $filename\n";
        if ($verbose) {
            $problem .= "\nMissing texinfo license language:\n"
                . Text::Diff::diff( $text, \$texi_license );
        }
        push @problems, $problem;
    } ## end if ( ( index ${$text}, $texi_license ) < 0 )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
            "=== texinfo licensing section for $filename should be as follows:\n"
            . $pod_section
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
} ## end sub license_problems_in_texi_file

1;

# vim: expandtab shiftwidth=4:
