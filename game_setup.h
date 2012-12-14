#ifndef GAME_SETUP_H_INC
#define GAME_SETUP_H_INC

#include <iostream>
#include <vector>
#include <memory>

#include "board.h"
#include "player.h"

board setup_board();
std::vector<std::unique_ptr<player>> setup_players();
unsigned for_victory();

#endif
