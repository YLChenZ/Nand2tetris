
load IsNg16.hdl,
output-file IsNg16.out,
compare-to IsNg16.cmp,
output-list in%B1.16.1 out;

set in %B0000000000000000,
eval,
output;

set in %B1111111111111111,
eval,
output;

set in %B1010101010101010,
eval,
output;

set in %B0011110011000011,
eval,
output;

set in %B0001001000110100,
eval,
output;