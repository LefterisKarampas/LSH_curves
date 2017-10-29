CC = g++
OUT = main
FLAG = -g -Wall 
FILES = src/main.cpp src/read_curves.cpp src/LSH_Curve.cpp src/List.cpp src/HashTable.cpp src/HashFunctions.cpp src/Grid.cpp src/Curve.cpp src/generator.cpp src/Distance.cpp

all:
	$(CC) $(FLAG) -o $(OUT) $(FILES)
	
clean:
	rm -rf *.o $(OUT)
