#ifndef HUMAN_PLAYER_H_INC
#define HUMAN_PLAYER_H_INC

#include "board.h"
#include "player.h"

class human_player : public player {
public:
	virtual unsigned move(board&, char) override;
};

#endif
