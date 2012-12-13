#include "board.h"
#include "bin.h"
#include "range.h"
#include <cassert>
#include <stdexcept>
#include <string>

board::board(unsigned x, unsigned y) : bins(x, bin(y)) {
	assert(x != 0 && y != 0);
}

void board::check_bounds(unsigned x, unsigned y) const {
	if (x >= num_bins() || y >= height()) {
		throw std::range_error("index out of range - cannot access");
	}
}

void board::check_parents(board::row_reference r, board::column_reference c) const {
	if (r.parent != c.parent || r.parent != this) {
		throw std::logic_error("iterator does not have correct parent");
	}
}

char board::operator()(unsigned x, unsigned y) const {
	check_bounds(x, y);
	return bins[x][y];
}

char board::operator()(board::row_reference r, board::column_reference c) const {
	check_parents(r, c);
	return (*this)(c.index, r.index);
}

board::position board::at(unsigned x, unsigned y) const {
	check_bounds(x, y);
	return position(x, y, this);
}

board::position board::at(board::row_reference r, board::column_reference c) const {
	check_parents(r, c);
	return at(c.index, r.index);
}

unsigned board::num_bins() const {
	return bins.size();
}

unsigned board::height() const {
	return bins[0].size();
}

range<board::row_iterator> board::by_rows() const {
	return make_range(
		row_iterator(0, this),
		row_iterator(height(), this)
	);
}
range<board::column_iterator> board::by_columns() const {
	return make_range(
		column_iterator(0, this),
		column_iterator(num_bins(), this)
	);
}
range<board::reverse_row_iterator> board::by_rows_reverse() const {
	return make_range(
		reverse_row_iterator(row_iterator(height(), this)),
		reverse_row_iterator(row_iterator(0, this))
	);
}
range<board::reverse_column_iterator> board::by_columns_reverse() const {
	return make_range(
		reverse_column_iterator(column_iterator(num_bins(), this)),
		reverse_column_iterator(column_iterator(0, this))
	);
}

bool board::insert(unsigned bin_num, char id) {
	return bins[bin_num].insert(id);
}

char board::position::operator*() const {
	return (*parent)(x, y);
}

//these move a position marker in the board array.  Note that as a key
//invariant if they fail they do NOT modify the position

void board::position::left() {
	if (x == 0) {
		throw std::range_error("cannot move left - index out of range");
	}
	--x;
}

void board::position::right() {
	if (++x == parent->num_bins()) {
		--x;
		throw std::range_error("cannot move right - index out of range");
	}
}

void board::position::up() {
	if (++y == parent->height()) {
		--y;
		throw std::range_error("cannot move up - index out of range");
	}
}

void board::position::down() {
	if (y == 0) {
		throw std::range_error("cannot move down - index out of range");
	}
	--y;
}

void board::position::upright() {
	up(); //if this fails just let it keep unwinding
	try {
		right(); //try moving right
	}
	catch (std::range_error& e) {
		--y; //we can't move right - so move back down
		throw; //rethrow exception
	}
}

//that idiom is repeated in the others:

void board::position::downright() {
	down();
	try {
		right();
	}
	catch (std::range_error& e) {
		++y;
		throw;
	}
}

void board::position::downleft() {
	down();
	try {
		left();
	}
	catch (std::range_error& e) {
		++y;
		throw;
	}
}

void board::position::upleft() {
	up();
	try {
		left();
	}
	catch (std::range_error& e) {
		--y;
		throw;
	}
}
