s/\(.*\):\(.*\)/std::vector<u64> \1 {\2 \n \2};/
s/\([0-9]\) /\1, /g
P
s/.*\n\(.*\)/\1/
s/[ ,]//g
