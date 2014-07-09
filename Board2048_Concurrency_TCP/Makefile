all: clean board.o main.o GameStart ClientStart

# to get .o file DO NOT using "-o" while using "-c"
# "-c" means "Just complie, Not Link"
# using nothing Means "Compile and Link"
# "-o" means "output file name is"

board.o: board.h
	g++ -c -g board.cpp 
main.o:	board.h  
	g++ -c main.cpp
GameStart: 
	g++ -o GameStart main.o board.o -pthread
ClientStart:
	g++ -o ClientStart client.cpp
clean:
	rm -rf GameStart ClientStart *.o
