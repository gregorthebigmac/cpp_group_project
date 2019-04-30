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
	p1.calc_total();
	p2.calc_total();

	if (p1.get_bet() > p2.get_bet()) {
		game_loop(p1, p2); }
	else if (p1.get_bet() < p2.get_bet()) {
		game_loop(p2, p1); }
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
	p.calc_total();
}

void game::game_loop(player &first, player &second) {
	while (m_game_over == false) {
		while (m_round_over == false) {
			if (second.has_screen_control())
				switch_players();
			if (first.is_turn_over() == false) {
				player_turn(first, second);
			}
			m_round_over = is_round_over();
			if (m_game_over || m_round_over) {
				break;
			}
			if (first.has_screen_control()) {
				switch_players(); }
			if (second.is_turn_over() == false) {
				player_turn(second, first);
			}
			m_round_over = is_round_over();
			if (m_game_over || m_round_over) {
				break;
			}
			if (first.is_turn_over() && second.is_turn_over()) {
				first.begin_turn();
				second.begin_turn();
				first.set_hit_this_turn(false);
				second.set_hit_this_turn(false);
			}
		}
		m_game_over = is_game_over();
		if (m_game_over) {
			break; }
		else {
			if (m_round_over) {
				std::cout << "End of the round. Starting new round." << std::endl;
				system("PAUSE");
				p1.round_reset();
				p2.round_reset();
				new_round();
				init_round();
			}
		}
	}
}

void game::player_turn(player &me, player &them) {
	while (me.is_turn_over() == false) {
		draw_player_hud(me, them);
		draw_actions(me, them);
		if (me.is_turn_over())
			break;
		player_action(me, them);
		if (me.is_staying())
			me.end_turn();
	}
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
	cout << them.get_name() << " is showing " << them.display_total('t') << endl << endl;
	cout << them.get_name() << " current bet:   $" << them.get_bet() << endl;
	cout << them.get_name() << " current funds: $" << them.get_money() << endl << endl;

	cout << endl << "--------------------------------------------------------------------------------" << endl << endl;

	cout << me.get_name() << "'s hand:" << endl << endl;
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
	if (me.get_bet() < them.get_bet()) {
		if (me.get_money() > 0) {
			cout << "You must match " << them.get_name() << "'s bet before proceeding." << endl;
			std::string name = them.get_name();
			std::string match_bet = "Match " + name + "'s bet? y/n";
			if (confirm(match_bet)) {
				int diff = them.get_bet() - me.get_bet();
				me.match_bet(diff);
				draw_player_hud(me, them);
				draw_actions(me, them);
			}
			else m_round_over = is_round_over();
		}
	}
	else {
		if (me.get_total() > 21) {
			cout << "Total is " << me.get_total() << ". Busted!" << endl;
			me.end_turn();
			system("PAUSE");
			return;
		}
		cout << "[Q] Quit   ";
		if (me.get_total() < 21) {
			me.set_can_hit(true);
			cout << "[S] Stay   ";
		}
		if (me.get_money() > 0) {
			if (me.get_bet() < them.get_bet()) {
				cout << "[M] Match Bet   ";
			}
			else { cout << "[R] Raise Bet   "; }
		}
		std::vector<char> aces = me.get_player_aces();
		if (aces[0] != 'n')
			me.set_can_flip_ace(true);
		if (me.can_hit())
			cout << "[H] Hit   ";
		if (me.can_flip_ace())
			cout << "[F] Flip Ace   ";
		cout << "[E] End Turn   " << endl << endl;
	}
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
		if (me.can_stay()) {
			me.stay();
			me.end_turn();
		}
		else { cout << "That wasn't an option!" << endl; }
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
		if (me.has_hit_this_turn()) {
			cout << "You already hit this turn! Either Stay or End turn!" << endl;
			system("PAUSE");
			return;
		}
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
	if (p1.get_total() > 21 && p2.get_total() > 21) {
		reveal_cards();
		cout << "Both players bust! Both players get their money back!" << endl;
		system("PAUSE");
		p1.round_is_a_draw();
		p2.round_is_a_draw();
		return true;
	}
	// This little else-if is because I couldn't figure out how to do a logical XOR in C++ using std lib
	else if (p1.get_total() == 21 && p2.get_total() == 21) {
		reveal_cards();
		cout << "It's a draw! Both players get their money back!" << endl;
		system("PAUSE");
		p1.round_is_a_draw();
		p2.round_is_a_draw();
		return true;
	}
	if (p1.is_staying() && p2.is_staying()) {
		if (p1.get_total() == 21 || p2.get_total() == 21) {
			if (p2.get_total() != 21) {
				reveal_cards();
				cout << p1.get_name() << " wins!" << endl;
				system("PAUSE");
				p1.win_bet(p2.get_bet());
				p2.reset_bet();
				return true;
			}
			else if (p1.get_total() != 21) {
				reveal_cards();
				cout << p2.get_name() << " wins!" << endl;
				system("PAUSE");
				p2.win_bet(p1.get_bet());
				p1.reset_bet();
				return true;
			}
		}
		else if (p1.get_total() > p2.get_total()) {
			reveal_cards();
			cout << p1.get_name() << " wins!" << endl;
			system("PAUSE");
			p1.win_bet(p2.get_bet());
			p2.reset_bet();
			return true;
		}
		else if (p2.get_total() < p2.get_total()) {
			reveal_cards();
			cout << p2.get_name() << " wins!" << endl;
			system("PAUSE");
			p2.win_bet(p1.get_bet());
			p1.reset_bet();
			return true;
		}
		else {
			reveal_cards();
			cout << "It's a draw! Both players get their money back!" << endl;
			system("PAUSE");
			p1.round_is_a_draw();
			p2.round_is_a_draw();
			return true;
		}
	}
	else if (p1.is_turn_over() && p2.is_turn_over()) {
		if (p1.get_total() > 21) {
			reveal_cards();
			cout << p1.get_name() << " busts! " << p2.get_name() << " wins!" << endl;
			p2.win_bet(p1.get_bet());
			p1.reset_bet();
			return true;
		}
		else if (p2.get_total() > 21) {
			reveal_cards();
			cout << p2.get_name() << " busts!" << p1.get_name() << " wins!" << endl;
			p1.win_bet(p2.get_bet());
			p2.reset_bet();
			return true;
		}
		else return false;
	}
	else return false;
}

void game::new_round() {
	m_round_over = false;
	deck temp_deck;
	temp_deck.shuffle_deck();
	m_deck = temp_deck;
	p1.reset_hand();
	p2.reset_hand();
}

bool game::is_game_over() {
	bool game_over = false;
	using std::cout;
	using std::endl;
	if (p1.get_money() < 1) {
		if (p1.get_bet() < 1) {
			cout << p2.get_name() << " wins!" << endl;
			system("PAUSE");
			game_over = true;
		}
	}
	else if (p2.get_money() < 1) {
		if (p2.get_bet() < 1) {
			cout << p1.get_money() << " wins!" << endl;
			system("PAUSE");
			game_over = true;
		}
	}
	else if (p1.get_money() < 1 && p2.get_money() < 1) {
		cout << "I don't know how you *both* managed to have no money, but somehow you broke me." << endl;
		cout << "Congratulations to the both of you!" << endl;
		system("PAUSE");
		game_over = true;
	}
	return game_over;
}

bool game::confirm(std::string question) {
	std::cout << question << std::endl;
	std::string response;
	getline(std::cin, response);
	if (response[0] == 'y' || response[0] == 'Y')
		return true;
	else return false;
}

void game::reveal_cards() {
	system("CLS");
	using std::cout;
	using std::endl;

	cout << "############################# KEY ###########################" << endl;
	cout << "#  A = Ace   | K = King   | Q = Queen    | J = Jack         #" << endl;
	cout << "#  C = Clubs | H = Hearts | D = Diamonds | S = Spades       #" << endl;
	cout << "#############################################################" << endl;
	cout << endl;

	cout << endl << "--------------------------------------------------------------------------------" << endl << endl;

	cout << p1.get_name() << "'s hand:" << endl << endl;
	p1.display_hand('m');
	cout << endl;
	cout << p1.get_name() << "'s Total: " << p1.get_total() << endl << endl;
	cout << p1.get_name() << " current bet:   $" << p1.get_bet() << endl;
	cout << p1.get_name() << " current funds: $" << p1.get_money() << endl << endl;

	cout << endl << "--------------------------------------------------------------------------------" << endl << endl;

	cout << p2.get_name() << "'s hand:" << endl << endl;
	p2.display_hand('m');
	cout << endl;
	cout << p2.get_name() << "'s Total: " << p2.get_total() << endl << endl;
	cout << p2.get_name() << " current bet:   $" << p2.get_bet() << endl;
	cout << p2.get_name() << " current funds: $" << p2.get_money() << endl;
}