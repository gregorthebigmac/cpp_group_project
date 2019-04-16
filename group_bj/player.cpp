#include "player.h"

player::player() {
	// These are all false by default
	m_is_dealer = false;
	m_can_flip_ace = false;
	m_can_hit = false;
	m_total = 0;
}

void player::draw_card(card _card) {
	m_hand.push_back(_card);
	// If this is the dealer getting their first card, that card starts hidden.
	if (m_is_dealer) {
		if (m_hand.size() == 1)
			m_hand[0].flip_hidden();
	}
}

void player::display_hand() {
	using std::cout;
	using std::endl;
	for (int i = 0; i < m_hand.size(); i++) {
		cout << "Card " << i + 1 << ": ";
		if (m_hand[i].is_hidden()) {
				cout << "[Face Down]" << endl;
				continue;
		}
		if (m_hand[i].is_ace()) {
			cout << "A||" << m_hand[i].get_suit();
			if (m_hand[i].get_value() == 1)
				cout << m_hand[i].get_value() << " (LOW)" << endl;
			else if (m_hand[i].get_value() == 11)
				cout << m_hand[i].get_value() << " (HIGH)" << endl;
			continue;
		}
		else {
			if (m_hand[i].get_value() == 11)
				cout << "J|";
			else if (m_hand[i].get_value() == 12)
				cout << "Q|";
			else if (m_hand[i].get_value() == 13)
				cout << "K|";
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
		if (m_hand[i].is_hidden() == false)
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
// UPDATE: I think it's fixed, but more testing is needed.
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

// TODO: FINISH will_dealer_hit()
bool player::will_dealer_hit() {
	if (m_total < 18) {
		std::cout << "Dealer hits!" << std::endl;
		system("PAUSE");
		return true;
	}
	else {
		std::cout << "Dealer stays!" << std::endl;
		system("PAUSE");
		return false;
	}
}