#pragma once
#include <iostream>
#include <vector>

class card {
public:
	card();
	~card();

	// getters
	int get_suit() { return m_suit; }
	int get_value() { return m_value; }
	// setters
	void set_suit(int suit) { m_suit = suit; }
	void set_value(int value) { m_value = value; }

private:
	int m_suit;	// 0 = clubs, 1 = hearts, 2 = diamonds, 3 = spades
	int m_value;
};

