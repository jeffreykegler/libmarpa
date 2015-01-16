set -x
date > timings.out
for i in 0 1 2 3 4 5 6 7 8 9
do
time -o timings.out --append --format="XS0 %S %U %e" perl -MJSON::XS -MData::Dumper -E 'local $Data::Dumper::Indent = 0; $/=undef; say Data::Dumper::Dumper(JSON::XS::decode_json(<STDIN>))' < test.in
time -o timings.out --append --format="PP0 %S %U %e" perl -MJSON::PP -MData::Dumper -E 'local $Data::Dumper::Indent = 0; $/=undef; say Data::Dumper::Dumper(JSON::PP::decode_json(<STDIN>))' < test.in
time -o timings.out --append --format="a.out %S %U %e" ./a.out test.in
time -o timings.out --append --format="XS3 %S %U %e" perl -MJSON::XS -MData::Dumper -E 'local $Data::Dumper::Indent = 3; $/=undef; say Data::Dumper::Dumper(JSON::XS::decode_json(<STDIN>))' < test.in
time -o timings.out --append --format="PP3 %S %U %e" perl -MJSON::PP -MData::Dumper -E 'local $Data::Dumper::Indent = 3; $/=undef; say Data::Dumper::Dumper(JSON::PP::decode_json(<STDIN>))' < test.in
done >timing.log 2>&1
