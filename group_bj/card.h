#pragma once
#include <iostream>

class card {
public:
	card(){}
	~card(){}

	// getters
	char get_suit() { return m_suit; }
	int get_value() { return m_value; }
	// setters
	void set_suit(char suit) { m_suit = suit; }
	void set_value(int value) { m_value = value; }

private:
	
	char m_suit;	// [C]lubs, [H]earts, [D]iamonds, [S]pades
	int m_value;	// 2-10, [J]ack, [Q]ueen, [K]ing, [A]ce
};

