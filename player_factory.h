#ifndef PLAYER_FACTORY_H_INC
#define PLAYER_FACTORY_H_INC

#include <memory>
#include "player.h"

class player_factory_t {
public:
	virtual std::unique_ptr<player> create() const = 0;
	virtual const char * name() const = 0;
};

template <class Player>
class player_factory : public player_factory_t {
private:
	const char * name_string;
public:
	player_factory(const char * tag) : name_string(tag) {}
	virtual std::unique_ptr<player> create() const override {
		return std::unique_ptr<player>(new Player);
	}
	virtual const char * name() const override {
		return name_string;
	}
};

#endif
