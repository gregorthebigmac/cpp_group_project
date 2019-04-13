#include "deck.h"

deck::deck(){
	card temp;
	for (int suit = 0; suit < 4; suit++) {
		for (int val = 1; val < 14; val++) {
			temp.set_suit(suit);
			temp.set_value(val);
			m_cards.push_back(temp);
		}
	}
}
deck::~deck(){}

void deck::_debug_dump_deck() {
	for (int i = 0; i < m_cards.size(); i++) {
		std::cout << "m_cards[" << i << "] = ";
		if (m_cards[i].get_value() == 1) {
			std::cout << "Ace"; }
		else if (m_cards[i].get_value() == 11) {
			std::cout << "Jack"; }
		else if (m_cards[i].get_value() == 12) {
			std::cout << "Queen"; }
		else if (m_cards[i].get_value() == 13) {
			std::cout << "King"; }
		else {
			std::cout << m_cards[i].get_value();
		}
		if (m_cards[i].get_suit() == 0) {
			std::cout << " of clubs"; }
		else if (m_cards[i].get_suit() == 1) {
			std::cout << " of hearts"; }
		else if (m_cards[i].get_suit() == 2) {
			std::cout << " of diamonds"; }
		else if (m_cards[i].get_suit() == 3) {
			std::cout << " of spades"; }
		else std::cout << "of garbage data!";
		std::cout << std::endl;
	}
}