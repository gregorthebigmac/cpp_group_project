#include "game.h"

game::game() {
	m_game_over = false;
	dealer.set_is_dealer();
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
	cout << "What is your name?" << endl;
	string player_name;
	getline(cin, player_name);
	cout << "You entered " << player_name << ". Is this correct? y/n" << endl;
	string response;
	getline(cin, response);
	if (response[0] == 'Y' || response[0] == 'y') {
		p1.set_name(player_name);
		dealer.set_name("Dealer");
		cout << "Welcome, " << p1.get_name() << "!" << endl;
		m_deck.shuffle_deck();
		deal_card(p1);
		deal_card(dealer);
		deal_card(p1);
		deal_card(dealer);
		game_loop();
	}
	else welcome_splash();
}

void game::deal_card(player &p) {
	card _card = m_deck.draw_card();
	p.draw_card(_card);
}

void game::game_loop() {
	while (m_game_over == false) {
		while (p1.is_staying() == false) {
			redraw();
			if (is_game_over()) {
				m_game_over = true;
				break;
			}
			draw_actions();
			player_action();
		}
		if (m_game_over)
			break;
		while (dealer.is_staying() == false) {
			if (dealer.will_dealer_hit())
				deal_card(dealer);
			else dealer.stay();
			redraw();
			if (is_game_over()) {
				m_game_over = true;
				break;
			}
		}
	}
}

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
	cout << dealer.get_name() << "'s hand:" << endl;
	dealer.display_hand();
	cout << endl;
	cout << "Dealer is showing: " << dealer.get_total() << endl;
	cout << endl << endl;
}

void game::draw_actions() {
	using std::cout;
	using std::endl;
	p1.set_can_flip_ace(false);
	p1.set_can_hit(false);
	cout << "Press the key in [ ] + [ENTER] to perform the indicated action." << endl << endl;
	cout << "[Q] Quit   [S] Stay   ";
	if (p1.get_total() < 21)
		p1.set_can_hit(true);
	else {
		p1.stay();
		return;
	}
	std::vector<char> aces = p1.get_player_aces();
	if (aces[0] != 'n')
		p1.set_can_flip_ace(true);
	if (p1.can_hit())
		cout << "[H] Hit   ";
	if (p1.can_flip_ace())
		cout << "[F] Flip Ace";
	cout << endl << endl;
}

void game::player_action() {
	char action;
	std::string response;
	getline(std::cin, response);
	action = response[0];
	if (action == 's' || action == 'S')
		p1.stay();
	else if (action == 'q' || action == 'Q') {
		std::cout << "Are you sure you want to end the game? y/n" << std::endl;
		getline(std::cin, response);
		action = response[0];
		if (action == 'y' || action == 'Y') {
			m_game_over = true;
			return;
		}
	}
	else if (action == 'h' || action == 'H') {
		if (p1.can_hit()) {
			deal_card(p1);
			return;
		}
	}
	else if (action == 'f' || action == 'F') {
		if (p1.can_flip_ace()) {
			p1.flip_ace();
			return;
		}
	}
	else {
		std::cout << "[" << action << "] Is not a valid option! Try again!" << std::endl;
		return;
	}
}

// TODO: COMPLETE is_game_over() CONDITIONS!
//			There are still some incorrect scenarios playing out. Track 'em down!
bool game::is_game_over() {
	if (p1.get_total() == 21) {
		if (dealer.get_total() == 21) {
			std::cout << "Both got exactly 21! Game is a draw!" << std::endl;
			m_game_over = true;
			return true;
		}
		else if (dealer.get_total() < 21) {
			return false;
		}
	}
	else if (dealer.get_total() == 21) {
		if (p1.get_total() > 21) {
			if (p1.can_flip_ace()) {
				bool score_can_go_down = false;
				std::deque<card> temp_hand = p1.get_hand();
				for (int i = 0; i < temp_hand.size(); i++) {
					if (temp_hand[i].is_ace()) {
						if (temp_hand[i].get_value() == 11) {
							score_can_go_down = true;
							break;
						}
					}
				}
				return score_can_go_down;
			}
		}
		std::cout << "Dealer has 21! House wins!" << std::endl;
		return true;
	}
	else if (p1.get_total() > 21) {
		if (dealer.get_total() > 21)
			std::cout << "Both busted! Game is a draw!" << std::endl;
		else {
			std::cout << "Bust! You lose!" << std::endl;
			system("PAUSE");
			m_game_over = true;
			return true;
		}
	}
	else if (dealer.get_total() > 21) {
			std::cout << "Dealer Busts! You Win!" << std::endl;
			system("PAUSE");
			m_game_over = true;
			return true;
	}
	else if (p1.get_total() < 21) {
		if (dealer.get_total() < 21) {
			return false;
		}
	}
}