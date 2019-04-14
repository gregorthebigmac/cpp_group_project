#include "player.h"

void player::display_hand() {
	for (int i = 0; i < m_hand.size(); i++) {
		std::cout << "Card [" << i + 1 << "] is ";
		if (m_hand[i].get_value() == 1)
			std::cout << "A";
		else if (m_hand[i].get_value() == 11)
			std::cout << "J";
		else if (m_hand[i].get_value() == 12)
			std::cout << "Q";
		else if (m_hand[i].get_value() == 13)
			std::cout << "K";
		else {
			std::cout << m_hand[i].get_value();
		}
		std::cout << m_hand[i].get_suit() << std::endl;
	}
}