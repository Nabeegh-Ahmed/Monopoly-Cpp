#ifndef Dice_HEADER
#define Dice_HEADER
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
class Dice {
private:
	sf::RectangleShape dice;
	sf::Texture* faces;
public:
	Dice() {
		float width = 150;
		float height = 150;
		srand(time(NULL));
		dice.setSize(sf::Vector2f(width, height));
		faces = new sf::Texture[6];
		for (unsigned int i = 0; i < 6; i++) {
			std::string path = "assets/Dice/dice" + std::to_string(i + 1) + ".png";
			faces[i].loadFromFile(path);
		}
		dice.setTexture(&faces[0]);
	}
	Dice(float width, float height) {
		srand(time(NULL));
		dice.setSize(sf::Vector2f(width, height));
		faces = new sf::Texture[6];
		for (unsigned int i = 0; i < 6; i++) {
			std::string path = "assets/Dice/dice" + std::to_string(i + 1) + ".png";
			faces[i].loadFromFile(path);
		}
		dice.setTexture(&faces[0]);
	}
	sf::RectangleShape getBlock() {
		return dice;
	}
	int Roll() {
		unsigned int faceNumber = rand() % 6 + 0;
		dice.setTexture(&faces[faceNumber]);
		return faceNumber + 1;
	}
	void Draw(sf::RenderWindow& window) {
		window.draw(dice);
	}
	void setposition(float x, float y) {
		this->dice.setPosition(sf::Vector2f(x, y));
	}
};
#endif