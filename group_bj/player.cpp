#include "player.h"
#include <exception>

player::player() {	// ctor
	m_wallet = 100;
	m_total = 0;
	// These are all false by default
	m_can_flip_ace = false;
	m_can_hit = false;
}

/////////////// GETTERS ///////////////

int player::get_total() {
	int total = 0;
	for (int i = 0; i < m_hand.size(); i++) {
		if (m_hand[i].is_hidden() == false)
			total = total + m_hand[i].get_value();
	}
	m_total = total;
	return m_total;
}

std::vector<char> player::get_player_aces() {
	std::vector<char> aces;
	for (int i = 0; i < m_hand.size(); i++) {
		if (m_hand[i].is_ace())
			aces.push_back(m_hand[i].get_suit());
	}
	if (aces.size() == 0)
		aces.push_back('n');
	return aces;
}

/////////////// SETTERS ///////////////

void player::set_name(int player_num) {
	using std::cout;
	using std::endl;
	cout << "Player #" << player_num << ": What is your name?" << endl;
	std::string name;
	getline(std::cin, name);
	cout << "You entered " << name << ". Is this correct? y/n" << endl;
	std::string response;
	getline(std::cin, response);
	if (response[0] == 'y' || response[0] == 'Y')
		m_name = name;
	else set_name(player_num);
}

/////////////// DOERS ///////////////

void player::place_bet() {
	using std::cout;
	using std::endl;
	using std::string;
	using std::cin;

	int bet = 0;
	string str_bet;
	while (bet < 10) {
		cout << m_name << ": How much for your starting bet? Minimum of $10" << endl;
		getline(cin, str_bet);
		std::string::size_type sz;
		// If the player doesn't enter a number, an exception will occur, and needs to be handled.
		try { 
			bet = std::stoi(str_bet, &sz); }
		catch (std::exception &e) {
			cout << str_bet << " is not a number!" << endl;
			continue;
		}
		if (bet < 10)
			cout << "Sorry, minimum bet is $10. Try again." << endl;
		else {
			cout << "You want to bet $" << bet << ". Correct? y/n" << endl;
			string response;
			getline(cin, response);
			if (response[0] != 'y' && response[0] != 'Y')
				bet = 0;
		}
	}
	m_bet = bet;
	m_wallet = m_wallet - bet;
}

void player::display_hand(char me_or_them) {
	using std::cout;
	using std::endl;
	for (int i = 0; i < m_hand.size(); i++) {
		int card_num = i + 1;
		cout << "Card " << card_num << ": ";
		if (i == 0 && me_or_them == 't') {
				if (m_hand[i].is_hidden() == false)
					m_hand[i].flip_hidden();
				cout << "[Hidden]" << endl;
				continue;
		}
		else if (m_hand[i].is_ace()) {
			cout << "A||" << m_hand[i].get_suit();
			cout << m_hand[i].get_value();
			if (m_hand[i].get_value() == 1)
				cout << " (LOW)" << endl;
			else if (m_hand[i].get_value() == 11)
				cout << " (HIGH)" << endl;
			continue;
		}
		else {
			if (m_hand[i].get_value() == 10) {
				if (m_hand[i].get_face() == 0) {
					cout << m_hand[i].get_value() << "|";
				}
				else {
					cout << m_hand[i].get_face() << "|";
				}
			}
			else {
				cout << m_hand[i].get_value();
				if (m_hand[i].get_value() != 10) {
					cout << "|"; }
			}
		}
		cout << "|" << m_hand[i].get_suit() << endl;
	}
}

// TODO: FIX FLIP_ACE()
// If more than one ace in hand, nothing changes when player selects [F]
void player::flip_ace() {
	using std::cout;
	using std::endl;
	std::vector<char> aces;
	aces = get_player_aces();
	if (aces.size() > 1) {
		cout << "which Ace do you want to flip?" << endl;
		for (int x = 0; x < aces.size(); x++) {
			for (int y = 0; y < m_hand.size(); y++) {
				if (m_hand[y].is_ace()) {
					if (aces[x] == m_hand[y].get_suit())
						cout << "Ace [" << aces[x] << "] : value = " << m_hand[y].get_value() << endl;
				}
			}
		}
		std::string response;
		getline(std::cin, response);
		char choice = response[0];
		putchar(toupper(choice));	// I think a difference in letter case is what was causing it to break. Need to test this.
		for (int i = 0; i < m_hand.size(); i++) {
			if (m_hand[i].is_ace()) {
				if (choice == m_hand[i].get_suit()) {
					m_hand[i].flip_ace();
					return;
				}
			}
		}
	}
	else {
		for (int i = 0; i < m_hand.size(); i++) {
			if (m_hand[i].is_ace())
				m_hand[i].flip_ace();
		}
	}
}
