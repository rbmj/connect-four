#include "human_player.h"
#include <iostream>

unsigned human_player::move(board& b, char id) {
	bool moved = false;
	unsigned bin;
	while (!moved) {
		std::cout << "Where would you like to go? ";
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
	return bin;
}
