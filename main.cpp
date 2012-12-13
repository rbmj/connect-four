#include <iostream>

#include "board.h"
#include "board_io.h"

int main() {
	std::cout << "How many bins? ";
	unsigned bins;
	std::cin >> bins;
	std::cout << "How high? ";
	unsigned height;
	std::cin >> height;
	board b(bins, height);
	std::cout << b;
	return 0;
}
