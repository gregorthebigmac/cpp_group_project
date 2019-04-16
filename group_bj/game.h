#pragma once
#include "player.h"

class game {
public:
	game();
	~game();

	void welcome_splash();
	void deal_card(player &p);
	void game_loop();
	void redraw();
	void draw_actions();
	void player_action();
	bool is_game_over();

private:
	player p1, dealer;
	deck m_deck;
	bool m_game_over;
};

