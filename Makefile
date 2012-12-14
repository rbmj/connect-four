CFLAGS=-std=c++11 -g -Wall
OBJECTS=main.o bin.o board.o board_io.o game_setup.o human_player.o random_ai.o

all: connect-four

clean:
	rm *.o
	rm connect-four
	
connect-four: $(OBJECTS)
	g++ $(CFLAGS) -o connect-four $(OBJECTS)
	
main.o: main.cpp board.h board_io.h player.h game_setup.h
	g++ $(CFLAGS) -c -o main.o main.cpp
	
bin.o: bin.cpp bin.h
	g++ $(CFLAGS) -c -o bin.o bin.cpp

board.o: board.cpp board.h bin.h range.h
	g++ $(CFLAGS) -c -o board.o board.cpp
	
board_io.o: board_io.cpp board_io.h board.h bin.h range.h
	g++ $(CFLAGS) -c -o board_io.o board_io.cpp

game_setup.o: game_setup.cpp board.h player.h player_factory.h human_player.h random_ai.h
	g++ $(CFLAGS) -c -o game_setup.o game_setup.cpp

human_player.o: human_player.cpp human_player.h
	g++ $(CFLAGS) -c -o human_player.o human_player.cpp
	
random_ai.o: random_ai.cpp random_ai.h bin.h board.h
	g++ $(CFLAGS) -c -o random_ai.o random_ai.cpp

