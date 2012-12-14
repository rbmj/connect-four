#include "human_player.h"
#include <iostream>

void human_player::move(board& b, char id) {
	bool moved = false;
	while (!moved) {
		std::cout << "Where would you like to go? ";
		unsigned bin;
		std::cin >> bin;
		--bin; //change from 1..x to 0..x-1
		if (bin >= b.num_bins()) {
			std::cout << "That is not a valid bin.  Try again.\n";
		}
		else {
			moved = b.insert(bin, id);
			if (!moved) {
				std::cout << "That bin is full.  Try again.\n";
			}
		}
	}
}
