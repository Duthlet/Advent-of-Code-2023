s/\(.*\) \(.*\)/{"\1", \2},/
1s/\(.*\)/std::vector<std::pair<std::string, u32>> input {\n\1/
$s/,$/};/
