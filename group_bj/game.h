#pragma once
#include "player.h"

class game {
public:
	game() { m_game_over = false; }
	~game();

	void welcome_splash();
	void shuffle_deck();
	void deal_card(player p);
	void game_loop();

private:
	player p1, dealer;
	deck m_deck;
	bool m_game_over;
};

