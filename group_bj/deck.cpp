#include "deck.h"

deck::deck(){
	card temp;
	char suit;
	for (int x = 0; x < 4; x++) {
		for (int y = 1; y < 14; y++) {
			if (x == 0)
				suit = 'C';
			else if (x == 1)
				suit = 'H';
			else if (x == 2)
				suit = 'D';
			else if (x == 3)
				suit = 'S';
			else std::cout << "deck::deck for loop broke!" << std::endl;
			temp.set_suit(suit);
			temp.set_value(y);
			m_cards.push_back(temp);
		}
	}
}
deck::~deck(){}

// DEBUG METHODS

void deck::_debug_dump_deck() {
	for (int i = 0; i < m_cards.size(); i++) {
		std::cout << "m_cards[" << i << "] = ";
		if (m_cards[i].get_value() == 1) {
			std::cout << "A"; }
		else if (m_cards[i].get_value() == 11) {
			std::cout << "J"; }
		else if (m_cards[i].get_value() == 12) {
			std::cout << "Q"; }
		else if (m_cards[i].get_value() == 13) {
			std::cout << "K"; }
		else {
			std::cout << m_cards[i].get_value();
		}
		std::cout << m_cards[i].get_suit() << std::endl;
	}
}
