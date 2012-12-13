CFLAGS=-std=c++11
OBJECTS=main.o bin.o board.o board_io.o

all: connect-four

clean:
	rm *.o
	rm connect-four
	
connect-four: $(OBJECTS)
	g++ $(CFLAGS) -o connect-four $(OBJECTS)
	
main.o: main.cpp board.h board_io.h
	g++ $(CFLAGS) -c -o main.o main.cpp
	
bin.o: bin.cpp bin.h
	g++ $(CFLAGS) -c -o bin.o bin.cpp

board.o: board.cpp board.h bin.h range.h
	g++ $(CFLAGS) -c -o board.o board.cpp
	
board_io.o: board_io.cpp board_io.h board.h bin.h range.h
	g++ $(CFLAGS) -c -o board_io.o board_io.cpp


