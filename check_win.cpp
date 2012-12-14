#include "check_win.h"
#include "board.h"
#include "bin.h"
#include "range.h"

template <class Iterator>
bool check_win(range<Iterator> r, char id, unsigned to_win) {
	unsigned longest_sequence = 0;
	unsigned counter = 0;
	for (char x : r) {
		if (x == id) {
			++counter;
		}
		else {
			if (counter > longest_sequence) {
				longest_sequence = counter;
			}
			counter = 0;
		}
	}
	if (counter > longest_sequence) {
		longest_sequence = counter;
	}
	if (longest_sequence >= to_win) {
		return true;
	}
	else {
		return false;
	}
}
		
//for checking if they won, we can simplify because if we check every turn
//then we *know* that a winning sequence must include that cell
bool check_win(board& b, unsigned x, char id, unsigned to_win) { //x == bin of last move
	unsigned h = b.height();
	unsigned w = b.num_bins();
	unsigned y = 0;
	for (; y < h; ++y) {
		if (b[x][y] == 0) {
			break;
		}
	}
	--y;
	//last move was at x,y
	
	//check row
	if (check_win(range<adapters::horiz_it>(b.at(0, y), b.at(w, y)), id, to_win)) {
		return true;
	}
	
	//check column
	if (check_win(range<adapters::vert_it>(b.at(x, 0), b.at(x, h)), id, to_win)) {
		return true;
	}
	
	//check diagonals
	int intercept;
	int hs = h; //h signed
	int ws = w; //w signed
	unsigned xi;
	unsigned xf;
	unsigned yi;
	unsigned yf;
	
	//up slope:
	intercept = y - x;
	//all of these magic numbers are based off geometry.  I'm not going
	//to prove it, as this is a program, not a proof, but if you want to
	//see it, it's not hard.  My copy is on a receipt somewhere in the
	//trash :/
	if (intercept < 0) {
		xi = (unsigned) -intercept;
		yi = 0;
	}
	else {
		xi = 0;
		yi = (unsigned) intercept;
	}
	if (intercept < hs - ws) {
		xf = w;
		yf = (unsigned)(ws + intercept);
	}
	else {
		xf = (unsigned)(hs - intercept);
		yf = h;
	}
	
	if (check_win(range<adapters::pos_diag_it>(b.at(xi, yi), b.at(xf, yf)), id, to_win)) {
		return true;
	}
	
	//down slope
	//there are some unsigned -1s because i need an invalid index that is
	//= to --0, so the underflow is OK.  It will never be dereferenced.
	intercept = x + y;
	if (intercept < hs) {
		xi = 0;
		yi = (unsigned) intercept;
	}
	else {
		xi = (unsigned)(intercept - hs) + 1;
		yi = h - 1;
	}
	if (intercept < ws) {
		xf = (unsigned) intercept + 1;
		yf = (unsigned) -1;
	}
	else {
		xf = w;
		yf = (unsigned)(intercept - ws);
	}
	
	if (check_win(range<adapters::neg_diag_it>(b.at(xi, yi), b.at(xf, yf)), id, to_win)) {
		return true;
	}
	
	return false;
}
