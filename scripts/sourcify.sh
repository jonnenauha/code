# parse parameters
name=$1; shift
until [ -z "$1" ]  
do srchfiles="$srchfiles $1"; shift
done

files=$(ls $srchfiles)
first=1

# output files
echo -ne $name'_SOURCES = '
for file in $files
do
    if test $((first)) -eq 1
    then
	echo -ne $file
	first=0
    else
	echo -ne ' \' 
	echo -ne '\n\t' $file
    fi
done
echo -ne '\n'
