#ifndef PLAYER_H_INC
#define PLAYER_H_INC

#include "board.h"

//abstract source of moves
class player {
public:
	//not the best in terms of loose coupling, but it's a lot easier
	//to handle human vs. AI movers this way...
	virtual void move(board& b, char) = 0;
	virtual ~player() {}
};

#endif
