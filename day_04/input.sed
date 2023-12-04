s/.*://
s/|/}, {/
s/\(.*\)/{{\1}},/
s/\([0-9]\) \+\([0-9]\)/\1, \2/g
s/\([0-9]\) \+\([0-9]\)/\1, \2/g
1s/\(.*\)/std::vector<std::pair<std::vector<int>, std::vector<int>>> input {\n\1/
$s/,$/};/
