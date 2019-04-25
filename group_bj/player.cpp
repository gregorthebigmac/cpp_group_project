#include "player.h"

player::player() {
	// These are all false by default
	m_can_flip_ace = false;
	m_can_hit = false;
	m_total = 0;
}

void player::display_hand() {
	using std::cout;
	using std::endl;
	for (int i = 0; i < m_hand.size(); i++) {
		int card_num = i + 1;
		cout << "Card " << card_num << ": ";
		if (m_hand[i].is_ace()) {
			cout << "A||" << m_hand[i].get_suit();
			cout << m_hand[i].get_value();
			if (m_hand[i].get_value() == 1)
				cout << " (LOW)" << endl;
			else if (m_hand[i].get_value() == 11)
				cout << " (HIGH)" << endl;
			continue;
		}
		else {
			if (m_hand[i].get_value() == 11)
				cout << "J||";
			else if (m_hand[i].get_value() == 12)
				cout << "Q||";
			else if (m_hand[i].get_value() == 13)
				cout << "K||";
			else {
				cout << m_hand[i].get_value();
				if (m_hand[i].get_value() != 10)
					cout << "|";
			}
		}
		cout << "|" << m_hand[i].get_suit() << endl;
	}
}

int player::get_total() {
	int total = 0;
	for (int i = 0; i < m_hand.size(); i++) {
		total = total + m_hand[i].get_value();
	}
	m_total = total;
	return m_total;
}

std::vector<char> player::get_player_aces() {
	std::vector<char> aces;
	for (int i = 0; i < m_hand.size(); i++) {
		if (m_hand[i].is_ace()) {
			aces.push_back(m_hand[i].get_suit());
		}
	}
	if (aces.size() == 0)
		aces.push_back('n');
	return aces;
}

// TODO: FIX FLIP_ACE()
// If more than one ace in hand, nothing changes when player selects [F]
// UPDATE: The code looks solid, but it's still not working. Breakpoints have been set.
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
		putchar(toupper(choice));	// I think this is what was causing it to break. Need to test this.
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
