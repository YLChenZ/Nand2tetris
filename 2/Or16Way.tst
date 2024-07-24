
load Or16Way.hdl,
output-file Or16Way.out,
compare-to Or16Way.cmp,
output-list in%B1.16.1 out;

set in %B0000000000000000,
eval,
output;

set in %B1111111111111111,
eval,
output;

set in %B0001000000000000,
eval,
output;

set in %B0000000100000001,
eval,
output;

set in %B0010011000100110,
eval,
output;