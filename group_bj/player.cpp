#include "player.h"

void player::calc_total() {
	m_low_total = 0;
	m_high_total = 0;
	for (int i = 0; i < m_hand.size(); i++) {
		if (m_hand[i].get_value() == 1) {
			m_low_total = m_low_total + 1;
			m_high_total = m_high_total + 11;
		}
		else {
			m_low_total = m_low_total + m_hand[i].get_value();
			m_high_total = m_high_total + m_hand[i].get_value();
		}
	}
}

void player::display_hand() {
	for (int i = 0; i < m_hand.size(); i++) {
		std::cout << "Card [" << i + 1 << "] is the ";
		if ((m_hand[i].get_value() == 1) || (m_hand[i].get_value() == 11)) {
			std::cout << "Ace";}
		else if (m_hand[i].get_value() == 11) {
			std::cout << "Jack";}
		else if (m_hand[i].get_value() == 12) {
			std::cout << "Queen"; }
		else if (m_hand[i].get_value() == 13) {
			std::cout << "King"; }
		else {
			std::cout << m_hand[i].get_value();
		}
		std::cout << " of " << m_hand[i].get_suit() << std::endl;
	}
}