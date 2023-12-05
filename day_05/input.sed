:start
s/\([0-9]\) \+\([0-9]\)/\1, \2/g
s/\([0-9]\) \+\([0-9]\)/\1, \2/g
s/\(^[0-9].*\)/{\1},/
s/-/_/g
s/\(.*\) map:/std::vector<mapping> \1 {/
1s/seeds: \(.*\)/std::vector<uint64_t> seeds {\1,/
$s/,$/};/
h
N
s/,\n$/};\n/
t keep
x
p
x
s/.*\n//
b start
: keep
