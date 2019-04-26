#include "game.h"

game::game() {
	m_game_over = false;
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
		new_round();
		init_round();
	}
}

void game::player_turn(player &me, player &them) {
	draw_player_hud(me, them);
	draw_actions(me);
	player_action(me);
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

	cout << them.get_name() << "'s hand:" << endl;
	them.display_hand('t');
	cout << endl;
	cout << them.get_name() << " is showing " << them.get_total() << endl;

	cout << endl << "--------------------------------------------------------------------------------" << endl << endl;

	cout << me.get_name() << "'s hand:" << endl;
	me.display_hand('m');
	cout << endl;
	cout << "Total: " << me.get_total() << endl;
}

void game::draw_actions(player &p) {
	using std::cout;
	using std::endl;
	p.begin_turn();
	p.set_can_flip_ace(false);
	p.set_can_hit(false);
	cout << "Press the key in [ ] + [ENTER] to perform the indicated action." << endl << endl;
	cout << "[Q] Quit   [S] Stay   [E] End Turn   ";
	if (p.get_total() < 21)
		p.set_can_hit(true);
	if (p.get_money() > 0)
		cout << "[R] Raise Bet   ";
	std::vector<char> aces = p.get_player_aces();
	if (aces[0] != 'n')
		p.set_can_flip_ace(true);
	if (p.can_hit())
		cout << "[H] Hit   ";
	if (p.can_flip_ace())
		cout << "[F] Flip Ace";
	cout << endl << endl;
}

void game::player_action(player &p) {
	char action;
	std::string response;
	getline(std::cin, response);
	action = response[0];
	if (action == 's' || action == 'S') {
		p.stay();
		p.end_turn();
	}
	else if (action == 'q' || action == 'Q') {
		std::cout << "Are you sure you want to end the game? y/n" << std::endl;
		getline(std::cin, response);
		action = response[0];
		if (action == 'y' || action == 'Y') {
			p.end_turn();
			m_round_over = true;
			m_game_over = true;
			return;
		}
	}
	else if (action == 'h' || action == 'H') {
		if (p.has_hit_this_turn())
			return;
		else {
			if (p.can_hit()) {
				deal_card(p);
				p.set_hit_this_turn(true);
				return;
			}
		}
	}
	else if (action == 'f' || action == 'F') {
		if (p.can_flip_ace()) {
			p.flip_ace();
			return;
		}
	}
	else if (action == 'e' || action == 'E') {
		if (confirm ("End turn? y/n"))
			p.end_turn();
		else return;
	}
	else if (action == 'r' || action == 'R') {
		if (p.get_money() > 0) {
			std::cout << "Raise bet by how much?" << std::endl;
			std::string response;
			getline(std::cin, response);
			std::string::size_type sz;
			int raise = std::stoi(response, &sz);
			int total_bet = p.get_bet() + raise;
			std::string question = "Raise bet to $";
			std::string str_bet = std::to_string(total_bet);
			question = question + str_bet;
			question = question + "?";
			getline(std::cin, response);
			if (response[0] == 'y' || response[0] == 'Y')
				p.increase_bet(raise);
			else return;
		}
	}
	else {
		std::cout << "[" << action << "] Is not a valid option! Try again!" << std::endl;
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
	if (p1.get_money() < 1 && p2.get_money() < 1) {
		cout << "I don't know how you *both* managed to have no money, but somehow you broke me." << endl;
		cout << "Congratulations to the both of you!" << endl;
		m_game_over = true;
	}
	else if (p1.get_money() < 1 && p2.get_money() > 1) {
		cout << p2.get_name() << " wins the game!" << endl;
		m_game_over = true;
	}
	else if (p1.get_money() > 1 && p2.get_money() < 1) {
		cout << p1.get_name() << " wins the game!" << endl;
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

/*
void game::redraw() {
	system("CLS");
	using std::cout;
	using std::endl;
	
	cout << "############################# KEY ###########################" << endl;
	cout << "#  A = Ace   | K = King   | Q = Queen    | J = Jack         #" << endl;
	cout << "#  C = Clubs | H = Hearts | D = Diamonds | S = Spades       #" << endl;
	cout << "#############################################################" << endl;
	cout << endl;
	cout << p1.get_name() << "'s hand: " << endl;
	p1.display_hand();
	cout << endl;
	cout << "Total: " << p1.get_total() << endl;
	cout << endl << endl;
	cout << endl << endl;
	cout << p2.get_name() << "'s hand:" << endl;
	p2.display_hand();
	cout << endl;
	cout << "p2 is showing: " << p2.get_total() << endl;
	cout << endl << endl;
}
*/
