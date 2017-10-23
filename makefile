CC = g++
OUT = main
FLAG = -g -Wall 
FILES = main.cpp read_curves.cpp LSH_Curve.cpp List.cpp HashTable.cpp HashFunctions.cpp Grid.cpp Curve.cpp generator.cpp

all:
	$(CC) $(FLAG) -o $(OUT) $(FILES)
	
clean:
	rm -rf *.o $(OUT)
