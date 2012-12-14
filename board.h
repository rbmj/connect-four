#ifndef BOARD_H_INC
#define BOARD_H_INC

#include <vector>
#include <iterator>

#include "bin.h"
#include "range.h"

//note to self: in free time develop mixin library to make writing
//iterators less tedious.

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
	typedef std::vector<bin>::iterator bin_iterator;
	typedef std::vector<bin>::reverse_iterator reverse_bin_iterator;
	typedef std::vector<bin>::const_iterator const_bin_iterator;
	typedef std::vector<bin>::const_reverse_iterator const_reverse_bin_iterator;
	
	board(unsigned, unsigned); //make board(x, y) with dimensions
	board(const board&) = default;
	board(board&&) = default;
	
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
	range<bin_iterator> by_bins();
	range<reverse_bin_iterator> by_bins_reverse();
	range<const_bin_iterator> by_bins() const;
	range<const_reverse_bin_iterator> by_bins_reverse() const;
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

//This class is a template in order to reduce the amount of boilerplate.
//there's already enough of that in this file.
template <bool Row>
class board::reference {
	friend class board::iterator<board::reference<Row>>;
	friend class board;
private:
	unsigned index;
	const board * parent;
	reference(unsigned i, const board * p) : index(i), parent(p) {}
public:
	typedef board::reference<Row> Reference;
	reference(const Reference&) = default;
	
	class iterator;
	friend class Reference::iterator;
	
	//have to define this later...
	iterator begin();
	iterator end();
	
	typedef std::reverse_iterator<iterator> reverse_iterator;
	
	range<reverse_iterator> reverse() {
		return make_range(reverse_iterator(end()), reverse_iterator(begin()));
	}
};


//so much of this is directly copied from board::iterator...
//it makes me angry.  
template <bool Row>
class board::reference<Row>::iterator {
	friend class board::reference<Row>;
private:
	unsigned index;
	board::reference<Row> * ref;
	iterator(unsigned i, board::reference<Row>* r) : index(i), ref(r) {}
public:
	typedef board::reference<Row>::iterator Self;
	iterator(const Self&) = default;
	iterator() = default;
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
	Self& operator+=(unsigned offset) {
		index += offset;
	}
	Self& operator-=(unsigned offset) {
		index -= offset;
	}
	Self operator+(unsigned offset) const {
		return Self(*this) += offset;
	}
	Self operator-(unsigned offset) const {
		return Self(*this) -= offset;
	}
	char operator*() const {
		if (Row) {
			//ref's index is the row, this's index is the column
			return (*(ref->parent))(index, ref->index);
		}
		else {
			//ref's index is the column, this's index is the row
			return (*(ref->parent))(ref->index, index);
		}
	}
	board::position at() {
		if (Row) {
			return ref->parent->at(index, ref->index);
		}
		else {
			return ref->parent->at(ref->index, index);
		}
	}
	char operator[](unsigned offset) {
		return *(*this + offset);
	}
	bool operator<(const Self& other) { return index < other.index; }
	bool operator<=(const Self& other) { return index <= other.index; }
	bool operator>(const Self& other) { return index > other.index; }
	bool operator>=(const Self& other) { return index >= other.index; }
	bool operator==(const Self& other) { return index == other.index; }
	bool operator!=(const Self& other) { return index != other.index; }
	
};

template <bool Row>
inline typename board::reference<Row>::iterator board::reference<Row>::begin() {
	return iterator(0, this);
}

template <bool Row>
inline typename board::reference<Row>::iterator board::reference<Row>::end() {
	if (Row) {
		//reference is a row, so need number of columns
		return iterator(parent->num_bins(), this);
	}
	else {
		//reference is a column, need number of rows
		return iterator(parent->height(), this);
	}
}

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
