#ifndef Player_HEADER
#define Player_HEADER
#pragma once
#include <string>
#include "Cell.h"
#include <iostream>
#include <SFML/Graphics.hpp>
class Player {
private:
	bool bankrupt;
	int playerID;
	int oncellnumber;
	int balance;
	int turnsinjail;
	int* propertiesownedlist;
	int numberofpropertiesowned;
	bool listofgroups[7];
	int GOJFcards = 0;
	sf::RectangleShape playershape;
	sf::Image playerimage;
	sf::Texture playergraphic;
	sf::Vector2f playertopleftcoordinates;
public:
	Player();
	void drawplayer(sf::RenderWindow& window);
	/*void moveplayer(int diceroll, sf::RenderWindow& window);*/
	sf::Vector2f getplayercoordinates();
	~Player();
	void updateplayercoordinates(sf::Vector2f playercoordinates);
	void setcurrentcell(int currentlyoncell);
	void addpropertyID(int ID);
	bool operator[](int index);
	void removepropertyID(int ID);
	bool ownallstations();
	void setbankruptflag(bool flag);
	bool getbankruptflag();
	bool ownallutilities();
	void setGOJFcardamount(int num);
	void gainGOJFcard();
	bool removeGOJFcard();
	int getGOJFcard();
	int getbalance();
	int getcurrentlyoncell();
	void spendturninjail();
	void setbalance(int bal);
	int getturnsinjail();
	void setturnsinjail(int turns);
	void depositcash(int amount);
	void setplayercoordinates(sf::Vector2f playercoordinates);
	bool withdrawcash(int amount);
	void displayownedpropsconsole();
	void updatelistofownedgroups();
	bool playerownsallofthisprivategroup(int propertyID);
	sf::RectangleShape& getplayershape();
	bool propertyowned(int propertyID);
	int getPlayerID();
	int getnumberofpropertiesowned();
	int* getPropertyIDlist();

};

Player::Player() {
	bankrupt = 0;
	turnsinjail = 0;
	GOJFcards = 0;
	static int incrementer = 1;
	static float coordx = 950;
	static float coordy = 900;
	for (int i = 0; i < 7; i++) {
		this->listofgroups[i] = false;
	}
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
	propertiesownedlist = 0;
	numberofpropertiesowned = 0;
	std::string filepath = "assets/Pawns/P" + std::to_string(incrementer) + ".png";
	playerID = incrementer;
	balance = 5000;
	playershape.setSize(sf::Vector2f(50, 50));
	playerimage.loadFromFile(filepath);
	playergraphic.loadFromImage(playerimage);
	playershape.setTexture(&playergraphic);
	playershape.setPosition(sf::Vector2f(coordx, coordy));
	playertopleftcoordinates = sf::Vector2f(coordx, coordy);
	oncellnumber = 1;
	incrementer++;
}
int Player::getnumberofpropertiesowned() { return this->numberofpropertiesowned; }
int* Player::getPropertyIDlist() { return this->propertiesownedlist; }

int Player::getPlayerID() {
	return this->playerID;
}

Player::~Player() {
	if (propertiesownedlist != 0) {
		delete[]propertiesownedlist;
	}
}
void Player::drawplayer(sf::RenderWindow& window) {
	window.draw(this->playershape);
}
sf::Vector2f Player::getplayercoordinates() {
	return playertopleftcoordinates;
}
void Player::updateplayercoordinates(sf::Vector2f playercoordinates) {
	playertopleftcoordinates = playershape.getPosition();
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
void Player::addpropertyID(int ID) {
	int* tempIDholder = 0;
	numberofpropertiesowned++;
	if (propertiesownedlist == 0) {
		propertiesownedlist = new int[1];
		propertiesownedlist[0] = ID;
	}
	else {
		tempIDholder = new int[numberofpropertiesowned];
		for (int i = 0; i < numberofpropertiesowned; i++) {
			tempIDholder[i] = 0;
		}
		for (int i = 0; i < numberofpropertiesowned - 1; i++) {
			tempIDholder[i] = propertiesownedlist[i];
		}
		tempIDholder[numberofpropertiesowned - 1] = ID;
		delete[]propertiesownedlist;
		propertiesownedlist = new int[numberofpropertiesowned];
		for (int i = 0; i < numberofpropertiesowned; i++) {
			propertiesownedlist[i] = tempIDholder[i];
		}
		delete[]tempIDholder;
		tempIDholder = 0;
	}
}
void Player::removepropertyID(int ID) {
	int* tempIDholder = 0;
	if (propertiesownedlist == 0) {
	}
	else {
		numberofpropertiesowned--;
		if (numberofpropertiesowned != 0) {
			tempIDholder = new int[numberofpropertiesowned];
			for (int i = 0; i < numberofpropertiesowned; i++) {
				tempIDholder[i] = 0;
			}
			for (int i = 0, k = 0; i < numberofpropertiesowned + 1; i++) {
				if (propertiesownedlist[i] == ID) {
				}
				else {
					tempIDholder[k] = propertiesownedlist[i];
					k++;
				}
			}
			delete[]propertiesownedlist;
			propertiesownedlist = new int[numberofpropertiesowned];
			for (int i = 0; i < numberofpropertiesowned; i++) {
				propertiesownedlist[i] = tempIDholder[i];
			}
			delete[]tempIDholder;
			tempIDholder = 0;
		}
		else {
			delete[]propertiesownedlist;
			propertiesownedlist = 0;
		}
	}
}
void Player::displayownedpropsconsole() {
	std::cout << "----Properties owned by player " << this->playerID << "----\n";
	for (int i = 0; i < numberofpropertiesowned; i++) {
		std::cout << propertiesownedlist[i] << std::endl;
	}
	if (numberofpropertiesowned == 0) {
		std::cout << "None\n";
	}
	std::cout << "-----------------------------------";
}
bool Player::propertyowned(int propertyID) {
	if (numberofpropertiesowned == 0) {
		return false;
	}
	else {
		for (int i = 0; i < numberofpropertiesowned; i++) {
			if (propertiesownedlist[i] == propertyID) {
				return true;
			}
		}
		return false;
	}
}
bool Player::ownallstations() {
	int counter = 0;
	for (int i = 0; i < numberofpropertiesowned; i++) {
		if (propertiesownedlist[i] == 3 || propertiesownedlist[i] == 11 || propertiesownedlist[i] == 18 || propertiesownedlist[i] == 26) {
			counter++;
		}
	}
	if (counter == 4) {
		return true;
	}
	return false;
}
bool Player::ownallutilities() {
	int counter = 0;
	for (int i = 0; i < numberofpropertiesowned; i++) {
		if (propertiesownedlist[i] == 8 || propertiesownedlist[i] == 21 || propertiesownedlist[i] == 27 || propertiesownedlist[i] == 28) {
			counter++;
		}
	}
	if (counter == 4) {
		return true;
	}
	return false;
}
void Player::depositcash(int amount) {
	this->balance += amount;
}
bool Player::withdrawcash(int amount) {
	if (balance >= amount) {
		this->balance -= amount;
		return true;
	}
	else {
		return false;
	}
}
int Player::getbalance() {
	return this->balance;
}
void Player::updatelistofownedgroups() {
	if (propertyowned(1) && propertyowned(2)) {
		listofgroups[0] = 1;
	}
	else {
		listofgroups[0] = 0;
	}
	if (propertyowned(4) && propertyowned(5) && propertyowned(6)) {
		listofgroups[1] = 1;
	}
	else {
		listofgroups[1] = 0;
	}
	if (propertyowned(7) && propertyowned(9) && propertyowned(10)) {
		listofgroups[2] = 1;
	}
	else {
		listofgroups[2] = 0;
	}
	if (propertyowned(12) && propertyowned(13) && propertyowned(14)) {
		listofgroups[3] = 1;
	}
	else {
		listofgroups[3] = 0;
	}
	if (propertyowned(15) && propertyowned(16) && propertyowned(17)) {
		listofgroups[4] = 1;
	}
	else {
		listofgroups[4] = 0;
	}
	if (propertyowned(19) && propertyowned(20) && propertyowned(22)) {
		listofgroups[5] = 1;
	}
	else {
		listofgroups[5] = 0;
	}
	if (propertyowned(23) && propertyowned(24) && propertyowned(25)) {
		listofgroups[6] = 1;
	}
	else {
		listofgroups[6] = 0;
	}

}
bool Player::operator[](int index) {
	if (index >= 0 && index < 7) {
		return this->listofgroups[index];
	}
}
bool Player::playerownsallofthisprivategroup(int propertyID) {
	this->updatelistofownedgroups();
	if (propertyID == (1) || propertyID == (2)) {
		return listofgroups[0];
	}
	if (propertyID == (4) || propertyID == (5) || propertyID == (6)) {
		return listofgroups[1];
	}

	if (propertyID == (7) || propertyID == (9) || propertyID == (10)) {
		return listofgroups[2];
	}

	if (propertyID == (12) || propertyID == (13) || propertyID == (14)) {
		return listofgroups[3];
	}

	if (propertyID == (15) || propertyID == (16) || propertyID == (17)) {
		return listofgroups[4];
	}

	if (propertyID == (19) || propertyID == (20) || propertyID == (22)) {
		return listofgroups[5];
	}

	if (propertyID == (23) || propertyID == (24) || propertyID == (25)) {
		return listofgroups[6];
	}
	return 0;
}
void Player::spendturninjail() {
	this->turnsinjail--;
	if (this->turnsinjail < 0) {
		this->turnsinjail = 0;
	}
}
int Player::getturnsinjail() {
	return this->turnsinjail;
}
void Player::setturnsinjail(int turns) {
	this->turnsinjail = turns;
}
void Player::gainGOJFcard() {
	GOJFcards++;
}
bool Player::removeGOJFcard() {
	if (this->GOJFcards > 0) {
		GOJFcards--;
		return true;
	}
	return false;
}
int Player::getGOJFcard() {
	return this->GOJFcards;
}
void Player::setbankruptflag(bool flag) {
	this->bankrupt = flag;
}
bool Player::getbankruptflag() {
	return this->bankrupt;
}
void Player::setplayercoordinates(sf::Vector2f playercoordinates) {
	this->playertopleftcoordinates = playercoordinates;
	playershape.setPosition(playertopleftcoordinates);
}
void Player::setbalance(int bal) {
	this->balance = bal;
}
void Player::setGOJFcardamount(int num) {
	this->GOJFcards = num;
}
#endif