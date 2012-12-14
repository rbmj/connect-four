#ifndef BIN_H_INC
#define BIN_H_INC

#include <vector>

class bin {
private:
	//position vector
	std::vector<char> positions;
	//lowest index (positions[0]) is lowest vertically
	//each position is an char representing player ID, 0 is empty
public:
	explicit bin(unsigned); //construct a bin, given height
	bin(const bin&) = default;
	bin(bin&&) = default;
	
	bool insert(char); //insert into bin, ret. false if failure
	char operator[](unsigned) const; //note this does not return a reference!
	unsigned size() const;
	bool full() const;
	
};

#endif
