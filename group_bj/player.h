#pragma once
#include "deck.h"
#include <string>
#include <vector>
#include <ctype.h>

class player {
public:
	player();
	~player(){}

	// getters
	std::string get_name() { return m_name; }
	int get_total();
	std::vector<char> get_player_aces();
	bool can_hit() { return m_can_hit; }
	bool can_flip_ace() { return m_can_flip_ace; }
	bool is_staying() { return m_stay; }
	std::deque<card> get_hand() { return m_hand; }

	// setters
	void set_name(std::string name) { m_name = name; }
	void set_can_hit(bool hit) { m_can_hit = hit; }
	void set_can_flip_ace(bool flip) { m_can_flip_ace = flip; }

	void display_hand();
	void stay() { m_stay = true; }
	void draw_card(card _card) { m_hand.push_back(_card); }
	void flip_ace();

private:
	std::string m_name;			// player name
	std::deque<card> m_hand;	// player's "hand" of cards
	int m_total;				// Sum of cards in player's hand.
	bool m_can_flip_ace;		// Flag to show if any of the player's cards are aces and can be flipped.
	bool m_can_hit;				// Can the player draw a card this turn?
	bool m_stay;				// Once this goes true, the player is ready to reveal their cards
};
