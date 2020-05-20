#ifndef Board_HEADER
#define Board_HEADER
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Cell.h"
#include "Dice.h"
class Board{
private:
	Cell** listofcells;
	Dice gamedice;
public:
	Board();
	~Board();
	void drawboard(sf::RenderWindow& window);
	void rungame();
};


Board::Board() {
	gamedice.setposition(550, -350);
	this->listofcells = new Cell * [41];
	for (int i = 0; i < 41; i++) {
		this->listofcells[i] = new Cell;
	}
}
Board::~Board() {
	for (int i = 0; i < 41; i++) {
		delete this->listofcells[i];
	}
	delete[]this->listofcells;
}
void Board::rungame() {
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "Monopoly", sf::Style::Close);
	sf::View view1(sf::Vector2f(640, 512), sf::Vector2f(1280, 1024));
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
					view1.zoom(0.9);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					view1.zoom(1.1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
					gamedice.Roll();
				}
				break;
			}
			window.clear(sf::Color::White);
			window.setView(view1);
			this->drawboard(window);
			gamedice.Draw(window);
			window.display();
		}
	}
}
void Board::drawboard(sf::RenderWindow& window) {
	for (int i = 0; i < 41; i++) {
		this->listofcells[i][0].drawcell(window);
	}
}
#endif