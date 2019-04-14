#include "game.h"

game::~game(){}

void game::welcome_splash() {
	using std::cout;
	using std::cin;
	using std::endl;
	using std::string;

	system("CLS");
	cout << "########################################" << endl;
	cout << "      Welcome to ANSI Blackjack!        " << endl;
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
		shuffle_deck();
		deal_card(p1);
		deal_card(dealer);
		deal_card(p1);
		deal_card(dealer);
		game_loop();
	}
	else welcome_splash();
}

void game::shuffle_deck() {
	deck shuffled;
	deck sorted = m_deck;

}

void game::deal_card(player p) {}

void game::game_loop() {
	
}
