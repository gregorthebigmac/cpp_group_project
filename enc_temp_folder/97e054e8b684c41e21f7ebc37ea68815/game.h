#pragma once
#include "player.h"

class game {
public:
	game();
	~game();

	void welcome_splash();
	void init_round();
	void switch_players();
	void deal_card(player &p);
	void game_loop(player &first, player &second);
	void player_turn(player &me, player &them);
	void draw_player_hud(player &me, player &them);
	void draw_actions(player &me, player &them);
	void player_action(player &me, player &them);
	bool is_round_over();
	bool is_game_over();
	bool confirm(std::string question);
	void reveal_cards();
	void new_round();
	
	//void redraw();

private:
	player p1, p2;
	deck m_deck;
	bool m_game_over;
	bool m_round_over;
};
