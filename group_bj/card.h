#pragma once
#include <iostream>

class card {
public:
	card() {
		// These values are always false by default
		m_is_ace = false;
		m_is_hidden = false;
	}
	~card(){}

	// getters
	char get_suit() { return m_suit; }
	int get_value() { return m_value; }
	bool is_ace() { return m_is_ace; }
	char get_face() { return m_face; }

	// setters
	void set_suit(char suit) { m_suit = suit; }
	void set_value(int value) { m_value = value; }
	void set_ace() { m_is_ace = true; }
	void set_face(char face) { m_face = face; }

	
	void flip_hidden() { m_is_hidden = !m_is_hidden; }
	
	// Used for flipping the value of an Ace from 1 <---> 11
	void flip_ace() {
		if (m_is_ace) {
			if (m_value == 1)
				m_value = 11;
			else if (m_value == 11)
				m_value = 1;
			else m_value = 1;
		}
		else std::cout << "Cannot flip card! " << m_value << " is not an Ace!" << std::endl;
	}

private:
	char m_suit;		// [C]lubs, [H]earts, [D]iamonds, [S]pades
	int m_value;		// 2-10, 11 = [J]ack, 12 = [Q]ueen, 13 = [K]ing, 1/11 = [A]ce
	char m_face;		// [J]ack, [Q]ueen, [K]ing
	bool m_is_ace;		// Because Jack = 11 and Ace CAN = 11, we need a bool flag to track whether this is a Jack or an Ace
	bool m_is_hidden;	// The dealer has one hidden card at the start of each game, and is not revealed until the end.
};

