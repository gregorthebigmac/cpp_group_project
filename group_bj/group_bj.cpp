#include "game.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

char new_game();

int main() {
	char play_again = 'y';
	while (play_again == 'y') {
		play_again = new_game();
	}
	return 0;
}

char new_game() {
	game g;
	g.welcome_splash();
	cout << "That was fun! Play again? y/n" << endl;
	string response;
	getline(cin, response);
	if (response[0] == 'y' || response[0] == 'Y')
		return 'y';
	else return 'n';
}