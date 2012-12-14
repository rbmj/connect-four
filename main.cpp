#include <iostream>
#include <vector>
#include <memory>

#include "board.h"
#include "board_io.h"
#include "player.h"

#include "game_setup.h"
#include "check_win.h"

int main() {
	board b = setup_board();
	auto players = setup_players();
	auto connect_n = for_victory();
	char winner = 0;
	while (winner == 0) {
		for (unsigned i = 0; i < players.size(); ++i) {
			char player_id = 'A' + i;
			std::cout << "Player " << player_id << "'s turn:\n";
			std::cout << b << std::endl;
			auto move = players[i]->move(b, player_id);
			if (check_win(b, move, player_id, connect_n)) {
				winner = player_id;
				break;
			}
			else {
				bool full = true;
				for (unsigned j = 0; j < b.num_bins(); ++j) {
					if (!b[j].full()) {
						full = false;
						break;
					}
				}
				if (full) {
					winner = -1; //indicates a draw
					break;
				}
			}
		}
	}
	std::cout << b << std::endl;
	if (winner == -1) {
		std::cout << "This game was a draw.\n";
	}
	else {
		std::cout << "Player " << winner << " won!\n";
	}
	return 0;
}

