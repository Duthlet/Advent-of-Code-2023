s/,//g
s/;//g
s/.*: //
s/\(.*\)/"\1",/
1s/\(.*\)/std::vector<std::string> input {"",\n\1/
$s/,$/};/
