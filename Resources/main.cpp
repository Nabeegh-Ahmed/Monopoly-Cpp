#include <iostream>
#include <SFML/Graphics.hpp>
#include "Cell.h"
#include"Board.h"
#include "PrivateProperty.h"
int PrivateProperty::highestnumberofhotels[7] = { 0,0,0,0,0,0,0 };
int PrivateProperty::highestnumberofhouses[7] = { 0,0,0,0,0,0,0 };
int PrivateProperty::highestnumberofshops[7] = { 0,0,0,0,0,0,0 };
int main() {
	srand(time(NULL));
	Board game;
	game.rungame();
	return 0;
}