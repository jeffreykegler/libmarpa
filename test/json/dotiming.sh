MARPA_JSON=a.out

if [[ $OSTYPE == cygwin ]]; then
	MARPA_JSON=a.exe
fi

TIME="env time"

set -x
date > timings.out
for i in 0 1 2 3 4 5 6 7 8 9
do
$TIME -o timings.out --append --format="XS0 %S %U %e" perl -MJSON::XS -MData::Dumper -E 'local $Data::Dumper::Indent = 0; $/=undef; say Data::Dumper::Dumper(JSON::XS::decode_json(<STDIN>))' < test.in
$TIME -o timings.out --append --format="PP0 %S %U %e" perl -MJSON::PP -MData::Dumper -E 'local $Data::Dumper::Indent = 0; $/=undef; say Data::Dumper::Dumper(JSON::PP::decode_json(<STDIN>))' < test.in
$TIME -o timings.out --append --format="$MARPA_JSON %S %U %e" ./$MARPA_JSON test.in
$TIME -o timings.out --append --format="XS3 %S %U %e" perl -MJSON::XS -MData::Dumper -E 'local $Data::Dumper::Indent = 3; $/=undef; say Data::Dumper::Dumper(JSON::XS::decode_json(<STDIN>))' < test.in
$TIME -o timings.out --append --format="PP3 %S %U %e" perl -MJSON::PP -MData::Dumper -E 'local $Data::Dumper::Indent = 3; $/=undef; say Data::Dumper::Dumper(JSON::PP::decode_json(<STDIN>))' < test.in
done >timing.log 2>&1
