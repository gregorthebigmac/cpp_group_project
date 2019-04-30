#include "player.h"

player::player() {	// ctor
	m_bet = 0;
	m_wallet = 100;
	m_total = 0;
	// These are all false by default
	m_turn_end = false;
	m_stay = false;
	m_controls_screen = false;
	m_hit_this_turn = false;
	m_can_flip_ace = false;
	m_can_hit = false;
	m_can_stay = true;
}

/////////////// GETTERS ///////////////

int player::display_total(char me_or_them) {
	int total = 0;
	for (int i = 0; i < m_hand.size(); i++) {
		if (me_or_them == 't' && i == 0) {
			continue; }
		else {
			total = total + m_hand[i].get_value(); }
	}
	m_total = total;
	return m_total;
}

bool player::can_hit() {
	if (m_hit_this_turn) {
		return false; }
	else return m_can_hit;
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

	bool bet_is_valid = false;
	int bet = 0;
	string str_bet;
	if (m_wallet == 0) {
		cout << "Somehow you broke me. The game is over!" << endl;
		return;
	}
	while (bet_is_valid == false) {
		system("CLS");
		cout << m_name << ": How much for your starting bet? Minimum of $10," << endl;
		cout << "and your current funds are $" << m_wallet << "." << endl;
		cout << "If your current funds are less than $10 you must go all in." << endl;
		getline(cin, str_bet);
		std::string::size_type sz;
		
		// If the player doesn't enter a number, an exception will occur. This try-catch block handles it.
		try { 
			bet = std::stoi(str_bet, &sz); }
		catch (std::exception &e) {
			cout << "\"" << str_bet << "\" is not a number!" << endl;
			system("PAUSE");
			continue;
		}

		if (bet < 10) {
			if (m_wallet < 10) {
				if (bet < m_wallet) {
					cout << "You have less than $10 in your wallet, therefore you must go all in." << endl; }
				else if (bet > m_wallet) {
					cout << "You cannot bet more money than you have!" << endl; }
				else {
					string question = "You want to bet $" + str_bet + ". Correct? y/n";
					if (confirm(question)) {
						bet_is_valid = true; }
				}
			}
			else {
				cout << "Sorry, minimum bet is $10. Try again." << endl; }
		}
		else if (bet > m_wallet) {
			cout << "You cannot bet more money than you have!" << endl;
			system("PAUSE");
			bet = 0;
			continue;
		}
		else {
			string question = "You want to bet $" + str_bet + ". Correct? y/n";
			if (confirm(question)) {
				bet_is_valid = true; }
			else bet = 0;
		}
	}
	m_bet = bet;
	m_wallet = m_wallet - bet;
}

void player::calc_total() {
	int total = 0;
	for (int i = 0; i < m_hand.size(); i++) {
		total = total + m_hand[i].get_value();
	}
	m_total = total;
}

void player::display_hand(char me_or_them) {
	using std::cout;
	using std::endl;
	for (int i = 0; i < m_hand.size(); i++) {
		int card_num = i + 1;
		cout << "Card " << card_num << ": ";
		if (i == 0 && me_or_them == 't') {
			cout << "[Hidden]" << endl;
			continue;
		}
		if (m_hand[i].is_ace()) {
			cout << "A||" << m_hand[i].get_suit();
			if (m_hand[i].get_value() == 1)
				cout << "  (LOW)" << endl;
			else if (m_hand[i].get_value() == 11)
				cout << "  (HIGH)" << endl;
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

void player::match_bet(int difference) {
	using std::cout;
	using std::endl;
	using std::string;
	using std::cin;

	if (difference > m_wallet) {
		if (confirm("You don't have enough money to match their current bet. Go all in? y/n")) {
			m_bet = m_wallet + m_bet;
			m_wallet = 0;
		}
	}
	else {
		m_bet = m_bet + difference;
		m_wallet = m_wallet - difference;
	}
}

void player::raise_bet() {
	using std::cout;
	using std::endl;
	using std::string;
	using std::cin;
	bool should_raise_bet = false;
	int raise = 0;
	int total_bet = 0;
	if (m_wallet > 0) {
		cout << "Raise bet by how much?" << endl;
		string response;
		getline(cin, response);
		string::size_type sz;
		try {
			raise = std::stoi(response, &sz);
			should_raise_bet = true;
		}
		catch (std::exception & e) {
			cout << "\"" << response << "\" is not a number!" << endl;
		}

		if (should_raise_bet) {
			if (raise > m_wallet) {
				cout << "Insufficient Funds!" << endl;
				return;
			}
			total_bet = m_bet + raise;
			string question = "Raise bet to $";
			string str_bet = std::to_string(total_bet);
			question = question + str_bet;
			question = question + "?";
			if (confirm(question)) {
				increase_bet(raise); }
			else return;
		}
		else return;
	}
	else {
		cout << "Insufficient Funds!" << endl;
		return;
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

void player::draw_card(card _card) {
	m_hand.push_back(_card);
	calc_total();
}

bool player::confirm(std::string question) {
	std::cout << question << std::endl;
	std::string response;
	getline(std::cin, response);
	if (response[0] == 'y' || response[0] == 'Y')
		return true;
	else return false;
}