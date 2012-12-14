#ifndef RANDOM_AI_H_INC
#define RANDOM_AI_H_INC

#include "board.h"
#include "player.h"

#include <random>

std::minstd_rand& random_generator();

void think();

class random_ai : public player {
public:
	virtual unsigned move(board&, char) override;
};

#endif
