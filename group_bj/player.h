#pragma once
#include "card.h"

class player {
public:
	player(){}
	~player(){}

	// getters
	int get_low_total() { return m_low_total; }
	int get_high_total() { return m_high_total; }
	std::vector<card> get_hand() { return m_hand; }

	void deal_card(card dealt_card) { m_hand.push_back(dealt_card); }
	void calc_total();
	void display_hand();

private:
	std::vector<card> m_hand;
	int m_high_total;
	int m_low_total;
};
