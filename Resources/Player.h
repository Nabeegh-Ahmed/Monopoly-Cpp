#ifndef Player_HEADER
#define Player_HEADER
#pragma once
#include <string>
#include "Cell.h"
#include <SFML/Graphics.hpp>
class Player {
private:
	int playerID;
	int oncellnumber;
	float balance;
	sf::RectangleShape playershape;
	sf::Image playerimage;
	sf::Texture playergraphic;
	sf::Vector2f playertopleftcoordinates;
public:
	Player();
	void drawplayer(sf::RenderWindow& window);
	/*void moveplayer(int diceroll, sf::RenderWindow& window);*/
	sf::Vector2f getplayercoordinates();
	void setplayercoordinates(sf::Vector2f playercoordinates);
	void setcurrentcell(int currentlyoncell);
	int getcurrentlyoncell();
	sf::RectangleShape& getplayershape();
	friend class Board;
};

Player::Player() {
	static int incrementer = 1;
	static float coordx = 950;
	static float coordy = 900;
	switch (incrementer) {
	case 2:
		coordx = 1000;
		break;
	case 3:
		coordx = 1050;
		break;
	case 4:
		coordx = 950;
		coordy = 950;
		break;
	case 5:
		coordx = 1000;
		break;
	case 6:
		coordx = 1050;
		break;
	}
	std::string filepath="Assets/Pawns/P"+std::to_string(incrementer)+".png";
	playerID = incrementer;
	balance = 5000; 
	playershape.setSize(sf::Vector2f(50, 50));
	playerimage.loadFromFile(filepath);
	playergraphic.loadFromImage(playerimage);
	playershape.setTexture(&playergraphic);
	playershape.setPosition(sf::Vector2f(coordx,coordy));
	playertopleftcoordinates = sf::Vector2f(coordx, coordy);
	oncellnumber = 1;
	incrementer++;
}
void Player::drawplayer(sf::RenderWindow& window) {
	window.draw(this->playershape);
}
sf::Vector2f Player::getplayercoordinates() {
	return playertopleftcoordinates;
}
void Player::setplayercoordinates(sf::Vector2f playercoordinates) {
	this->playertopleftcoordinates = playercoordinates;
}
int Player::getcurrentlyoncell() {
	return oncellnumber;
}
void Player::setcurrentcell(int currentlyoncell) {
	this->oncellnumber = currentlyoncell;
}
sf::RectangleShape& Player::getplayershape() {
	return playershape;
}
#endif