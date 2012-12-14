#ifndef PLAYER_H_INC
#define PLAYER_H_INC

#include "board.h"

//abstract source of moves
class player {
public:
	//this interface design is not the best in terms of loose coupling, but
	//its a lot easier to handle human vs. AI movers this way...
	
	//do move, but return *where* you moved so we can check if you won!
	virtual unsigned move(board& b, char) = 0;
	virtual ~player() {}
};

#endif
