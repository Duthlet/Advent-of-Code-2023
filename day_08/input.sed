1s/\(.*\)/std::string directions {"\1"};/
s/^$/std::vector<std::vector<std::string>> input {/
s/\(.*\) = (\(.*\), \(.*\))/{"\1","\2","\3"},/
$s/,$/};/
