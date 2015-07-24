
DIRECTORY=$( cd "$( dirname $0 )" && pwd)

for filepath in $*; do
	mkdir -p $filepath
	filename=$(basename "$filepath")
	sed -e s/\\\$NAME\\\$/$filename/g $DIRECTORY/template.cpp > $filepath/$filename.cpp
	cp $DIRECTORY/Makefile $filepath/Makefile
	ln -s $DIRECTORY/.dcj $filepath/dcj
done
