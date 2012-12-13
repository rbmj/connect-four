#ifndef RANGE_H_INC
#define RANGE_H_INC

#include <iterator>

template <class Iterator>
class range {
private:
	Iterator m_begin;
	Iterator m_end;
public:
	range(Iterator b, Iterator e) : m_begin(b), m_end(e) {}
	Iterator begin() { return m_begin; }
	Iterator end() { return m_end; }
};


#endif
