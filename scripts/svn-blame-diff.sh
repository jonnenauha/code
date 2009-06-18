#!/bin/bash

REVIEWEE=$1
R1=$2
R2=$3

echo $REVIEWEE
echo $R1:$R2

shift 3
while (("$#"));
do
    echo $1
    svn blame $1@$R1 > $1.before
    svn blame $1@$R2 > $1.after
    diff -u $1.before $1.after | awk '{ if ($1 == "+" && $3 != "'$REVIEWEE'"); else print }' > $1.blame-diff
    rm $1.before $1.after 
    shift
done

tar zcf code-review-$REVIEWEE-$R1-$R2.tgz $(find . -name '*.blame-diff')
rm $(find . -name '*.blame-diff')
