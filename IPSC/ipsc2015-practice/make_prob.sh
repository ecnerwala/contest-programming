for filepath in $*; do
	mkdir -p $filepath
	filename=$(basename "$filepath")
	sed -e s/\\\$NAME\\\$/$filename/g template.cpp > $filepath/$filename.cpp
	cp ./Makefile $filepath/Makefile
done
