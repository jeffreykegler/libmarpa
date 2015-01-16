#perl

use 5.010;
use strict;
use warnings;

print '[';
say q{[1,"abc\ndef",-2.3,null,[],[1,2,3],{},{"a":1,"b":2}],} for 1 .. 21195;
say q{[1,"abc\ndef",-2.3,null,[],[1,2,3],{},{"a":1,"b":2}]};
say ']'
