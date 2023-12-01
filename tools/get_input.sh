if [[ $# > 1 ]]
then 
    echo 'Unexpected number of arguments'
    exit 1
fi

if [[ $# == 0 ]]
then 
    _DATE='now'
else
    _DATE=$1
fi

curl "`date -d $_DATE +https://adventofcode.com/%Y/day/%-d/input`" --cookie "$AOC_COOKIE"
