#include "game_setup.h"

#include <iostream>
#include <vector>
#include <memory>

#include "board.h"
#include "player.h"
#include "player_factory.h"
#include "human_player.h"
#include "random_ai.h"

//TODO:  Make setup_board and for_victory more robust with input

//helper setup function prototypes
unsigned get_num_players();

board setup_board() {
	std::cout << "How many bins? ";
	unsigned bins;
	std::cin >> bins;
	std::cout << "How high? ";
	unsigned height;
	std::cin >> height;
	return board(bins, height);
}

//now for the different players types:
player_factory<human_player> human("Human");
player_factory<random_ai> computer_random("Dumb Computer");

const player_factory_t* player_types[] = {
	&human,
	&computer_random
};
//wish there was a way to have this collection without having
//to give each of its elements a name...

std::vector<std::unique_ptr<player>> setup_players() {
	unsigned num_players = get_num_players();
	
	std::vector<std::unique_ptr<player>> players;

	for (unsigned i = 0; i < num_players; ++i) {
		std::cout << "Please choose a player type for player " << (char)('A'+i) << ':';
		for (unsigned j = 0; j < sizeof(player_types)/sizeof(player_types[0]); ++j) {
			std::cout << "\n\t" << j+1 << "- " << player_types[j]->name();
		}
		std::cout << '\n';
		unsigned player_type = (unsigned)-1;
		while (player_type == (unsigned)-1) {
			std::cout << "Player type: ";
			std::cin >> player_type;
			--player_type; //nice enough, if it's zero it'll underflow
			if (player_type >= sizeof(player_types)/sizeof(player_types[0])) {
				std::cout << "Invalid player type, try again.\n";
				player_type = (unsigned)-1;
			}
		}
		players.push_back(player_types[player_type]->create());
	}
	return players;
}

unsigned get_num_players() {
	unsigned num_players = 0;
	while (num_players == 0) {
		std::cout << "How many players? ";
		std::cin >> num_players;
		if (num_players < 2) {
			std::cout << "You need to have at least two players.  Try again\n";
			num_players = 0;
		}
		else if (num_players > 26) {
			std::cout << "Currently this program only supports games with <= 26 players.  Try again\n";
			num_players = 0;
		}
	}
	return num_players;
}

unsigned for_victory() {
	std::cout << "Connect how many in a row for victory? ";
	unsigned count;
	std::cin >> count;
	return count;
}
