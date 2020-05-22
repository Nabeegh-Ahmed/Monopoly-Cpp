#ifndef Board_HEADER
#define Board_HEADER
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Cell.h"
#include "Dice.h"
#include "Player.h"
#include <iostream>
class Board{
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
	void moveplayerandswitchturn( sf::RenderWindow& window);
	void moveplayerprocess( sf::RenderWindow& window);
	void spectatecurrentplayer( sf::RenderWindow& window);
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
					this->moveplayerandswitchturn( window);
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
	for (int i = 0; i < 41; i++) {
		listofcells[i] = new Cell;
	}
}
void Board::drawplayers(sf::RenderWindow& window) {
	for (int i = 0; i < this->numberofplayers; i++) {
		this->listofplayers[i][0].drawplayer(window);
	}
}
void Board::spectatecurrentplayer( sf::RenderWindow & window) {;
	screenview.setCenter(this->listofplayers[this->thisplayersturn][0].getplayercoordinates());
	window.setView(screenview);
}
void Board::moveplayerprocess(sf::RenderWindow & window) {
	this->spectatecurrentplayer(window);
	sf::RectangleShape tempplayershape= listofplayers[thisplayersturn][0].getplayershape();
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
					if (currentcell >= 31 && currentcell <= 41) {
						amounttoshiftx = 0;
						amounttoshifty = 0.150;
						if (currentcell == 41) {
							currentcell = 1;
						}
					}
				}
			}
		}
		for (float j = 1; j <= 1.75; j+=0.005) {
			listofplayers[thisplayersturn][0].getplayershape().setScale(j, j);
			window.clear(sf::Color::White);
			this->drawboard(window);
			this->drawplayers(window);
			window.display();
		}
		for (int j = 0; j < 1000; j++) {
		listofplayers[thisplayersturn][0].getplayershape().move(amounttoshiftx,amounttoshifty);
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		window.display();
		}
		for (float j = 1.75; j >=1; j -= 0.005) {
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
void Board::moveplayerandswitchturn( sf::RenderWindow& window) {
	screenview.setCenter(500, -300);
	window.setView(screenview);
	int roll = 0;
	for (int counter = 0; counter < 2000; counter++) {
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		roll=gamedice.Roll();
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