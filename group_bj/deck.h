#pragma once
#include <deque>
#include "card.h"

class deck {
public:
	deck();
	~deck();

	// DEBUG
	void _debug_dump_deck();

private:
	std::deque<card> m_cards;
};

