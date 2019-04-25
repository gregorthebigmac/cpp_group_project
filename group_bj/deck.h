#pragma once
#include <deque>
#include <stdlib.h>
#include <time.h>
#include "card.h"

class deck {
public:
	deck();
	~deck(){};

	void shuffle_deck();
	card draw_card();

	// DEBUG
	void _debug_dump_deck();

private:
	std::deque<card> m_cards;
};
