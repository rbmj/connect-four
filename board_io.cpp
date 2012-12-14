#include "board_io.h"

#include <iostream>
#include <vector>

#include "bin.h"
#include "board.h"
#include "range.h"

//these three functions are private to this module:
void output_divider(std::ostream&, const board&, char);
void output_indexes(std::ostream&, const board&);
void output_data(std::ostream&, const board&);
char printable_id(char);

//big output function
std::ostream& operator<<(std::ostream& str, const board& b) {
	output_divider(str, b, '+');
	output_indexes(str, b);
	output_divider(str, b, '|');
	output_data(str, b);
	output_divider(str, b, '+');
	return str;
}


void output_divider(std::ostream& str, const board& b, char end) {
	str << end << "---";
	for (unsigned i = 1; i < b.num_bins(); ++i) { //already did 0...
		str << "----";
	}
	str << end << '\n';
}

void output_indexes(std::ostream& str, const board& b) {
	str << '|';
	for (unsigned i = 1; i <= b.num_bins(); ++i) { //<= because starting at 1
		str << ' ' << i << " |";
	}
	str << '\n';
}

//and *this beauty* is what all the work in board.cpp was for :)
void output_data(std::ostream& str, const board& b) {
	for (auto&& row : b.by_rows_reverse()) { //reverse so from top to bottom
		str << '|';
		for (auto&& element : row) {
			str << ' ' << printable_id(element) << " |";
		}
		str << '\n';
	}
}

//that Meyers talk on universal references was actually useful...

char printable_id(char c) {
	if (c) {
		return c;
	}
	else {
		return ' ';
	}
}
