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

package libmarpa::Internal::License;

use 5.010;
use strict;
use warnings;

use English qw( -no_match_vars );
use Fatal qw(open close read);
use File::Spec;
use Text::Diff ();

my $copyright_line = q{Copyright 2018 Jeffrey Kegler};
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

my $texi_copyright = <<'END_OF_TEXI_COPYRIGHT';
Copyright @copyright{} 2018 Jeffrey Kegler.
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
my $r2_hash_license    = hash_comment($license);
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
        'work/test/dev/' . $_,   1,
        'dist/' . $_,   1,
        'doc_dist/' . $_,   1,
        'doc1_dist/' . $_,   1
        )
    } qw(
        aclocal.m4
        config.guess
        config.sub
        configure
        depcomp
        mdate-sh
        texinfo.tex
        ltmain.sh
        m4/libtool.m4
        m4/ltoptions.m4
        m4/ltsugar.m4
        m4/ltversion.m4
        m4/lt~obsolete.m4
        missing
        Makefile.in
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
    'COPYING.LESSER' => \&ignored,    # GNU license text, leave it alone
    'cm_dist/COPYING.LESSER' => \&ignored,    # GNU license text, leave it alone
    'cm_dist/COPYING' => \&ignored,    # MIT license text, leave it alone
    'dist/compile' => \&ignored,    # GNU file, leave it alone
    'dist/COPYING' => \&ignored,    # MIT license text, leave it alone
    'doc_dist/COPYING' => \&ignored,    # MIT license text, leave it alone
    'doc1_dist/COPYING' => \&ignored,    # MIT license text, leave it alone
    'LICENSE' => \&license_problems_in_license_file,
    'META.json' =>
        \&ignored,    # not source, and not clear how to add license at top
    'META.yml' =>
        \&ignored,    # not source, and not clear how to add license at top
    'README'                            => \&trivial,
    'TODO'                              => \&trivial,
    'etc/dovg.sh'                       => \&trivial,
    'etc/compile_for_debug.sh'          => \&trivial,
    'etc/reserved_check.sh'             => \&trivial,
    'libmarpa/shared/do_not_edit.c'            => \&trivial,
    'libmarpa/public/marpa_codes.c.p10'        => \&trivial,
    'libmarpa/public/marpa.h.p10'              => \&trivial,
    'libmarpa/public/marpa.h-version'          => \&trivial,
    'libmarpa/public/marpa.h.p90'              => \&trivial,
    'libmarpa/dev/README'                      => \&trivial,
    'dist/LIB_VERSION'                => \&trivial,
    'dist/LIB_VERSION.in'             => \&trivial,
    'cm_dist/LIB_VERSION.cmake'             => \&trivial,
    'cm_dist/README'             => \&license_problems_in_text_file,
    'cm_dist/modules/inline.c'             => \&trivial,
    'cm_dist/config.h.cmake'             => \&trivial,
    'libmarpa/public/LIB_VERSION.in'           => \&trivial,
    'libmarpa/bin/too_long.pl'                 => \&trivial,
    'libmarpa/shared/copyright_page_license.w' => \&copyright_page,
    'libmarpa/shared/cwebmac.tex' =>
        \&ignored,    # originally from Cweb, leave it alone
    'libmarpa/test/Makefile'        => \&trivial,
    'libmarpa/test/README'          => \&trivial,
    'libmarpa/test/dev/install-sh'  => \&check_X_copyright,
    'libmarpa/win32/make.bat'           => \&trivial,
    'doc_dist/version.texi'   => \&trivial,
    'doc_dist/version_i.texi' => \&trivial,
    'doc1_dist/version.texi'   => \&trivial,
    'doc1_dist/version_i.texi' => \&trivial,
    'etc/my_suppressions'              => \&trivial,
    'libmarpa/tavl/README' => \&trivial,

    # Leave obstack licensing as is
    'dist/marpa_obs.c' => \&ignored,
    'dist/marpa_obs.h' => \&ignored,
    'cm_dist/marpa_obs.c' => \&ignored,
    'cm_dist/marpa_obs.h' => \&ignored,

    # Leave Pfaff's licensing as is
    'dist/marpa_avl.c' => \&ignored,
    'dist/marpa_avl.h' => \&ignored,
    'dist/marpa_tavl.c' => \&ignored,
    'dist/marpa_tavl.h' => \&ignored,
    'cm_dist/marpa_avl.c' => \&ignored,
    'cm_dist/marpa_avl.h' => \&ignored,
    'cm_dist/marpa_tavl.c' => \&ignored,
    'cm_dist/marpa_tavl.h' => \&ignored,
    'libmarpa/tavl/marpa_tavl.c' => \&ignored,
    'libmarpa/tavl/marpa_tavl.h' => \&ignored,
    'libmarpa/tavl/README.Pfaff' => \&ignored,
    'libmarpa/tavl/tavl-test.c'  => \&ignored,
    'libmarpa/tavl/test.c'       => \&ignored,
    'libmarpa/tavl/test.h'       => \&ignored,

    # MS .def file -- contents trivial
    'dist/win32/marpa.def' => \&ignored,
);

# Common files in the GNU distributions
for my $distlib (
    qw(work/ac_doc work/ac_doc1 work/ac dist doc_dist doc1_dist)
    )
{
    $files_by_type{"$distlib/AUTHORS"}   = \&trivial;
    $files_by_type{"$distlib/NEWS"}      = \&trivial;
    $files_by_type{"$distlib/ChangeLog"} = \&trivial;

    ## GNU license text, leave it alone
    $files_by_type{"$distlib/COPYING.LESSER"} = \&ignored;

    ## GNU standard -- has their license language
    $files_by_type{"$distlib/INSTALL"} = \&ignored;

    $files_by_type{"$distlib/README"}     = \&license_problems_in_text_file;
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
    return \&license_problems_in_perl_file
        if scalar @dirs > 1
            and $dirs[0] eq 'pperl'
            and $filepart =~ /[.]pm\z/xms;
    return \&ignored if $filepart =~ /[.]tar\z/xms;

    # info files are generated -- licensing is in source
    return \&ignored if $filepart =~ /[.]info\z/xms;
    return \&ignored
        if scalar @dirs >= 2
            and $dirs[0] eq 'libmarpa'
            and $dirs[1] eq 'orig';
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

sub license_problems_in_license_file {
    my ( $filename, $verbose ) = @_;
    my @problems = ();
    my $text     = ${ slurp($filename) };
    if ( $text ne $license_file ) {
        my $problem = "LICENSE file is wrong\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( \$text, \$license_file )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( $text ne $license_file )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== $filename should be as follows:\n"
            . $license_file
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
    my $text = slurp_top( $filename, length $r2_hash_license );
    if ( $r2_hash_license ne ${$text} ) {
        my $problem = "No license language in $filename (hash style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$r2_hash_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( $r2_hash_license ne ${$text} )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $r2_hash_license
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
    my $ref_text = slurp_top( $filename, 256 + length $r2_hash_license );
    my $text = ${$ref_text};
    $text =~ s/ \A [#][!] [^\n]* \n//xms;
    $text = substr $text, 0, length $r2_hash_license;
    if ( $r2_hash_license ne $text ) {
        my $problem = "No license language in $filename (sh hash style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( \$text, \$r2_hash_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    }
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $r2_hash_license
            . ( q{=} x 30 );
        push @problems, $problem;
    } ## end if ( scalar @problems and $verbose >= 2 )
    return @problems;
}

sub license_problems_in_perl_file {
    my ( $filename, $type, $verbose ) = @_;
    if ($verbose) {
        say {*STDERR} "Checking $filename as $type perl file" or die "say failed: $ERRNO";
    }
    $verbose //= 0;
    my @problems = ();
    my $text = slurp_top( $filename, 132 + length $r2_hash_license );

    # Delete hash bang line, if present
    ${$text} =~ s/\A [#][!] [^\n] \n//xms;
    if ( 0 > index ${$text}, $r2_hash_license ) {
        my $problem = "No license language in $filename (perl style)\n";
        if ($verbose) {
            $problem
                .= "=== Differences ===\n"
                . Text::Diff::diff( $text, \$r2_hash_license )
                . ( q{=} x 30 );
        } ## end if ($verbose)
        push @problems, $problem;
    } ## end if ( 0 > index ${$text}, $r2_hash_license )
    if ( scalar @problems and $verbose >= 2 ) {
        my $problem =
              "=== license for $filename should be as follows:\n"
            . $r2_hash_license
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

# In "Text" files, just look for the full language.
# No need to comment it out.
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
