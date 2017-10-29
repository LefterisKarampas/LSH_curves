CC = g++
OUT = lsh
FLAG = -g -Wall 
FILES = src/main.cpp src/main_fuctionality.cpp src/LSH_Curve.cpp src/List.cpp src/HashTable.cpp src/HashFunctions.cpp \
src/Grid.cpp src/Curve.cpp src/generator.cpp src/Distance.cpp src/Node.cpp

OBJECTS = objects/main.o objects/main_fuctionality.o objects/LSH_Curve.o objects/List.o  objects/Node.o\
objects/HashTable.o objects/HashFunctions.o objects/Grid.o objects/Curve.o objects/generator.o objects/Distance.o

all:
	for i in $(FILES); do \
		$(CC) $(FLAG) -c $$i;  \
	done
	mkdir -p objects
	mv *.o objects
	$(CC) $(FLAG) -o $(OUT) $(OBJECTS)
clean:
	rm -rf objects/ $(OUT)
