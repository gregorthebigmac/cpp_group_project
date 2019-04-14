#pragma once
#include "deck.h"
#include <string>

class player {
public:
	player(){}
	~player(){}

	// getters
	std::string get_name() { return m_name; }
	int get_total() { return m_total; }
	std::deque<card> get_hand() { return m_hand; }

	// setters
	void set_name(std::string name) { m_name = name; }
	void deal_card(card dealt_card) { m_hand.push_back(dealt_card); }
	void display_hand();

private:
	std::string m_name;	// player name
	std::deque<card> m_hand;	// player's "hand" of cards
	int m_total;
};
