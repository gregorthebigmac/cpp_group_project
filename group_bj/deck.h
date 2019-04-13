#pragma once
#include "card.h"

class deck {
public:
	deck();
	~deck();

	// DEBUG
	void _debug_dump_deck();
private:
	std::vector<card> m_cards;
};

