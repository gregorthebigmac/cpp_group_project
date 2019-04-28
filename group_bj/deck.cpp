#include "deck.h"

deck::deck(){
	for (int x = 0; x < 4; x++) {
		for (int y = 1; y < 14; y++) {
			char suit;
			card temp;
			if (x == 0)
				suit = 'C';
			else if (x == 1)
				suit = 'H';
			else if (x == 2)
				suit = 'D';
			else if (x == 3)
				suit = 'S';
			else {
				std::cout << "deck::deck for loop broke!" << std::endl;
				break;
			}
			temp.set_suit(suit);
			if (y == 1) {
				temp.set_ace();
				temp.set_value(1);
			}
			else if (y == 11) {
				temp.set_value(10);
				temp.set_face('J');
			}
			else if (y == 12) {
				temp.set_value(10);
				temp.set_face('Q');
			}
			else if (y == 13) {
				temp.set_value(10);
				temp.set_face('K');
			}
			else {
				temp.set_face(0);	// a '0' char is NULL. We're setting all non-face cards m_face variable to NULL
				temp.set_value(y);	// all non-face cards just get the value of int y from the loop. Face cards and Ace are the only exceptions
			}
			m_cards.push_back(temp);
		}
	}
}

void deck::shuffle_deck() {
	std::deque<card> shuffled_deck;
	srand(time(NULL));
	while (m_cards.size() > 0) {
		int index = rand() % m_cards.size();
		shuffled_deck.push_back(m_cards[index]);
		m_cards.erase(m_cards.begin()+index);
	}
	m_cards = shuffled_deck;
}

card deck::draw_card() {
	card drawn_card;
	int last_card_index = m_cards.size() - 1;
	drawn_card = m_cards[last_card_index];
	m_cards.pop_back();
	return drawn_card;
}

// DEBUG METHODS

void deck::_debug_dump_deck() {
	using std::cout;
	using std::endl;
	for (int i = 0; i < m_cards.size(); i++) {
		cout << "m_cards[" << i << "] = ";
		if (m_cards[i].is_ace()) {
			cout << "A||"; }
		else if (m_cards[i].get_value() == 10) {
			if (m_cards[i].get_face() == 0)
				cout << m_cards[i].get_value() << "|";
			else cout << m_cards[i].get_face() << "||";
		}
		else cout << m_cards[i].get_value() << "||";
		cout << m_cards[i].get_suit() << endl;
	}
}
