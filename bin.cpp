#include "bin.h"

bin::bin(unsigned height) : positions(height, 0) {
	//
}

bool bin::insert(char id) {
	for (auto& x: positions) { //find first 
		if (x == 0) {
			x = id;
			return true;
		}
	}
	return false;
}

char bin::operator[](unsigned i) const {
	return positions[i]; //not bounds checked
}

unsigned bin::size() const {
	return positions.size();
}
