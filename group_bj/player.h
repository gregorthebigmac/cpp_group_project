#pragma once
#include "deck.h"
#include <string>
#include <vector>
#include <ctype.h>
#include <exception>

class player {
public:
	player();	// ctor
	~player(){}	// dtor

	// getters
	int get_total();
	std::vector<char> get_player_aces();
	std::string get_name() { return m_name; }
	std::deque<card> get_hand() { return m_hand; }
	bool can_flip_ace() { return m_can_flip_ace; }
	bool is_staying() { return m_stay; }
	bool is_turn_over() { return m_turn_end; }
	int get_bet() { return m_bet; }
	bool has_screen_control() { return m_controls_screen; }
	int get_money() { return m_wallet; }
	bool has_hit_this_turn() { return m_hit_this_turn; }
	bool can_hit();
	bool can_stay() { return m_can_stay; }

	// setters
	void set_name(int player_num);
	void begin_turn() { m_turn_end = false; }
	void end_turn() { m_turn_end = true; }
	void set_can_hit(bool hit) { m_can_hit = hit; }
	void set_can_flip_ace(bool flip) { m_can_flip_ace = flip; }
	void reset_bet() { m_bet = 0; }
	void stay() { m_stay = true; }
	void toggle_screen_control() { m_controls_screen = !m_controls_screen; }
	void set_hit_this_turn(bool hit) { m_hit_this_turn = hit; }
	void set_can_stay(bool stay) { m_can_stay = stay; }

	// doers
	void place_bet();
	void display_hand(char me_or_them);
	void flip_ace();
	void draw_card(card _card) { m_hand.push_back(_card); }
	void increase_bet(int amount) { m_bet = m_bet + amount; }
	void win_bet(int their_bet) { m_wallet = m_wallet + m_bet + their_bet; }
	void round_is_a_draw() { m_wallet = m_wallet + m_bet; }
	void reset_hand() { m_hand.clear(); }
	void match_bet(int difference);
	void raise_bet();
	bool confirm(std::string question);

private:
	std::string m_name;			// player name
	std::deque<card> m_hand;	// player's "hand" of cards
	int m_total;				// Sum of cards in player's hand.
	bool m_can_flip_ace;		// Flag to show if any of the player's cards are aces and can be flipped.
	bool m_can_hit;				// If the player has less than 21, this is TRUE.
	bool m_can_stay;			// If the player busts, this is not an option, and therefore FALSE.
	bool m_hit_this_turn;		// If the player drew a card this turn, then this is TRUE.
	bool m_stay;				// Once this goes TRUE, the player is ready to reveal their cards
	bool m_turn_end;			// Once this goes TRUE, the player's turn ends.
	int m_wallet;				// The amount of money the player has in their wallet
	int m_bet;					// How much the player is betting this turn
	bool m_controls_screen;		// If TRUE, switch_players will switch *from* this player to the other player
};
