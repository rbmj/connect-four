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

unsigned random_ai::move(board& b, char id) {
	std::vector<unsigned> free_bins;
	for (unsigned i = 0; i < b.num_bins(); ++i) {
		if (!b[i].full()) {
			free_bins.push_back(i);
		}
	}
	assert(free_bins.size() != 0); //should be checked for elsewhere
	unsigned into = free_bins[random_generator()() % free_bins.size()];
	bool success = b.insert(into, id);
	assert(success);
	think();
	return into;
}
	
