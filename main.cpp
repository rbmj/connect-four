#include <iostream>
#include <vector>
#include <memory>

#include "board.h"
#include "board_io.h"
#include "player.h"

#include "game_setup.h"

int main() {
	board b = setup_board();
	auto players = setup_players();
	std::cout << b;
	return 0;
}

