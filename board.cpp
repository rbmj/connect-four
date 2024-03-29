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
	//check_bounds(x, y); //don't check bounds until dereference
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
 /*
//this has a slightly different semantic meaning, and is less abstract
//than by_columns()... but here, we'll allow non-const
range<board::bin_iterator> board::by_bins() {
	return make_range(bins.begin(), bins.end());
}

range<board::reverse_bin_iterator> board::by_bins_reverse() {
	return make_range(bins.rbegin(), bins.rend());
}

range<board::const_bin_iterator> board::by_bins() const {
	return make_range(bins.cbegin(), bins.cend());
}

range<board::const_reverse_bin_iterator> board::by_bins_reverse() const {
	return make_range(bins.crbegin(), bins.crend());
}
*/

bin& board::operator[](unsigned i) {
	return bins[i];
}

const bin& board::operator[](unsigned i) const {
	return bins[i];
}

bool board::insert(unsigned bin_num, char id) {
	return bins[bin_num].insert(id);
}

char board::position::operator*() const {
	return (*parent)(x, y);
}

void board::position::left() {
	--x;
}

void board::position::right() {
	++x;
}

void board::position::up() {
	++y;
}

void board::position::down() {
	--y;
}

void board::position::upright() {
	up();
	right();
}

void board::position::downright() {
	down();
	right();
}

void board::position::downleft() {
	down();
	left();
}

void board::position::upleft() {
	up();
	left();
}

bool board::position::operator==(const board::position& other) const {
	return (x == other.x) && (y == other.y);
}

bool board::position::operator!=(const board::position& other) const {
	return !(*this == other);
}
