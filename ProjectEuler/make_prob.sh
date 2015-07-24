for i in $*; do
	mkdir $i
	sed -e s/\\\$NAME\\\$/$i/g template.cpp > $i/$i.cpp
	cp ./Makefile $i/Makefile
done
