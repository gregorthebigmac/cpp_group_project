#include "game.h"

game::game() {
	m_game_over = false;
	m_round_over = false;
}
game::~game(){}

void game::welcome_splash() {
	using std::cout;
	using std::cin;
	using std::endl;
	using std::string;

	system("CLS");
	cout << "########################################" << endl;
	cout << "#                                      #" << endl;
	cout << "#     Welcome to ANSI Blackjack!       #" << endl;
	cout << "#                                      #" << endl;
	cout << "########################################" << endl;
	cout << endl;

	p1.set_name(1);
	p2.set_name(2);
	m_deck.shuffle_deck();
}

void game::init_round() {
	system("CLS");
	p1.toggle_screen_control();
	p1.place_bet();
	switch_players();
	p2.place_bet();
	deal_card(p1);
	deal_card(p2);
	deal_card(p1);
	deal_card(p2);

	if (p1.get_bet() > p2.get_bet())
		game_loop(p1, p2);
	else if (p1.get_bet() < p2.get_bet())
		game_loop(p2, p1);
	else {
		std::cout << "Both player's bets are the same! Now choosing one at random...";
		srand(time(NULL));
		int who_goes_first = rand() % 2;
		if (who_goes_first == 0) {
			std::cout << p1.get_name() << " was chosen! You go first!" << std::endl;
			game_loop(p1, p2);
		}
		else {
			std::cout << p2.get_name() << " was chosen! You go first!" << std::endl;
			game_loop(p2, p1);
		}
	}
}

void game::switch_players() {
	system("CLS");
	if (p1.has_screen_control()) {
		std::cout << "Okay, " << p1.get_name() << ". Please hand the laptop to " << p2.get_name() << "," << std::endl;
		std::cout << "and " << p2.get_name() << ", press[ENTER] when you ready." << std::endl;
	}
	else {
		std::cout << "Okay, " << p2.get_name() << ". Please hand the laptop to " << p1.get_name() << "," << std::endl;
		std::cout << "and " << p1.get_name() << ", press[ENTER] when you ready." << std::endl;
	}
	p1.toggle_screen_control();
	p2.toggle_screen_control();
	system("PAUSE");
	system("CLS");
}

void game::deal_card(player &p) {
	card _card = m_deck.draw_card();
	p.draw_card(_card);
}

void game::game_loop(player &first, player &second) {
	while (m_game_over == false) {
		while (m_round_over == false) {
			if (first.is_staying() == false)
				if (second.has_screen_control())
					switch_players();
				first.begin_turn();
			while (first.is_turn_over() == false) {
				player_turn(first, second);
				if (m_round_over)
					break;
			}
			if (m_game_over) {
				break; }
			if (second.is_staying() == false)
				if (first.has_screen_control())
					switch_players();
				second.begin_turn();
			while (second.is_turn_over() == false) {
				player_turn(second, first);
				if (m_round_over)
					break;
			}
			if (first.is_staying() && second.is_staying())
				is_round_over();
			if (first.has_screen_control() == second.has_screen_control())
				first.toggle_screen_control();
			first.set_hit_this_turn(false);
			second.set_hit_this_turn(false);
			is_round_over();
		}
		if (m_game_over) {
			break; }
		else {
			if (m_round_over) {
				new_round();
				init_round();
			}
		}
	}
}

void game::player_turn(player &me, player &them) {
	draw_player_hud(me, them);
	draw_actions(me, them);
	player_action(me, them);
}

void game::draw_player_hud(player &me, player &them) {
	system("CLS");
	using std::cout;
	using std::endl;

	cout << "############################# KEY ###########################" << endl;
	cout << "#  A = Ace   | K = King   | Q = Queen    | J = Jack         #" << endl;
	cout << "#  C = Clubs | H = Hearts | D = Diamonds | S = Spades       #" << endl;
	cout << "#############################################################" << endl;
	cout << endl;
	
	cout << endl << "--------------------------------------------------------------------------------" << endl << endl;

	cout << them.get_name() << "'s hand:" << endl << endl;
	them.display_hand('t');
	cout << endl;
	cout << them.get_name() << " is showing " << them.get_total() << endl << endl;
	cout << them.get_name() << " current bet:   $" << them.get_bet() << endl;
	cout << them.get_name() << " current funds: $" << them.get_money() << endl << endl;

	cout << endl << "--------------------------------------------------------------------------------" << endl << endl;

	cout << me.get_name() << "'s hand:" << endl;
	me.display_hand('m');
	cout << endl;
	cout << "Total: " << me.get_total() << endl << endl;
	cout << "Current Bet:   $" << me.get_bet() << endl;
	cout << "Current Funds: $" << me.get_money() << endl;
}

void game::draw_actions(player &me, player &them) {
	using std::cout;
	using std::endl;
	me.begin_turn();
	me.set_can_flip_ace(false);
	me.set_can_hit(false);
	cout << "Press the key in [ ] + [ENTER] to perform the indicated action." << endl << endl;
	cout << "[Q] Quit   [S] Stay   [E] End Turn   ";
	if (me.get_total() < 21)
		me.set_can_hit(true);
	if (me.get_money() > 0) {
		if (me.get_bet() < them.get_bet()) {
			cout << "[M] Match Bet   "; }
		else { cout << "[R] Raise Bet   "; }
	}
	std::vector<char> aces = me.get_player_aces();
	if (aces[0] != 'n')
		me.set_can_flip_ace(true);
	if (me.can_hit())
		cout << "[H] Hit   ";
	if (me.can_flip_ace())
		cout << "[F] Flip Ace";
	cout << endl << endl;
}

void game::player_action(player &me, player &them) {
	using std::cout;
	using std::endl;
	using std::string;
	using std::cin;
	char action;
	string response;
	getline(cin, response);
	action = response[0];
	if (action == 's' || action == 'S') {
		me.stay();
		me.end_turn();
	}
	else if (action == 'q' || action == 'Q') {
		cout << "Are you sure you want to end the game? y/n" << endl;
		getline(cin, response);
		action = response[0];
		if (action == 'y' || action == 'Y') {
			me.end_turn();
			m_round_over = true;
			m_game_over = true;
			return;
		}
	}
	else if (action == 'h' || action == 'H') {
		if (me.has_hit_this_turn())
			return;
		else {
			if (me.can_hit()) {
				deal_card(me);
				me.set_hit_this_turn(true);
				return;
			}
		}
	}
	else if (action == 'f' || action == 'F') {
		if (me.can_flip_ace()) {
			me.flip_ace();
			return;
		}
	}
	else if (action == 'e' || action == 'E') {
		if (confirm ("End turn? y/n"))
			me.end_turn();
		else return;
	}
	else if (action == 'r' || action == 'R') {
		me.raise_bet();
	}
	else if (action == 'm' || action == 'M') {
		int diff = them.get_bet() - me.get_bet();
		if (diff < 1) {
			cout << "I don't know how you managed to do this, but you broke me." << endl; }
		else { me.match_bet(diff); }
	}
	else {
		cout << "[" << action << "] Is not a valid option! Try again!" << endl;
		return;
	}
}

bool game::is_round_over() {
	using std::cout;
	using std::endl;
	if (p1.get_total() == 21 && p2.get_total() == 21) {
		cout << "It's a draw! Both players get their money back!" << endl;
		p1.round_is_a_draw();
		p2.round_is_a_draw();
		return true;
	}
	else if (p1.get_total() > 21 && p2.get_total() < 22) {
		cout << p2.get_name() << " wins!" << endl;
		p2.win_bet(p1.get_bet());
		p1.reset_bet();
		m_round_over = true;
		return m_round_over;
	}
	else if (p1.get_total() < 22 && p2.get_total() > 21) {
		cout << p1.get_name() << " wins!" << endl;
		p1.win_bet(p2.get_bet());
		p2.reset_bet();
		m_round_over = true;
		return m_round_over;
	}
	else if (p1.get_total() < 21 && p2.get_total() < 21) {
		if (p1.get_total() > p2.get_total()) {
			cout << p1.get_name() << " wins!" << endl;
			p1.win_bet(p2.get_bet());
			p2.reset_bet();
			m_round_over = true;
			return m_round_over;
		}
		else if (p1.get_total() < p2.get_total()) {
			cout << p2.get_name() << " wins!" << endl;
			p2.win_bet(p1.get_bet());
			p2.reset_bet();
			p1.reset_bet();
			m_round_over = true;
			return m_round_over;
		}
		else if (p1.get_total() == p2.get_total()) {
			cout << "It's a draw! Both players get their money back!" << endl;
			p1.win_bet(0);
			p2.win_bet(0);
			m_round_over = true;
			return m_round_over;
		}
		else {
			cout << "I don't know how you got here, but something went wrong!" << endl;
			m_round_over = true;
			return m_round_over;
		}
	}
	else {
		cout << "I don't know how you got here, but something went wrong!" << endl;
		m_round_over = true;
		return m_round_over;
	}
}

void game::new_round() {
	deck temp_deck;
	temp_deck.shuffle_deck();
	m_deck = temp_deck;
	p1.reset_hand();
	p2.reset_hand();
}

bool game::is_game_over() {
	using std::cout;
	using std::endl;
	if (p1.get_money() < 1) {
		cout << p2.get_name() << " wins!" << endl;
		m_game_over = true;
	}
	else if (p2.get_money() < 1) {
		cout << p1.get_money() << " wins!" << endl;
		m_game_over = true;
	}
	else if (p1.get_money() < 1 && p2.get_money() < 1) {
		cout << "I don't know how you *both* managed to have no money, but somehow you broke me." << endl;
		cout << "Congratulations to the both of you!" << endl;
		m_game_over = true;
	}
	else { m_game_over = false; }
	return m_game_over;
}

bool game::confirm(std::string question) {
	using std::cout;
	using std::endl;
	using std::cin;
	using std::string;

	cout << question << endl;
	string response;
	getline(cin, response);
	if (response[0] == 'y' || response[0] == 'Y')
		return true;
	else return false;
}
