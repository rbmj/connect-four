#include "random_ai.h"
#include "board.h"
#include "bin.h"

#include <chrono>
#include <thread>
#include <random>
#include <vector>
#include <cassert>

void think() {
	//simulate 'thinking' about a move
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

std::minstd_rand& random_generator() {
	//just to avoid issues with static initialization:
	//construct-on-first-use
	static std::minstd_rand * gen = new std::minstd_rand(
		std::chrono::system_clock::now().time_since_epoch().count()
	);
	//NOT A MEMORY LEAK.  Object lasts entire process lifetime and is
	//reclaimed by the operating system when the page is freed after
	//process exit
	return *gen;
}

void random_ai::move(board& b, char id) {
	std::vector<bin*> bins;
	for (bin& current : b.by_bins()) {
		if (!current.full()) {
			bins.push_back(&current);
		}
	}
	assert(bins.size() != 0); //should be checked for elsewhere
	bool success = bins[random_generator()()]->insert(id);
	assert(success);
	think();
}
	
