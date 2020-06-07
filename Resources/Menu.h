#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "GameEngine.h"

struct userOptions {
	int amountOfPlayers = 2;
	int startingMoney = 500;
};

class Menu {
	GPEngine Engine;
	sf::RectangleShape* options;
	sf::RectangleShape* money;
	sf::Texture* moneyTexture;
	sf::Texture* optionsTexture;
	sf::RectangleShape MonopolyLogo;
	sf::Texture MonopolyLogoTexture;
	sf::RectangleShape nbrOfPlayers;
	sf::Texture nbrOfPlayersTexture;
	sf::RectangleShape startingCash;
	sf::Texture startingCashTexture;
	sf::RectangleShape PlayBtn;
	sf::Texture PlayBtnTexture;
	sf::RectangleShape LoadBtn;
	sf::Texture LoadBtnTexture;
public:
	Menu() {
		options = new sf::RectangleShape[4];
		optionsTexture = new sf::Texture[4];
		money = new sf::RectangleShape[3];
		moneyTexture = new sf::Texture[3];
		
		MonopolyLogoTexture.loadFromFile("assets/Menu/MonopolyLogo.png");
		MonopolyLogo.setSize(sf::Vector2f(400, 200));
		MonopolyLogo.setPosition(400, 50);
		MonopolyLogo.setTexture(&MonopolyLogoTexture);

		nbrOfPlayersTexture.loadFromFile("assets/Menu/nbrOfPlayers.png");
		nbrOfPlayers.setSize(sf::Vector2f(558, 100));
		nbrOfPlayers.setPosition(320, 270);
		nbrOfPlayers.setTexture(&nbrOfPlayersTexture);

		startingCashTexture.loadFromFile("assets/Menu/startingCash.png");
		startingCash.setSize(sf::Vector2f(558, 100));
		startingCash.setPosition(320, 500);
		startingCash.setTexture(&startingCashTexture);

		for (unsigned int i = 0, j=2, x = 370; i < 4; i++, x += 120, j++) {
			options[i].setPosition(x, 400);
			options[i].setOutlineColor(sf::Color::Black);
			options[i].setOutlineThickness(5);
			options[i].setSize(sf::Vector2f(100, 50));
			std::string filePath = "assets/Menu/" + std::to_string(j) + "Player.png";
			optionsTexture[i].loadFromFile(filePath);
			options[i].setTexture(&optionsTexture[i]);
		}
		
		for (unsigned int i = 0, x = 430; i < 3; i++, x += 120) {
			money[i].setPosition(x, 620);
			money[i].setOutlineColor(sf::Color::Black);
			money[i].setOutlineThickness(5);
			std::string filePath = "assets/Menu/Rs" + std::to_string(i + 1) + ".png";
			moneyTexture[i].loadFromFile(filePath);
			money[i].setTexture(&moneyTexture[i]);
			money[i].setSize(sf::Vector2f(100, 50));
		}
		//Original 550,750
		LoadBtn.setPosition(500, 750);
		LoadBtn.setSize(sf::Vector2f(200, 50));
		LoadBtn.setOutlineColor(sf::Color::Black);
		LoadBtn.setOutlineThickness(5);
		LoadBtnTexture.loadFromFile("assets/Menu/loadGame.png");
		LoadBtn.setTexture(&LoadBtnTexture);

		PlayBtn.setPosition(550, 860);
		PlayBtn.setSize(sf::Vector2f(100, 50));
		PlayBtn.setOutlineColor(sf::Color::Black);
		PlayBtn.setOutlineThickness(5);
		PlayBtnTexture.loadFromFile("assets/Menu/Play.png");
		PlayBtn.setTexture(&PlayBtnTexture);

	}

	bool MenuPlay(userOptions& attributes, sf::Event event, sf::RenderWindow& window, bool &loadflag) {
		for (int i = 0; i < 4; i++) {
			if (Engine.MouseClick(options[i], event, window)) {
				attributes.amountOfPlayers = i + 2;
			}
		}
		for (int i = 0; i < 3; i++) {
			if (Engine.MouseClick(money[i], event, window)) {
				attributes.startingMoney = (i+1)*500;
			}
		}
		if (Engine.MouseClick(LoadBtn, event, window)) {
			loadflag = true;
			return true;
		}
		if (Engine.MouseClick(PlayBtn, event, window)) {
			loadflag = false;
			return true;
		}

		return false;
	}

	void draw(sf::RenderWindow& window) {
		for (int i = 0; i < 4; i++) { window.draw(options[i]); }
		for (int i = 0; i < 3; i++) { window.draw(money[i]); }
		window.draw(MonopolyLogo);
		window.draw(nbrOfPlayers);
		window.draw(startingCash);
		window.draw(LoadBtn);
		window.draw(PlayBtn);
	}
	~Menu() {
		delete[] options;
		delete[] money;
	}
};