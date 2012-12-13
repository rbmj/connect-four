#ifndef BOARD_H_INC
#define BOARD_H_INC

#include <vector>
#include <iterator>

#include "bin.h"
#include "range.h"

class board {
private:
	std::vector<bin> bins;
	template <class Reference>
	class iterator;
	template <bool Row>
	class reference;
public:
	class position;
	typedef reference<true> row_reference;
	typedef reference<false> column_reference;
	typedef iterator<row_reference> row_iterator;
	typedef iterator<column_reference> column_iterator;
	typedef std::reverse_iterator<row_iterator> reverse_row_iterator;
	typedef std::reverse_iterator<column_iterator> reverse_column_iterator;
	
	board(unsigned, unsigned); //make board(x, y) with dimensions
	char operator()(unsigned, unsigned) const;
	char operator()(row_reference, column_reference) const;
	position at(unsigned, unsigned) const;
	position at(row_reference, column_reference) const;
	unsigned num_bins() const;
	unsigned height() const;
	range<row_iterator> by_rows() const;
	range<column_iterator> by_columns() const;
	range<reverse_row_iterator> by_rows_reverse() const;
	range<reverse_column_iterator> by_columns_reverse() const;
	bool insert(unsigned, char);
private:
	void check_bounds(unsigned, unsigned) const;
	void check_parents(row_reference, column_reference) const;
};

//this class is sort of like an iterator except it's just pointing
//into a 2-d range, and can move through in different directions
class board::position {
	friend class board;
private:
	unsigned x;
	unsigned y;
	const board * parent;
	position(unsigned a, unsigned b, const board* p) : x(a), y(b), parent(p) {}
public:
	position(const board::position&) = default;
	char operator*() const;
	void left();
	void right();
	void up();
	void down();
	void upright();
	void upleft();
	void downright();
	void downleft();
};

//a random access iterator that can iterate through the rows/columns
//of the board.  Most of this is trivial.
template <class Reference>
class board::iterator {
	friend class board;
private:
	unsigned index;
	const board * parent;
	iterator(unsigned i, const board * p) : index(i), parent(p) {}
public:
	typedef iterator<Reference> Self;
	iterator() = default; //not usable until assignment
	iterator(const Self&) = default;
	//note that, like most iterators, these iterators are not
	//bounds-checked.  However, if they are out of bounds and
	//you call board::at(*a, *b); it will fail *that* bounds check.
	Self& operator++() {
		++index;
		return *this;
	}
	Self operator++(int) {
		Self ret(*this);
		++(*this);
		return ret;
	}
	Self& operator--() {
		--index;
		return *this;
	}
	Self operator--(int) {
		Self ret(*this);
		--(*this);
		return ret;
	}
	Self& operator+=(int offset) {
		index += offset;
		return *this;
	}
	const Self operator+(int offset) const {
		return Self(*this) += offset;
	}
	Self& operator-=(int offset) {
		index -= offset;
		return *this;
	}
	const Self operator-(int offset) const {
		return Self(*this) -= offset;
	}
	Reference operator*() const {
		return Reference(index, parent);
	}
	Reference operator[](int offset) const {
		return *(*this + offset);
	}
	bool operator<(const Self& other) { return index < other.index; }
	bool operator>(const Self& other) { return index > other.index; }
	bool operator==(const Self& other) { return index == other.index; }
	bool operator!=(const Self& other) { return index != other.index; }
	bool operator<=(const Self& other) { return index <= other.index; }
	bool operator>=(const Self& other) { return index >= other.index; }
	//for std::iterator_traits
	typedef row_iterator difference_type;
	typedef Reference value_type;
	typedef Reference pointer;
	typedef Reference reference;
	typedef std::random_access_iterator_tag iterator_category;
};

//board::reference<Row>: right now just a dummy so that range-based
//for will work nicely.  Later on might allow iteration in the second
//dimension, but for right now that's handled by combining a row_reference
//with a column_reference into a position, and then using an adapter.

//this could be one class, but I want it to be two different types
//to enforce that one has both a row and column reference and not
//e.g. two row references.  Thus, for type saftey, there's a dummy
//parameter.  This might be used later on if I ever decide to flush
//out this class, but I don't think that the current assignment needs it
template <bool Row>
class board::reference {
	friend class board::iterator<board::reference<Row>>;
	friend class board;
private:
	unsigned index;
	const board * parent;
	reference(unsigned i, const board * p) : index(i), parent(p) {}
public:
	reference(const reference&) = default;
	//i don't think I need to implement anything here yet
};

//the adapters in this class map different types of movement through
//the board to different type of iterators

//note that these aren't full fleged iterators as they don't support
//comparisons.  This is because for now I don't need it...
namespace adapters {
	
	//helper typedef
	typedef void(board::position::*movement_func)();
	
	//this saves me a lot of typing...
	template <movement_func Decrement, movement_func Increment>
	class generic_adapter {
	private:
		board::position& pos;
	public:
		typedef generic_adapter<Decrement, Increment> Self;
		
		generic_adapter(board::position& p) : pos(p) {} //allow implicit
		generic_adapter(const Self&) = default;
		
		char operator*() const { return *pos; } //delegate
		
		Self& operator++() {
			(pos.*Increment)();
			return *this;
		}
		
		Self operator++(int) {
			Self ret(*this);
			++(*this);
			return ret;
		}
		
		Self& operator--() {
			(pos.*Decrement)();
			return *this;
		}
		
		Self operator--(int) {
			Self ret(*this);
			--(*this);
			return ret;
		}
	}; //so that whole class is basically boilerplate...
	
	//different adapters
	typedef generic_adapter<&board::position::left, &board::position::right> horiz_it;
	typedef generic_adapter<&board::position::down, &board::position::up> vert_it;
	typedef generic_adapter<&board::position::downleft, &board::position::upright> pos_diag_it;
	typedef generic_adapter<&board::position::upleft, &board::position::downright> neg_diag_it;
	
}
			
			

#endif
