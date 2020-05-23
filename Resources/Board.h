#ifndef Board_HEADER
#define Board_HEADER

#pragma once
#pragma warning(push, 0) 

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <typeinfo>


#include "GameEngine.h"
#include "PrivateProperty.h"
#include "PublicProperty.h"
#include "Player.h"
#include "Cell.h"
#include "Dice.h"

#include "Event.h"
#include "Chance.h"
#include "Chest.h"
#include "Jail.h"
#include "Card.h"
#include "Special.h"

class Board {
private:
	Cell** listofcells;
	Player** listofplayers;
	sf::View screenview;
	Dice gamedice;
	int numberofplayers;
	int thisplayersturn;
public:
	Board();
	~Board();
	void drawboard(sf::RenderWindow& window);
	void drawplayers(sf::RenderWindow& window);
	void addplayers();
	void moveplayerandswitchturn(sf::RenderWindow& window);
	void moveplayerprocess(sf::RenderWindow& window);
	void spectatecurrentplayer(sf::RenderWindow& window);
	void endturn();
	void addcells();
	void rungame();
};

Board::Board() {
	thisplayersturn = 0;
	numberofplayers = 5;//Interface will decide number of players
	addplayers();
	addcells();
	gamedice.setposition(550, -350);
}
Board::~Board() {
	for (int i = 0; i < 41; i++) {
		delete listofcells[i];
	}
	delete[]listofcells;
}
void Board::rungame() {
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "Monopoly", sf::Style::Close);
	sf::Image icon;
	icon.loadFromFile("assets/textures/inner_board.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	this->spectatecurrentplayer(window);
	screenview.setSize(sf::Vector2f(1280, 1024));
	screenview.zoom(1.025f);
	window.setView(screenview);
	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					screenview.zoom(0.9f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					screenview.zoom(1.1f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
					this->moveplayerandswitchturn(window);
				}
				break;
			}
			
			this->spectatecurrentplayer(window);
			window.clear(sf::Color::White);
			this->drawboard(window);
			this->drawplayers(window);
			window.display();
		}
	}
}
void Board::drawboard(sf::RenderWindow& window) {
	for (int i = 0; i < 41; i++) {
		listofcells[i][0].drawcell(window);
	}
}
void Board::addplayers() {
	this->listofplayers = new Player * [this->numberofplayers];
	for (int i = 0; i < this->numberofplayers; i++) {
		this->listofplayers[i] = new Player;
	}
}
void Board::addcells() {
	listofcells = new Cell * [41];
	
	listofcells[0] = new Special;

	// Johar Town A
	listofcells[1] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[1])->setPrice(200);
	static_cast<PrivateProperty*>(listofcells[1])->setRent(80);
	static_cast<PrivateProperty*>(listofcells[1])->setGroup("johar");

	listofcells[2] = new Chest;

	// Johar Town B
	listofcells[3] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[3])->setPrice(250);
	static_cast<PrivateProperty*>(listofcells[3])->setRent(90);
	static_cast<PrivateProperty*>(listofcells[3])->setGroup("johar");

	listofcells[4] = new Special;

	// Bus Station
	listofcells[5] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[5])->setPrice(2000);
	static_cast<PublicProperty*>(listofcells[5])->setRent(200);
	static_cast<PublicProperty*>(listofcells[5])->setGroup("station");

	// Iqbal Town A
	listofcells[6] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[6])->setPrice(250);	
	static_cast<PrivateProperty*>(listofcells[6])->setRent(60);
	static_cast<PrivateProperty*>(listofcells[6])->setGroup("iqbal");

	listofcells[7] = new Chance;
	
	// Iqbal Town B
	listofcells[8] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[8])->setPrice(300);
	static_cast<PrivateProperty*>(listofcells[8])->setRent(70);
	static_cast<PrivateProperty*>(listofcells[8])->setGroup("iqbal");

	// Iqbal Town C
	listofcells[9] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[9])->setPrice(400);
	static_cast<PrivateProperty*>(listofcells[9])->setRent(100);
	static_cast<PrivateProperty*>(listofcells[9])->setGroup("iqbal");

	// Jail
	listofcells[10] = new Jail;

	// Faisal Town A
	listofcells[11] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[11])->setPrice(300);
	static_cast<PrivateProperty*>(listofcells[11])->setRent(100);
	static_cast<PrivateProperty*>(listofcells[11])->setGroup("faisal");
	
	// SUI
	listofcells[12] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[12])->setPrice(2500);
	static_cast<PublicProperty*>(listofcells[12])->setRent(350);
	static_cast<PublicProperty*>(listofcells[12])->setGroup("utility");

	// Faisal Town B
	listofcells[13] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[13])->setPrice(400);
	static_cast<PrivateProperty*>(listofcells[13])->setRent(130);
	static_cast<PrivateProperty*>(listofcells[13])->setGroup("faisal");

	// Faisal Town C
	listofcells[14] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[13])->setPrice(400);
	static_cast<PrivateProperty*>(listofcells[13])->setRent(130);
	static_cast<PrivateProperty*>(listofcells[13])->setGroup("faisal");

	// Metro Station
	listofcells[15] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[15])->setPrice(2000);
	static_cast<PublicProperty*>(listofcells[15])->setRent(200);
	static_cast<PublicProperty*>(listofcells[15])->setGroup("station");

	// Model Town A
	listofcells[16] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[16])->setPrice(800);
	static_cast<PrivateProperty*>(listofcells[16])->setRent(200);
	static_cast<PrivateProperty*>(listofcells[16])->setGroup("model");

	listofcells[17] = new Chest;

	// Model Town B
	listofcells[18] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[18])->setPrice(850);
	static_cast<PrivateProperty*>(listofcells[18])->setRent(250);
	static_cast<PrivateProperty*>(listofcells[18])->setGroup("model");

	// Model Town C
	listofcells[19] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[19])->setPrice(2000);
	static_cast<PrivateProperty*>(listofcells[19])->setRent(500);
	static_cast<PrivateProperty*>(listofcells[19])->setGroup("model");

	listofcells[20] = new Special; // Parking

	// Gulberg A
	listofcells[21] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[21])->setPrice(1000);
	static_cast<PrivateProperty*>(listofcells[21])->setRent(300);
	static_cast<PrivateProperty*>(listofcells[21])->setGroup("gulberg");

	listofcells[22] = new Chance;

	// Gulberg B
	listofcells[23] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[23])->setPrice(1200);
	static_cast<PrivateProperty*>(listofcells[23])->setRent(350);
	static_cast<PrivateProperty*>(listofcells[23])->setGroup("gulberg");

	// Gulberg C
	listofcells[24] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[24])->setPrice(2500);
	static_cast<PrivateProperty*>(listofcells[24])->setRent(600);
	static_cast<PrivateProperty*>(listofcells[24])->setGroup("gulberg");

	// Orange Station
	listofcells[25] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[25])->setPrice(2500);
	static_cast<PublicProperty*>(listofcells[25])->setRent(350);
	static_cast<PublicProperty*>(listofcells[25])->setGroup("station");

	// DHA A
	listofcells[26] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[26])->setPrice(2000);
	static_cast<PrivateProperty*>(listofcells[26])->setRent(500);
	static_cast<PrivateProperty*>(listofcells[26])->setGroup("dha");

	// DHA B
	listofcells[27] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[27])->setPrice(2000);
	static_cast<PrivateProperty*>(listofcells[27])->setRent(500);
	static_cast<PrivateProperty*>(listofcells[27])->setGroup("dha");

	// WASA
	listofcells[28] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[28])->setPrice(4000);
	static_cast<PublicProperty*>(listofcells[28])->setRent(400);
	static_cast<PublicProperty*>(listofcells[28])->setGroup("utility");

	// DHA C
	listofcells[29] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[29])->setPrice(2500);
	static_cast<PrivateProperty*>(listofcells[29])->setRent(1000);
	static_cast<PrivateProperty*>(listofcells[29])->setGroup("dha");

	listofcells[30] = new Special; // Police Guy

	// Bahria A
	listofcells[31] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[31])->setPrice(2500);
	static_cast<PrivateProperty*>(listofcells[31])->setRent(800);
	static_cast<PrivateProperty*>(listofcells[31])->setGroup("bahria");

	listofcells[32] = new Chest;

	// Bahria B
	listofcells[33] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[33])->setPrice(3000);
	static_cast<PrivateProperty*>(listofcells[33])->setRent(900);
	static_cast<PrivateProperty*>(listofcells[33])->setGroup("bahria");

	// Bahria C
	listofcells[34] = new PrivateProperty;
	static_cast<PrivateProperty*>(listofcells[34])->setPrice(3000);
	static_cast<PrivateProperty*>(listofcells[34])->setRent(1000);
	static_cast<PrivateProperty*>(listofcells[34])->setGroup("bahria");

	// Railway Station
	listofcells[35] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[35])->setPrice(2500);
	static_cast<PublicProperty*>(listofcells[35])->setRent(350);
	static_cast<PublicProperty*>(listofcells[35])->setGroup("station");

	listofcells[36] = new Chance;

	// LESCO
	listofcells[37] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[37])->setPrice(5000);
	static_cast<PublicProperty*>(listofcells[37])->setRent(450);
	static_cast<PublicProperty*>(listofcells[37])->setGroup("utility");

	listofcells[38] = new Special; // Property Tax

	// PTCL
	listofcells[39] = new PublicProperty;
	static_cast<PublicProperty*>(listofcells[39])->setPrice(8000);
	static_cast<PublicProperty*>(listofcells[39])->setRent(500);
	static_cast<PublicProperty*>(listofcells[39])->setGroup("utility");

	listofcells[40] = new Cell;
};
void Board::drawplayers(sf::RenderWindow& window) {
	for (int i = 0; i < this->numberofplayers; i++) {
		this->listofplayers[i][0].drawplayer(window);
	}
}
void Board::spectatecurrentplayer(sf::RenderWindow& window) {
	screenview.setCenter(this->listofplayers[this->thisplayersturn][0].getplayercoordinates());
	window.setView(screenview);
}
void Board::moveplayerprocess(sf::RenderWindow& window) {
	this->spectatecurrentplayer(window);
	sf::RectangleShape tempplayershape = listofplayers[thisplayersturn][0].getplayershape();
	int currentcell = listofplayers[thisplayersturn][0].getcurrentlyoncell();
	float amounttoshiftx = 0;
	float amounttoshifty = 0;
	currentcell++;
	if (currentcell >= 1 && currentcell <= 11) {
		amounttoshiftx = -0.150;
		amounttoshifty = 0;
	}
	else {
		if (currentcell >= 11 && currentcell <= 21) {
			amounttoshiftx = 0;
			amounttoshifty = -0.150;
		}
		else {
			if (currentcell >= 21 && currentcell <= 31) {
				amounttoshiftx = 0.150;
				amounttoshifty = 0;
			}
			else {
				if (currentcell >= 31 && currentcell <= 40) {
					amounttoshiftx = 0;
					amounttoshifty = 0.150;
					if (currentcell == 40) {
						currentcell = 1;
					}
				}
			}
		}
	}
	for (float j = 1; j <= 1.75; j += 0.005) {
		listofplayers[thisplayersturn][0].getplayershape().setScale(j, j);
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		window.display();
	}
	for (int j = 0; j < 1000; j++) {
		listofplayers[thisplayersturn][0].getplayershape().move(amounttoshiftx, amounttoshifty);
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		window.display();
	}
	for (float j = 1.75; j >= 1; j -= 0.005) {
		listofplayers[thisplayersturn][0].getplayershape().setScale(j, j);
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		window.display();
	}
	listofplayers[thisplayersturn][0].setplayercoordinates(tempplayershape.getPosition());
	this->spectatecurrentplayer(window);
	listofplayers[thisplayersturn][0].setcurrentcell(currentcell);
}
void Board::moveplayerandswitchturn(sf::RenderWindow& window) {
	screenview.setCenter(500, -300);
	window.setView(screenview);
	int roll = 0;
	for (int counter = 0; counter < 100; counter++) {
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		roll = gamedice.Roll();
		gamedice.Draw(window);
		window.display();
	}
	window.clear(sf::Color::White);
	this->drawboard(window);
	this->drawplayers(window);
	gamedice.Draw(window);
	window.display();
	for (int timebuffer = 0; timebuffer < 100000000; timebuffer++) {
		for (int timebuffer2 = 0; timebuffer2 < 6; timebuffer2++) {

		}
	}
	for (int i = 0; i < roll; i++) {
		this->moveplayerprocess(window);
	}
	this->endturn();
}
void Board::endturn() {
	this->thisplayersturn++;
	if (this->thisplayersturn >= numberofplayers) {
		this->thisplayersturn = 0;
	}
}
#endif