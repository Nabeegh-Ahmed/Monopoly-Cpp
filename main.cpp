#include <iostream>
#include <SFML/Graphics.hpp>
#include "Resources/Cell.h"
#include"Resources/Board.h"
#include "Resources/PrivateProperty.h"

int PrivateProperty::highestnumberofhotels[7] = { 0,0,0,0,0,0,0 };
int PrivateProperty::highestnumberofhouses[7] = { 0,0,0,0,0,0,0 };
int PrivateProperty::highestnumberofshops[7] = { 0,0,0,0,0,0,0 };
int main() {
	Board game;
	game.rungame();
	return 0;
}