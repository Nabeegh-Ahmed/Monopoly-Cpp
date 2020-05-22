#ifndef Cell_HEADER
#define Cell_HEADER
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
class Cell {
private:
	int cellID;
	sf::RectangleShape cellshape;
	sf::Image cellimage;
	sf::Texture cellgraphic;
	sf::Vector2f celltopleftcoordinates;
public:
	Cell();
	sf::Vector2f getcellcoordinates();
	void drawcell(sf::RenderWindow& window);
	int getcellid();
	~Cell() {

	}
};


Cell::Cell() {
	std::string texturestring;
	static int incrementer=1;
	static float coordx = 1100;
	static float coordy = 850;
	static float amounttoshiftx = 0;
	static float amounttoshifty = 0;
	switch (incrementer) {
	case 1:
		amounttoshiftx = -150;
		amounttoshifty = 0;
		break;
	case 12:
		amounttoshiftx = 0;
		amounttoshifty = -150;
		break;
	case 22:
		amounttoshiftx = 150;
		amounttoshifty = 0;
		break;
	case 32:
		amounttoshiftx = 0;
		amounttoshifty = 150;
		break;
	}
	if (incrementer != 41) {
		coordx += amounttoshiftx;
		coordy += amounttoshifty;
		texturestring = "Textures/Cell" + std::to_string(incrementer) + ".png";
		cellshape.setSize(sf::Vector2f(150.0f, 150.0f));
		cellimage.loadFromFile(texturestring);
		celltopleftcoordinates = sf::Vector2f(coordx, coordy);
		cellshape.setPosition(celltopleftcoordinates);
		cellgraphic.loadFromImage(cellimage);
		cellshape.setTexture(&cellgraphic);
		cellID = incrementer;
	}
	else {
		texturestring = "Textures/Inner_Board.png";
		coordx = -400;
		coordy = -500;
		cellshape.setSize(sf::Vector2f(1350.0f, 1350.0f));
		cellimage.loadFromFile(texturestring);
		celltopleftcoordinates = sf::Vector2f(coordx, coordy);
		cellshape.setPosition(celltopleftcoordinates);
		cellgraphic.loadFromImage(cellimage);
		cellshape.setTexture(&cellgraphic);
		cellID = incrementer;
	}
	incrementer++;
}
sf::Vector2f Cell::getcellcoordinates() {
	return celltopleftcoordinates;
}
void Cell::drawcell(sf::RenderWindow& window) {
	window.draw(cellshape);
}
int Cell::getcellid() {
	return cellID;
}
#endif