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
#include <fstream>

#include "Menu.h"
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
	bool PlayStarted;
	Menu menu;
	userOptions userAttributes;
public:
	Board();
	~Board();
	void drawboard(sf::RenderWindow& window);
	void drawplayers(sf::RenderWindow& window);
	void addplayers();
	void moveplayermechanicandgraphic(sf::RenderWindow& window);
	void moveplayerprocess(sf::RenderWindow& window, bool goforward);
	void spectatecurrentplayer(sf::RenderWindow& window);
	//--------------------------------------------- Upgrade Panel Doodads----
	void upgradesystem(sf::RenderWindow& window, bool enablebankruptcy);
	void bringupgradepanel(sf::RenderWindow& window);
	void drawstaticpanel(sf::RenderWindow& window);
	void removeupgradepanel(sf::RenderWindow& window);
	void doublerentforutilities();
	void doublerentforstations();
	int  whichpropertyselectedmouse(sf::Vector2i coordinates);
	void updatehighestnumofupgrades();
	void exhibitpropertycard(int propertyID, int& celltoreturn, bool& ispublicprop, sf::RenderWindow& window);
	int  whichoptionselectedcard(sf::Vector2i coordinates);
	void upgradebuttonprocessing(int& selectinput, int propertyID, int cellnum, bool ispublicprop, sf::RenderWindow& window);
	//turn end stuff-------------------------------------
	void processlandedncell(sf::RenderWindow& window);
	void endturn(sf::RenderWindow& window);
	//--------------Bankruptcy system
	void bankruptcyinitiated(sf::RenderWindow& window, int idtoowemoney, int amountowed);
	void changethispropsowner(int playerIDtoremovefrom, int playeridtoaddto, int propID);
	int  convertpropIDtocellnumber(int propID);
	//-----------------------------------Community and Chance
	void drawcardforchanceorcomm(sf::RenderWindow& window, std::string message, bool ischance);
	void getplayershousehotelshopamount(int& numhouses, int& numshops, int& numhotels);
	//---------------------------------------------------- Go and Jail
	void gotojail(sf::RenderWindow& window);
	bool yesnopopup(sf::RenderWindow& window, std::string message);
	void passedgo();
	
	// Auction and Buy thingies
	void buyPropertyCard(int propertyID, sf::RenderWindow& window, int& celltoreturn, bool& ispublicprop, bool bought);
	bool PurchaseProperty(sf::RenderWindow& window, int PropertyID, bool isPublicProp);
	void Auction(sf::RenderWindow& window, int PropertyID);
	// Winning Thingies
	bool haveWin();
	void showWinCard(sf::RenderWindow& window);
	// Saving and Loading
	void SaveToFile();
	void loadfromfile();
	//--------------------------------------
	void addcells();
	void rungame();
	friend class Player;
};

Board::Board() {
	thisplayersturn = 0;
	numberofplayers = 5;//Interface will decide number of players
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
	screenview.setSize(sf::Vector2f(1280, 1024));
	screenview.zoom(1.025f);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (!PlayStarted) {
				PlayStarted = menu.MenuPlay(userAttributes, event, window);
				if (PlayStarted) {
					this->numberofplayers = userAttributes.amountOfPlayers;
					addplayers();
				}
			}
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
				if (PlayStarted) {
					
					case sf::Event::KeyPressed:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
							SaveToFile();
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
							screenview.zoom(0.9f);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
							screenview.zoom(1.1f);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
							this->moveplayermechanicandgraphic(window);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
							this->endturn(window);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
							this->upgradesystem(window,false);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
							this->gotojail(window);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
							this->moveplayerprocess(window, true);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
							this->processlandedncell(window);
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
							this->loadfromfile();
						}
						break;
				}
			}

			
			if (PlayStarted) {
				window.setView(screenview);
				this->spectatecurrentplayer(window);
				this->doublerentforstations();
				this->doublerentforutilities();
				for (int i = 0; i < numberofplayers; i++) {
					if (this->listofplayers[i] != 0) {
						listofplayers[i][0].updatelistofownedgroups();
					}
				}
				updatehighestnumofupgrades();
				if (haveWin()) {
					showWinCard(window);
				}
			}

			

			window.clear(sf::Color::White);
			if (PlayStarted) {
				this->drawboard(window);
				this->drawplayers(window);
			}
			else {
				menu.draw(window);
			}

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
	/*
	for (int i = 1; i < 19; i++) {
		listofplayers[0][0].addpropertyID(i);
	}
	for (int i = 20; i <= 28; i++) {
		listofplayers[1][0].addpropertyID(i);
	}
	*/
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
	static_cast<PrivateProperty*>(listofcells[14])->setPrice(400);
	static_cast<PrivateProperty*>(listofcells[14])->setRent(130);
	static_cast<PrivateProperty*>(listofcells[14])->setGroup("faisal");

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
		if (this->listofplayers[i] != 0) {
			this->listofplayers[i][0].drawplayer(window);
		}
	}
}
void Board::buyPropertyCard(int propertyID, sf::RenderWindow& window, int& cellnumtoreturn, bool& ispublicprop, bool bought) {
	int cellnumber = -1;
	bool publicprop = true;
	if (propertyID != -1) {
		sf::RectangleShape propertyshape(sf::Vector2f(330, 500));
		publicprop = false;
		sf::RectangleShape topbar(sf::Vector2f(330, 80));
		sf::RectangleShape icon(sf::Vector2f(80, 80));
		sf::Image iconimage;
		sf::Texture icongraphic;
		sf::Font font;
		sf::Text text;
		font.loadFromFile("assets/TimesNew.ttf");
		text.setFont(font);
		text.setFillColor(sf::Color::White);
		text.setPosition(105, 102);
		text.setStyle(sf::Text::Bold);
		text.setOutlineThickness(0.5);
		text.setOutlineColor(sf::Color::Black);
		text.setCharacterSize(35);
		propertyshape.setFillColor(sf::Color::White);
		propertyshape.setOutlineThickness(2);
		propertyshape.setOutlineColor(sf::Color::Black);
		propertyshape.setPosition(100, 100);
		topbar.setPosition(100, 100);
		switch (propertyID) {
		case 1:
			cellnumber = 1;
			topbar.setFillColor(sf::Color::Color(166, 83, 32));
			text.setString("Johar Town A");
			break;
		case 3:
			cellnumber = 3;
			topbar.setFillColor(sf::Color::Color(166, 83, 32));
			text.setString("Johar Town B");
			break;
		case 5:
			cellnumber = 5;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Bus Station");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 6:
			cellnumber = 6;
			topbar.setFillColor(sf::Color::Color(9, 96, 172));
			text.setString("Iqbal Town A");
			break;
		case 8:
			cellnumber = 8;
			topbar.setFillColor(sf::Color::Color(9, 96, 172));
			text.setString("Iqbal Town B");
			break;
		case 9:
			cellnumber = 9;
			topbar.setFillColor(sf::Color::Color(9, 96, 172));
			text.setString("Iqbal Town C");
			break;
		case 11:
			cellnumber = 11;
			topbar.setFillColor(sf::Color::Color(240, 134, 8));
			text.setString("Faisal Town A");
			break;
		case 12:
			cellnumber = 12;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("SUI");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		case 13:
			cellnumber = 13;
			topbar.setFillColor(sf::Color::Color(240, 134, 8));
			text.setString("Faisal Town B");
			break;
		case 14:
			cellnumber = 14;
			topbar.setFillColor(sf::Color::Color(240, 134, 8));
			text.setString("Faisal Town C");
			break;
		case 15:
			cellnumber = 15;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Metro Station");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 16:
			cellnumber = 16;
			topbar.setFillColor(sf::Color::Color(34, 24, 116));
			text.setString("Model Town A");
			break;
		case 18:
			cellnumber = 18;
			topbar.setFillColor(sf::Color::Color(34, 24, 116));
			text.setString("Model Town B");
			break;
		case 19:
			cellnumber = 19;
			topbar.setFillColor(sf::Color::Color(34, 24, 116));
			text.setString("Model Town C");
			break;
		case 21:
			cellnumber = 21;
			topbar.setFillColor(sf::Color::Color(200, 202, 0));
			text.setString("Gulberg II A");
			break;
		case 23:
			cellnumber = 23;
			topbar.setFillColor(sf::Color::Color(200, 202, 0));
			text.setString("Gulberg II B");
			break;
		case 24:
			cellnumber = 24;
			topbar.setFillColor(sf::Color::Color(200, 202, 0));
			text.setString("Gulberg II C");
			break;
		case 25:
			cellnumber = 25;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Orange Train");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 26:
			cellnumber = 26;
			topbar.setFillColor(sf::Color::Color(204, 62, 223));
			text.setString("DHA A");
			break;
		case 27:
			cellnumber = 27;
			topbar.setFillColor(sf::Color::Color(204, 62, 223));
			text.setString("DHA B");
			break;
		case 28:
			cellnumber = 28;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("WASA");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		case 29:
			cellnumber = 29;
			topbar.setFillColor(sf::Color::Color(204, 62, 223));
			text.setString("DHA Y");
			break;
		case 31:
			cellnumber = 31;
			topbar.setFillColor(sf::Color::Color(1, 150, 48));
			text.setString("Bahria Town A");
			break;
		case 33:
			cellnumber = 33;
			topbar.setFillColor(sf::Color::Color(1, 150, 48));
			text.setString("Bahria Town B");
			break;
		case 34:
			cellnumber = 34;
			topbar.setFillColor(sf::Color::Color(1, 150, 48));
			text.setString("Bahria Town C");
			break;
		case 35:
			cellnumber = 35;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Railway Station");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 37:
			cellnumber = 37;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("LESCO");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		case 39:
			cellnumber = 39;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("PTCL");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		}
		this->drawstaticpanel(window);
		window.draw(propertyshape);
		window.draw(topbar);
		window.draw(text);
		text.setCharacterSize(25);
		text.setFillColor(sf::Color::Black);
		text.setOutlineColor(sf::Color::White);
		text.setPosition(105, 175);
		text.setString("Rent Amount:");
		window.draw(text);
		if (publicprop == true) {
			text.setString(std::to_string(static_cast<PublicProperty*>(listofcells[cellnumber])->getRentpublic()) + " PKR");
		}
		else {
			if (this->listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(propertyID)) {
				if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getanyupgrades()) {
					text.setString(std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getprivaterent()) + " PKR");
				}
				else {
					text.setString(std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getprivaterent() * 2) + " PKR");
				}
			}
			else {
				text.setString(std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getprivaterent()) + " PKR");
			}
		}
		text.setPosition(295, 175);
		window.draw(text);
		text.setString("Mortgage Value:");
		text.setPosition(105, 205);
		window.draw(text);
		text.setString(std::to_string(static_cast<Property*>(listofcells[cellnumber])->getMortgage()) + " PKR");
		text.setPosition(295, 205);
		window.draw(text);
		text.setString("Purchase Price:");
		text.setPosition(105, 235);
		window.draw(text);
		text.setString(std::to_string(static_cast<Property*>(listofcells[cellnumber])->getPrice()) + " PKR");
		text.setPosition(295, 235);
		window.draw(text);
		if (publicprop) {
			text.setString("Double Rent:");
			text.setPosition(105, 265);
			window.draw(text);
			bool flag = static_cast<PublicProperty*>(listofcells[cellnumber])->getDoubleRent();
			if (flag) {
				text.setString("Yes");
			}
			else {
				text.setString("No");
			}
			text.setPosition(295, 265);
			window.draw(text);
			icongraphic.loadFromImage(iconimage);
			icon.setTexture(&icongraphic);
			icon.setPosition(350, 100);
			window.draw(icon);
		}
		else {
			iconimage.loadFromFile("assets/upgrade/property-icon.png");
			icongraphic.loadFromImage(iconimage);
			icon.setTexture(&icongraphic);
			icon.setPosition(350, 100);
			window.draw(icon);
		}
		icon.setTexture(0);
		icon.setSize(sf::Vector2f(120, 50));
		icon.setOutlineColor(sf::Color::Black);
		icon.setOutlineThickness(2);
		icon.setPosition(310, 550);
		window.draw(icon);
		if (static_cast<Property*>(listofcells[cellnumber])->getmortgageflag() == false) {
			if (bought) {
				text.setString("Owned");
				text.setCharacterSize(20);
				text.setPosition(317, 560);
			}
			else {
				text.setString("Purchase");
				text.setCharacterSize(20);
				text.setPosition(317, 560);
			}

			window.draw(text);
			text.setCharacterSize(25);

		}
		window.display();
	}
	ispublicprop = publicprop;
	cellnumtoreturn = cellnumber;
}
bool Board::PurchaseProperty(sf::RenderWindow& window, int propertyID, bool ispublicprop) {
	sf::Event event;
	sf::Vector2i mousecoords(0, 0);
	bool exitflag = false;
	bool playerBought = false;
	
	int sequenceofevents = 1;
	int maincellnumber = -1;
	int propertyIDselected = -1;
	int upgradeselected = -1;
	this->bringupgradepanel(window);
	this->drawstaticpanel(window);
	window.display();
	propertyIDselected = propertyID;
	bool owned;
	while (!exitflag) {
		if (static_cast<Property*>(listofcells[propertyID])->getPlayerID() != -1) {
			owned = true;
			playerBought = true;
			buyPropertyCard(propertyIDselected, window, maincellnumber, ispublicprop, true);
		}
		else {
			owned = false;
			buyPropertyCard(propertyIDselected, window, maincellnumber, ispublicprop, false);
		}
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (!owned) {
						mousecoords = sf::Mouse::getPosition(window);
						upgradeselected = whichoptionselectedcard(mousecoords);
						if (upgradeselected == 10) {
							this->listofplayers[thisplayersturn][0].addpropertyID(static_cast<Property*>(listofcells[propertyID])->getPropert_ID());
							this->listofplayers[thisplayersturn]->withdrawcash(static_cast<Property*>(listofcells[propertyID])->getPrice());
							static_cast<Property*>(listofcells[propertyID])->setPlayerID(this->listofplayers[thisplayersturn]->getPlayerID());
							playerBought = true;
						}
					}
					else {
						playerBought = true;
					}
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					exitflag = true;
				}
				break;
			}
		}
	}
	this->removeupgradepanel(window);
	return playerBought;
}
void Board::Auction(sf::RenderWindow& window, int propertyID) {
	sf::Event event;
	sf::Vector2f mousecoords(0, 0);

	bool exitFlag = false;

	int currentPlayers = 0;

	for (int i = 0; i < numberofplayers; i++) if (listofplayers[i] != 0) currentPlayers++;

	float AuctionCardWidth = currentPlayers * 200;

	

	sf::RectangleShape AuctionCard;
	AuctionCard.setPosition(250, 222);
	AuctionCard.setSize(sf::Vector2f(AuctionCardWidth, 400));
	AuctionCard.setOutlineThickness(3);
	AuctionCard.setFillColor(sf::Color::White);
	AuctionCard.setOutlineColor(sf::Color::Black);

	sf::RectangleShape* Call = new sf::RectangleShape[currentPlayers];
	sf::RectangleShape* Fold = new sf::RectangleShape[currentPlayers];
	sf::Text* CurrentBid = new sf::Text[currentPlayers];
	bool* Folds = new bool[currentPlayers];

	sf::Font font;
	font.loadFromFile("assets/TimesNew.ttf");

	sf::Text PlayerNumber;
	PlayerNumber.setFont(font);
	PlayerNumber.setString("Player ");
	PlayerNumber.setFillColor(sf::Color::Black);

	sf::Texture FoldTexture;
	FoldTexture.loadFromFile("assets/auction/fold.png");

	sf::Texture CallTexture;
	CallTexture.loadFromFile("assets/auction/call.png");

	sf::RectangleShape* AuctionSection = new sf::RectangleShape[currentPlayers];

	std::string Price = "0";

	for (int i = 0; i < currentPlayers; i++) {
		Folds[i] = false;

		AuctionSection[i].setPosition(250 + 200 * i, 222);
		AuctionSection[i].setSize(sf::Vector2f(AuctionCardWidth / currentPlayers, 400));
		AuctionSection[i].setOutlineThickness(3);
		AuctionSection[i].setFillColor(sf::Color::White);
		AuctionSection[i].setOutlineColor(sf::Color::Black);

		Call[i].setPosition(280 + 200 * i, 480);
		Call[i].setSize(sf::Vector2f(130, 50));
		Call[i].setTexture(&CallTexture);

		Fold[i].setPosition(280 + 200 * i, 550);
		Fold[i].setSize(sf::Vector2f(130, 50));
		Fold[i].setTexture(&FoldTexture);

		CurrentBid[i].setFont(font);
		CurrentBid[i].setString(Price);
		CurrentBid[i].setPosition(330 + 200 * i, 350);
		CurrentBid[i].setFillColor(sf::Color::Black);
	}


	while (!exitFlag) {
		mousecoords.x = sf::Mouse::getPosition(window).x;
		mousecoords.y = sf::Mouse::getPosition(window).y;
		while (window.pollEvent(event)) {
			int foldCount = 0;
			int maxBidPlayer = 0;
			int maxBid = 0;

			for (int i = 0; i < currentPlayers; i++) { if (Folds[i]) { foldCount++; } }
			if (foldCount == currentPlayers - 1) {

				for (int i = 0; i < currentPlayers; i++) {
					std::string PlayerBid = CurrentBid[i].getString();
					if (std::stoi(PlayerBid) > maxBid) {
						maxBid = std::stoi(PlayerBid);
						maxBidPlayer = i;
					}
				}

				listofplayers[maxBidPlayer]->withdrawcash(maxBid);
				listofplayers[maxBidPlayer]->addpropertyID(static_cast<Property*>(listofcells[propertyID])->getPropert_ID());
				static_cast<Property*>(listofcells[propertyID])->setPlayerID(this->listofplayers[maxBidPlayer]->getPlayerID());
				exitFlag = true;

			}
			switch (event.type) {
			case sf::Event::MouseButtonPressed:


				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					for (int i = 0; i < currentPlayers; i++) {

						if (!Folds[i]) {
							if (Call[i].getGlobalBounds().contains(mousecoords)) {
								std::string currentBidPrice = CurrentBid[i].getString();
								int newPrice = std::stoi(currentBidPrice) + 50;
								if (newPrice <= listofplayers[i]->getbalance()) {
									CurrentBid[i].setString(std::to_string(newPrice));
								}
							}
						}

						if (Fold[i].getGlobalBounds().contains(mousecoords)) {
							Folds[i] = true;
						}
					}
				}
				break;
			}
		}

		window.draw(AuctionCard);
		for (int i = 0; i < currentPlayers; i++) {
			window.draw(AuctionSection[i]);
			window.draw(Fold[i]);
			window.draw(Call[i]);
			window.draw(CurrentBid[i]);
			
			PlayerNumber.setString(PlayerNumber.getString() + std::to_string(i + 1));
			PlayerNumber.setPosition(300 + 200 * i, 300);
			window.draw(PlayerNumber);
			PlayerNumber.setString("Player ");	
		}

		window.display();
	}
}
void Board::spectatecurrentplayer(sf::RenderWindow& window) {
	screenview.setCenter(this->listofplayers[this->thisplayersturn][0].getplayercoordinates());
	window.setView(screenview);
}
void Board::moveplayerprocess(sf::RenderWindow& window, bool goforward) {
	this->spectatecurrentplayer(window);
	sf::RectangleShape tempplayershape = listofplayers[thisplayersturn][0].getplayershape();
	int currentcell = listofplayers[thisplayersturn][0].getcurrentlyoncell();
	float amounttoshiftx = 0;
	float amounttoshifty = 0;
	if (goforward) {
		currentcell++;
	}
	else {
		currentcell--;
	}
	if (currentcell >= 1 && currentcell <= 11) {
		amounttoshiftx = -1.50;
		amounttoshifty = 0;
	}
	else {
		if (currentcell >= 11 && currentcell <= 21) {
			amounttoshiftx = 0;
			amounttoshifty = -1.50;
		}
		else {
			if (currentcell >= 21 && currentcell <= 31) {
				amounttoshiftx = 1.50;
				amounttoshifty = 0;
			}
			else {
				if (currentcell >= 31 && currentcell <= 41) {
					amounttoshiftx = 0;
					amounttoshifty = 1.50;
					if (currentcell == 41) {
						currentcell = 1;
					}
				}
			}
		}
	}
	if (!goforward) {
		amounttoshiftx *= -1;
		amounttoshifty *= -1;
	}
	for (float j = 1; j <= 1.75; j += 0.005) {
		listofplayers[thisplayersturn][0].getplayershape().setScale(j, j);
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		window.display();
	}
	for (int j = 0; j < 100; j++) {
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
	listofplayers[thisplayersturn][0].updateplayercoordinates(tempplayershape.getPosition());
	this->spectatecurrentplayer(window);
	listofplayers[thisplayersturn][0].setcurrentcell(currentcell);
}
void Board::moveplayermechanicandgraphic(sf::RenderWindow& window) {
	screenview.setCenter(500, -300);
	window.setView(screenview);
	if (this->listofplayers[thisplayersturn][0].getturnsinjail() == 0) {
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
			this->moveplayerprocess(window, true);
			if (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() == 1) {
				passedgo();
			}
		}
	}
	else {
		this->listofplayers[thisplayersturn][0].spendturninjail();
		if (this->yesnopopup(window, "Press Left Click to (Pay 400PKR Fee/Use GOJF card) to get released or Right Click to not do so")) {
			if (this->listofplayers[thisplayersturn][0].removeGOJFcard()) {
				this->listofplayers[thisplayersturn][0].setturnsinjail(0);
				this->yesnopopup(window, "You have used 1 GOJF card, Proceed with turn");
				this->moveplayermechanicandgraphic(window);
			}
			else {
				if (this->listofplayers[thisplayersturn][0].withdrawcash(400)) {
					this->listofplayers[thisplayersturn][0].setturnsinjail(0);
					this->yesnopopup(window, "You have payed 400PKR, Proceed with turn");
					this->moveplayermechanicandgraphic(window);
				}
				else {
					this->yesnopopup(window, "You possess neither GOJF cards, nor enough cash");
				}
			}
		}
		else {
		}
		for (int timebuffer = 0; timebuffer < 100000000; timebuffer++) {
			for (int timebuffer2 = 0; timebuffer2 < 5; timebuffer2++) {

			}
		}
	}
}
void Board::endturn(sf::RenderWindow& window) {
	this->thisplayersturn++;
	if (this->thisplayersturn >= numberofplayers) {
		this->thisplayersturn = 0;
	}
	while (this->listofplayers[thisplayersturn] == 0) {
		if (this->thisplayersturn >= numberofplayers) {
			this->thisplayersturn = 0;
		}
		this->thisplayersturn++;
		if (this->thisplayersturn >= numberofplayers) {
			this->thisplayersturn = 0;
		}
	}
}
void Board::upgradesystem(sf::RenderWindow& window, bool bankruptcyenabled) {
	sf::Event event;
	sf::Vector2i mousecoords(0, 0);
	sf::Font font;
	sf::Text text;
	sf::Text text2;
	bool gobackflag = false;
	font.loadFromFile("assets/TimesNew.ttf");
	text.setFont(font);
	text.setString("Sell GOJF Card");
	text.setPosition(530, 55);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	text.setOutlineColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOutlineThickness(0.5);

	text2.setFont(font);
	text2.setString("Go Bankrupt");
	text2.setPosition(285, 55);
	text2.setCharacterSize(20);
	text2.setFillColor(sf::Color::Black);
	text2.setOutlineColor(sf::Color::White);
	text2.setStyle(sf::Text::Bold);
	text2.setOutlineThickness(0.5);
	sf::RectangleShape shape(sf::Vector2f(200, 40));
	shape.setPosition(500, 50);
	shape.setFillColor(sf::Color::White);
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::Black);
	sf::RectangleShape shape2(sf::Vector2f(200, 40));
	shape2.setPosition(250, 50);
	shape2.setFillColor(sf::Color::White);
	shape2.setOutlineThickness(1);
	shape2.setOutlineColor(sf::Color::Black);
	bool exitflag = false;
	bool ispublicprop = false;
	int sequenceofevents = 1;
	int maincellnumber = -1;
	int propertyIDselected = -1;
	int upgradeselected = -1;
	this->bringupgradepanel(window);
	this->drawstaticpanel(window);
	window.draw(shape);
	window.draw(text);
	if (bankruptcyenabled) {
		window.draw(shape2);
		window.draw(text2);
	}
	window.display();
	while (!exitflag) {
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					mousecoords = sf::Mouse::getPosition(window);
					switch (sequenceofevents) {
					case 1:
						propertyIDselected = whichpropertyselectedmouse(mousecoords);
						exhibitpropertycard(propertyIDselected, maincellnumber, ispublicprop, window);
						if (propertyIDselected != -1) { sequenceofevents++; }
						if ((mousecoords.x >= 500 && mousecoords.x <= 700) && (mousecoords.y >= 60 && mousecoords.y <= 100)) {
							if (listofplayers[thisplayersturn][0].removeGOJFcard()) {
								listofplayers[thisplayersturn][0].depositcash(400);
								this->drawstaticpanel(window);
								window.draw(shape);
								window.draw(text);
								if (bankruptcyenabled) {
									window.draw(shape2);
									window.draw(text2);
								}
								window.display();
							}
							else {
								
							}
						}
						if (bankruptcyenabled) {
							if ((mousecoords.x >= 260 && mousecoords.x <= 460) && (mousecoords.y >= 60 && mousecoords.y <= 100)) {
								this->listofplayers[thisplayersturn][0].setbankruptflag(1);
								exitflag = true;
							}
						}
						break;
					case 2:
						upgradeselected = whichoptionselectedcard(mousecoords);
						int tradingseer = upgradeselected;
						updatehighestnumofupgrades();
						upgradebuttonprocessing(upgradeselected, propertyIDselected, maincellnumber, ispublicprop, window);
						int propIDfortradingsystem = propertyIDselected;
						updatehighestnumofupgrades();
						exhibitpropertycard(propertyIDselected, maincellnumber, ispublicprop, window);
						if (tradingseer == 11) {
							int counter = 0;
							bool tradingloopexit = false;
							int selectedplayer = -1;
							sf::RectangleShape selectionshape(sf::Vector2f(100, 60));
							selectionshape.setFillColor(sf::Color::White);
							selectionshape.setOutlineThickness(1);
							selectionshape.setOutlineColor(sf::Color::Black);
							sf::Text playertext;
							playertext.setFont(font);
							playertext.setCharacterSize(20);
							playertext.setFillColor(sf::Color::Black);
							playertext.setOutlineColor(sf::Color::White);
							playertext.setStyle(sf::Text::Bold);
							playertext.setOutlineThickness(0.5);
							for (int i = 0; i < numberofplayers; i++) {
								if (listofplayers[i] != 0) {
									if (i != thisplayersturn) {
										playertext.setString("Player " + std::to_string(i + 1));
									}
									else {
										playertext.setString("You");
									}
								}
								else {
									playertext.setString("Bankrupt");
								}
								playertext.setPosition(465 + i * 100, 230);
								selectionshape.setPosition(450 + i * 100, 200);
								window.draw(selectionshape);
								window.draw(playertext);
								if (i == numberofplayers - 1) {
									selectionshape.setSize(sf::Vector2f(100 * i + 100, 30));
									selectionshape.setPosition(450, 170);
									window.draw(selectionshape);
									playertext.setString("Select a player");
									playertext.setPosition(450+i*40, 170);
									window.draw(playertext);
								}
							}
							window.display();
							while (!tradingloopexit) {
								while (window.pollEvent(event)) {
									switch (event.type) {
									case sf::Event::MouseButtonPressed:
										if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
											mousecoords = sf::Mouse::getPosition(window);
											for (int i = 0; i < numberofplayers; i++) {
												if (listofplayers[i] != 0) {
													if ((mousecoords.x >= 450 + i * 100 && mousecoords.x <= 450 + i * 100 + 100) && (mousecoords.y >= 205 && mousecoords.y <= 265)) {
														selectedplayer = i;
														if (selectedplayer != thisplayersturn) {
															tradingloopexit = true;
															break;
														}
													}
												}
											}
										}
										if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
											selectedplayer = -1;
											tradingloopexit = true;
										}
									}
								}
							}
							if (selectedplayer != -1) {
								if (yesnopopup(window, "Player " + std::to_string(selectedplayer + 1) + ", Left Clk: Accept offer to purchase property for " + std::to_string(static_cast<Property*>(listofcells[convertpropIDtocellnumber(propIDfortradingsystem)])->getPrice()) + " PKR, Right Clk: Decline offer")) {
									if (listofplayers[selectedplayer][0].withdrawcash(static_cast<Property*>(listofcells[convertpropIDtocellnumber(propIDfortradingsystem)])->getPrice())) {
										yesnopopup(window, "Transaction complete, Property and cash exchanged");
										listofplayers[thisplayersturn][0].depositcash(static_cast<Property*>(listofcells[convertpropIDtocellnumber(propIDfortradingsystem)])->getPrice());
										changethispropsowner(thisplayersturn, selectedplayer, propIDfortradingsystem);
									}
									else {
										yesnopopup(window, "Player " + std::to_string(selectedplayer + 1) + ", you can't afford mentioned property");
									}
									gobackflag = true;
								}
								else {

								}
							}
						}
						if (upgradeselected != -1) {}
						break;
					}
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right) ||gobackflag==true) {
					gobackflag = false;
					sequenceofevents--;
					switch (sequenceofevents) {
					case 0:
						exitflag = true;
						break;
					case 1:
						this->drawstaticpanel(window);
						window.draw(shape);
						window.draw(text);
						if (bankruptcyenabled) {
							window.draw(shape2);
							window.draw(text2);
						}
						window.display();
						break;
					}
				}
				break;
			}
		}
	}
	this->removeupgradepanel(window);
}
void Board::bringupgradepanel(sf::RenderWindow& window) {
	float amounttoshiftx = -72.5;
	float amountoshifty = 0;
	sf::RectangleShape panel(sf::Vector2f(600, 1100));
	sf::RectangleShape upgradeshape(sf::Vector2f(135, 130));
	sf::Image upgradeimage;
	sf::Texture upgradetexture;
	std::string filepath = "";
	panel.setFillColor(sf::Color::Color(65, 73, 80));
	panel.setPosition(1350, -50);
	panel.setOutlineThickness(1);
	panel.setOutlineColor(sf::Color::Black);
	screenview.setCenter(640, 512);
	window.setView(screenview);
	for (int gradualloader = 0; gradualloader < 10; gradualloader++) {
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		window.draw(panel);
		for (int rows = 0, filepathcounter = 1; rows < 7; rows++) {
			for (int cols = 0; cols < 4; cols++, filepathcounter++) {
				if (listofplayers[thisplayersturn][0].propertyowned(filepathcounter)) {
					switch (filepathcounter) {
					case 1:
						filepath = "assets/Textures/Cell2.png";
						break;
					case 2:
						filepath = "assets/Textures/Cell4.png";
						break;
					case 3:
						filepath = "assets/Textures/Cell6.png";
						break;
					case 4:
						filepath = "assets/Textures/Cell7.png";
						break;
					case 5:
						filepath = "assets/Textures/Cell9.png";
						break;
					case 6:
						filepath = "assets/Textures/Cell10.png";
						break;
					case 7:
						filepath = "assets/Textures/Cell12.png";
						break;
					case 8:
						filepath = "assets/Textures/Cell13.png";
						break;
					case 9:
						filepath = "assets/Textures/Cell14.png";
						break;
					case 10:
						filepath = "assets/Textures/Cell15.png";
						break;
					case 11:
						filepath = "assets/Textures/Cell16.png";
						break;
					case 12:
						filepath = "assets/Textures/Cell17.png";
						break;
					case 13:
						filepath = "assets/Textures/Cell19.png";
						break;
					case 14:
						filepath = "assets/Textures/Cell20.png";
						break;
					case 15:
						filepath = "assets/Textures/Cell22.png";
						break;
					case 16:
						filepath = "assets/Textures/Cell24.png";
						break;
					case 17:
						filepath = "assets/Textures/Cell25.png";
						break;
					case 18:
						filepath = "assets/Textures/Cell26.png";
						break;
					case 19:
						filepath = "assets/Textures/Cell27.png";
						break;
					case 20:
						filepath = "assets/Textures/Cell28.png";
						break;
					case 21:
						filepath = "assets/Textures/Cell29.png";
						break;
					case 22:
						filepath = "assets/Textures/Cell30.png";
						break;
					case 23:
						filepath = "assets/Textures/Cell32.png";
						break;
					case 24:
						filepath = "assets/Textures/Cell34.png";
						break;
					case 25:
						filepath = "assets/Textures/Cell35.png";
						break;
					case 26:
						filepath = "assets/Textures/Cell36.png";
						break;
					case 27:
						filepath = "assets/Textures/Cell38.png";
						break;
					case 28:
						filepath = "assets/Textures/Cell40.png";
						break;
					}
					upgradeimage.loadFromFile(filepath);
					upgradetexture.loadFromImage(upgradeimage);
					upgradeshape.setTexture(&upgradetexture);
					upgradeshape.setPosition(panel.getPosition().x + 145 * cols + 10, panel.getPosition().y + 140 * rows + 100);
					window.draw(upgradeshape);
				}
				else {
					for (int timebuffer = 0; timebuffer < 400000; timebuffer++);
				}
			}
		}
		window.display();
		panel.move(amounttoshiftx, amountoshifty);
	}
}
void Board::removeupgradepanel(sf::RenderWindow& window) {
	float amounttoshiftx = 72.5;
	float amountoshifty = 0;
	sf::RectangleShape panel(sf::Vector2f(600, 1100));
	sf::RectangleShape upgradeshape(sf::Vector2f(135, 130));
	sf::Image upgradeimage;
	sf::Texture upgradetexture;
	std::string filepath = "";
	panel.setFillColor(sf::Color::Color(65, 73, 80));
	panel.setPosition(710, -50);
	panel.setOutlineThickness(1);
	panel.setOutlineColor(sf::Color::Black);
	screenview.setCenter(640, 512);
	window.setView(screenview);
	for (int gradualloader = 0; gradualloader < 10; gradualloader++) {
		window.clear(sf::Color::White);
		this->drawboard(window);
		this->drawplayers(window);
		window.draw(panel);
		for (int rows = 0, filepathcounter = 1; rows < 7; rows++) {
			for (int cols = 0; cols < 4; cols++, filepathcounter++) {
				if (listofplayers[thisplayersturn][0].propertyowned(filepathcounter)) {
					switch (filepathcounter) {
					case 1:
						filepath = "assets/Textures/Cell2.png";
						break;
					case 2:
						filepath = "assets/Textures/Cell4.png";
						break;
					case 3:
						filepath = "assets/Textures/Cell6.png";
						break;
					case 4:
						filepath = "assets/Textures/Cell7.png";
						break;
					case 5:
						filepath = "assets/Textures/Cell9.png";
						break;
					case 6:
						filepath = "assets/Textures/Cell10.png";
						break;
					case 7:
						filepath = "assets/Textures/Cell12.png";
						break;
					case 8:
						filepath = "assets/Textures/Cell13.png";
						break;
					case 9:
						filepath = "assets/Textures/Cell14.png";
						break;
					case 10:
						filepath = "assets/Textures/Cell15.png";
						break;
					case 11:
						filepath = "assets/Textures/Cell16.png";
						break;
					case 12:
						filepath = "assets/Textures/Cell17.png";
						break;
					case 13:
						filepath = "assets/Textures/Cell19.png";
						break;
					case 14:
						filepath = "assets/Textures/Cell20.png";
						break;
					case 15:
						filepath = "assets/Textures/Cell22.png";
						break;
					case 16:
						filepath = "assets/Textures/Cell24.png";
						break;
					case 17:
						filepath = "assets/Textures/Cell25.png";
						break;
					case 18:
						filepath = "assets/Textures/Cell26.png";
						break;
					case 19:
						filepath = "assets/Textures/Cell27.png";
						break;
					case 20:
						filepath = "assets/Textures/Cell28.png";
						break;
					case 21:
						filepath = "assets/Textures/Cell29.png";
						break;
					case 22:
						filepath = "assets/Textures/Cell30.png";
						break;
					case 23:
						filepath = "assets/Textures/Cell32.png";
						break;
					case 24:
						filepath = "assets/Textures/Cell34.png";
						break;
					case 25:
						filepath = "assets/Textures/Cell35.png";
						break;
					case 26:
						filepath = "assets/Textures/Cell36.png";
						break;
					case 27:
						filepath = "assets/Textures/Cell38.png";
						break;
					case 28:
						filepath = "assets/Textures/Cell40.png";
						break;
					}
					upgradeimage.loadFromFile(filepath);
					upgradetexture.loadFromImage(upgradeimage);
					upgradeshape.setTexture(&upgradetexture);
					upgradeshape.setPosition(panel.getPosition().x + 145 * cols + 10, panel.getPosition().y + 140 * rows + 100);
					window.draw(upgradeshape);
				}
				else {
					for (int timebuffer = 0; timebuffer < 400000; timebuffer++);
				}
			}
		}
		window.display();
		panel.move(amounttoshiftx, amountoshifty);
	}
	for (int timebuffer = 0; timebuffer < 60000000; timebuffer++) {
		for (int timebuffer2 = 0; timebuffer2 < 2; timebuffer2++) {

		}
	}
}
void Board::drawstaticpanel(sf::RenderWindow& window) {
	sf::RectangleShape panel(sf::Vector2f(600, 1100));
	sf::Image upgradeimage;
	sf::RectangleShape upgradeshape(sf::Vector2f(135, 130));
	sf::Texture upgradetexture;
	std::string filepath = "";
	sf::Font font;
	sf::Text text;
	font.loadFromFile("assets/TimesNew.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setString("Player " + std::to_string(thisplayersturn + 1) + "'s Properties");
	text.setPosition(715, -12);
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(38);
	text.setOutlineThickness(0.5);
	text.setOutlineColor(sf::Color::Black);
	panel.setFillColor(sf::Color::Color(65, 73, 80));
	panel.setPosition(710, -50);
	panel.setOutlineThickness(1);
	panel.setOutlineColor(sf::Color::Black);
	window.clear(sf::Color::White);
	this->drawboard(window);
	this->drawplayers(window);
	window.draw(panel);
	window.draw(text);
	text.setPosition(1070, -12);
	text.setString("Funds:");
	window.draw(text);
	text.setPosition(1185, -6);
	text.setString(std::to_string(listofplayers[thisplayersturn][0].getbalance()));
	text.setFillColor(sf::Color::Green);
	text.setCharacterSize(32);
	window.draw(text);
	for (int rows = 0, filepathcounter = 1; rows < 7; rows++) {
		for (int cols = 0; cols < 4; cols++, filepathcounter++) {
			if (listofplayers[thisplayersturn][0].propertyowned(filepathcounter)) {
				switch (filepathcounter) {
				case 1:
					filepath = "assets/Textures/Cell2.png";
					break;
				case 2:
					filepath = "assets/Textures/Cell4.png";
					break;
				case 3:
					filepath = "assets/Textures/Cell6.png";
					break;
				case 4:
					filepath = "assets/Textures/Cell7.png";
					break;
				case 5:
					filepath = "assets/Textures/Cell9.png";
					break;
				case 6:
					filepath = "assets/Textures/Cell10.png";
					break;
				case 7:
					filepath = "assets/Textures/Cell12.png";
					break;
				case 8:
					filepath = "assets/Textures/Cell13.png";
					break;
				case 9:
					filepath = "assets/Textures/Cell14.png";
					break;
				case 10:
					filepath = "assets/Textures/Cell15.png";
					break;
				case 11:
					filepath = "assets/Textures/Cell16.png";
					break;
				case 12:
					filepath = "assets/Textures/Cell17.png";
					break;
				case 13:
					filepath = "assets/Textures/Cell19.png";
					break;
				case 14:
					filepath = "assets/Textures/Cell20.png";
					break;
				case 15:
					filepath = "assets/Textures/Cell22.png";
					break;
				case 16:
					filepath = "assets/Textures/Cell24.png";
					break;
				case 17:
					filepath = "assets/Textures/Cell25.png";
					break;
				case 18:
					filepath = "assets/Textures/Cell26.png";
					break;
				case 19:
					filepath = "assets/Textures/Cell27.png";
					break;
				case 20:
					filepath = "assets/Textures/Cell28.png";
					break;
				case 21:
					filepath = "assets/Textures/Cell29.png";
					break;
				case 22:
					filepath = "assets/Textures/Cell30.png";
					break;
				case 23:
					filepath = "assets/Textures/Cell32.png";
					break;
				case 24:
					filepath = "assets/Textures/Cell34.png";
					break;
				case 25:
					filepath = "assets/Textures/Cell35.png";
					break;
				case 26:
					filepath = "assets/Textures/Cell36.png";
					break;
				case 27:
					filepath = "assets/Textures/Cell38.png";
					break;
				case 28:
					filepath = "assets/Textures/Cell40.png";
					break;
				}
				upgradeimage.loadFromFile(filepath);
				upgradetexture.loadFromImage(upgradeimage);
				upgradeshape.setTexture(&upgradetexture);
				upgradeshape.setPosition(panel.getPosition().x + 145 * cols + 10, panel.getPosition().y + 140 * rows + 100);
				window.draw(upgradeshape);
			}
			else {
				for (int timebuffer = 0; timebuffer < 400000; timebuffer++);
			}
		}
	}
}
int  Board::whichpropertyselectedmouse(sf::Vector2i coordinates) {
	float topleftofblockx = 0;
	float topleftofblocky = 0;
	for (int rows = 0, filepathcounter = 1; rows < 7; rows++) {
		for (int cols = 0; cols < 4; cols++, filepathcounter++) {
			if (listofplayers[thisplayersturn][0].propertyowned(filepathcounter)) {
				topleftofblockx = 710 + 145 * cols + 10;
				topleftofblocky = -50 + 140 * rows + 100;
				if ((coordinates.x > topleftofblockx&& coordinates.x < topleftofblockx + 135) && (coordinates.y > topleftofblocky&& coordinates.y < topleftofblocky + 130)) {
					return filepathcounter;
				}
			}
		}
	}
	return -1;
}
void Board::exhibitpropertycard(int propertyID, int& cellnumtoreturn, bool& ispublicprop, sf::RenderWindow& window) {
	int cellnumber = -1;
	bool publicprop = true;
	if (propertyID != -1) {
		sf::RectangleShape propertyshape(sf::Vector2f(330, 500));
		publicprop = false;
		sf::RectangleShape topbar(sf::Vector2f(330, 80));
		sf::RectangleShape icon(sf::Vector2f(80, 80));
		sf::Image iconimage;
		sf::Texture icongraphic;
		sf::Font font;
		sf::Text text;
		font.loadFromFile("assets/TimesNew.ttf");
		text.setFont(font);
		text.setFillColor(sf::Color::White);
		text.setPosition(105, 102);
		text.setStyle(sf::Text::Bold);
		text.setOutlineThickness(0.5);
		text.setOutlineColor(sf::Color::Black);
		text.setCharacterSize(35);
		propertyshape.setFillColor(sf::Color::White);
		propertyshape.setOutlineThickness(2);
		propertyshape.setOutlineColor(sf::Color::Black);
		propertyshape.setPosition(100, 100);
		topbar.setPosition(100, 100);
		switch (propertyID) {
		case 1:
			cellnumber = 1;
			topbar.setFillColor(sf::Color::Color(166, 83, 32));
			text.setString("Johar Town A");
			break;
		case 2:
			cellnumber = 3;
			topbar.setFillColor(sf::Color::Color(166, 83, 32));
			text.setString("Johar Town B");
			break;
		case 3:
			cellnumber = 5;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Bus Station");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 4:
			cellnumber = 6;
			topbar.setFillColor(sf::Color::Color(9, 96, 172));
			text.setString("Iqbal Town A");
			break;
		case 5:
			cellnumber = 8;
			topbar.setFillColor(sf::Color::Color(9, 96, 172));
			text.setString("Iqbal Town B");
			break;
		case 6:
			cellnumber = 9;
			topbar.setFillColor(sf::Color::Color(9, 96, 172));
			text.setString("Iqbal Town C");
			break;
		case 7:
			cellnumber = 11;
			topbar.setFillColor(sf::Color::Color(240, 134, 8));
			text.setString("Faisal Town A");
			break;
		case 8:
			cellnumber = 12;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("SUI");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		case 9:
			cellnumber = 13;
			topbar.setFillColor(sf::Color::Color(240, 134, 8));
			text.setString("Faisal Town B");
			break;
		case 10:
			cellnumber = 14;
			topbar.setFillColor(sf::Color::Color(240, 134, 8));
			text.setString("Faisal Town C");
			break;
		case 11:
			cellnumber = 15;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Metro Station");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 12:
			cellnumber = 16;
			topbar.setFillColor(sf::Color::Color(34, 24, 116));
			text.setString("Model Town A");
			break;
		case 13:
			cellnumber = 18;
			topbar.setFillColor(sf::Color::Color(34, 24, 116));
			text.setString("Model Town B");
			break;
		case 14:
			cellnumber = 19;
			topbar.setFillColor(sf::Color::Color(34, 24, 116));
			text.setString("Model Town C");
			break;
		case 15:
			cellnumber = 21;
			topbar.setFillColor(sf::Color::Color(200, 202, 0));
			text.setString("Gulberg II A");
			break;
		case 16:
			cellnumber = 23;
			topbar.setFillColor(sf::Color::Color(200, 202, 0));
			text.setString("Gulberg II B");
			break;
		case 17:
			cellnumber = 24;
			topbar.setFillColor(sf::Color::Color(200, 202, 0));
			text.setString("Gulberg II C");
			break;
		case 18:
			cellnumber = 25;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Orange Train");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 19:
			cellnumber = 26;
			topbar.setFillColor(sf::Color::Color(204, 62, 223));
			text.setString("DHA A");
			break;
		case 20:
			cellnumber = 27;
			topbar.setFillColor(sf::Color::Color(204, 62, 223));
			text.setString("DHA B");
			break;
		case 21:
			cellnumber = 28;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("WASA");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		case 22:
			cellnumber = 29;
			topbar.setFillColor(sf::Color::Color(204, 62, 223));
			text.setString("DHA Y");
			break;
		case 23:
			cellnumber = 31;
			topbar.setFillColor(sf::Color::Color(1, 150, 48));
			text.setString("Bahria Town A");
			break;
		case 24:
			cellnumber = 33;
			topbar.setFillColor(sf::Color::Color(1, 150, 48));
			text.setString("Bahria Town B");
			break;
		case 25:
			cellnumber = 34;
			topbar.setFillColor(sf::Color::Color(1, 150, 48));
			text.setString("Bahria Town C");
			break;
		case 26:
			cellnumber = 35;
			topbar.setFillColor(sf::Color::Color(61, 68, 76));
			text.setString("Railway Station");
			iconimage.loadFromFile("assets/upgrade/Public.png");
			publicprop = true;
			break;
		case 27:
			cellnumber = 37;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("LESCO");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		case 28:
			cellnumber = 39;
			topbar.setFillColor(sf::Color::Color(148, 149, 153));
			text.setString("PTCL");
			iconimage.loadFromFile("assets/upgrade/Utility.png");
			publicprop = true;
			break;
		}
		this->drawstaticpanel(window);
		window.draw(propertyshape);
		window.draw(topbar);
		window.draw(text);
		text.setCharacterSize(25);
		text.setFillColor(sf::Color::Black);
		text.setOutlineColor(sf::Color::White);
		text.setPosition(105, 175);
		text.setString("Rent Amount:");
		window.draw(text);
		if (publicprop == true) {
			text.setString(std::to_string(static_cast<PublicProperty*>(listofcells[cellnumber])->getRentpublic()) + " PKR");
		}
		else {
			if (this->listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(propertyID)) {
				if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getanyupgrades()) {
					text.setString(std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getprivaterent()) + " PKR");
				}
				else {
					text.setString(std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getprivaterent() * 2) + " PKR");
				}
			}
			else {
				text.setString(std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getprivaterent()) + " PKR");
			}
		}
		text.setPosition(295, 175);
		window.draw(text);
		text.setString("Mortgage Value:");
		text.setPosition(105, 205);
		window.draw(text);
		text.setString(std::to_string(static_cast<Property*>(listofcells[cellnumber])->getMortgage()) + " PKR");
		text.setPosition(295, 205);
		window.draw(text);
		text.setString("Purchase Price:");
		text.setPosition(105, 235);
		window.draw(text);
		text.setString(std::to_string(static_cast<Property*>(listofcells[cellnumber])->getPrice()) + " PKR");
		text.setPosition(295, 235);
		window.draw(text);
		if (publicprop) {
			text.setString("Double Rent:");
			text.setPosition(105, 265);
			window.draw(text);
			bool flag = static_cast<PublicProperty*>(listofcells[cellnumber])->getDoubleRent();
			if (flag) {
				text.setString("Yes");
			}
			else {
				text.setString("No");
			}
			text.setPosition(295, 265);
			window.draw(text);
			icongraphic.loadFromImage(iconimage);
			icon.setTexture(&icongraphic);
			icon.setPosition(350, 100);
			window.draw(icon);
		}
		else {
			iconimage.loadFromFile("assets/upgrade/property-icon.png");
			icongraphic.loadFromImage(iconimage);
			icon.setTexture(&icongraphic);
			icon.setPosition(350, 100);
			window.draw(icon);
		}
		icon.setTexture(0);
		icon.setSize(sf::Vector2f(120, 50));
		icon.setOutlineColor(sf::Color::Black);
		icon.setOutlineThickness(2);
		icon.setPosition(310, 550);
		window.draw(icon);
		if (static_cast<Property*>(listofcells[cellnumber])->getmortgageflag() == false) {
			text.setString("Mortgage");
			text.setCharacterSize(20);
			text.setPosition(317, 560);
			window.draw(text);
			text.setCharacterSize(25);
			if (publicprop != true) {
				if (this->listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(propertyID)) {
					for (int i = 0, counter = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							if (i < 3) {
								sf::RectangleShape select(sf::Vector2f(35, 35));
								sf::Image selectimg;
								sf::Texture selectgraphic;
								if (j == 0) {
									selectimg.loadFromFile("assets/upgrade/Plus.png");
								}
								else {
									if (j == 1) {
										selectimg.loadFromFile("assets/upgrade/Minus.png");
									}
									else {
										switch (counter) {
										case 0:
											select.setSize(sf::Vector2f(50, 50));
											selectimg.loadFromFile("assets/upgrade/House.png");
											break;
										case 1:
											select.setSize(sf::Vector2f(50, 50));
											selectimg.loadFromFile("assets/upgrade/Shop2.png");
											break;
										case 2:
											select.setSize(sf::Vector2f(50, 50));
											selectimg.loadFromFile("assets/upgrade/Hotel.png");
											break;
										}
										counter++;
									}
								}
								selectgraphic.loadFromImage(selectimg);
								select.setTexture(&selectgraphic);
								if (j < 2) {
									select.setPosition(240 + j * 50, 280 + i * 60);
								}
								else {
									select.setPosition(230 + j * 60, 272 + i * 60);
								}
								select.setOutlineThickness(2);
								select.setOutlineColor(sf::Color::Black);
								window.draw(select);
							}
						}
						switch (i) {
						case 0:
							text.setPosition(105, 280 + i * 60);
							text.setString("Houses: " + std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofhouses()));
							break;
						case 1:
							text.setPosition(105, 280 + i * 60);
							text.setString("Shops: " + std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofshops()));
							break;
						case 2:
							text.setPosition(105, 280 + i * 60);
							text.setString("Hotels: " + std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofhotels()));
							break;
						}
						window.draw(text);
					}
					icon.setOutlineThickness(1);
					icon.setSize(sf::Vector2f(35, 35));
					icon.setOutlineThickness(1);
					text.setPosition(105, 460);
					text.setString("Wifi: " + std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getWifi()));
					window.draw(text);
					icon.setPosition(190, 460);
					text.setPosition(190, 465);
					window.draw(icon);
					text.setCharacterSize(18);
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getWifi()) {
						text.setString("Sell");
					}
					else {
						text.setString("Buy");
					}
					window.draw(text);
					text.setCharacterSize(25);
					text.setPosition(260, 460);
					text.setString("Gas: " + std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getGas()));
					window.draw(text);
					icon.setPosition(340, 460);
					text.setPosition(340, 465);
					window.draw(icon);
					text.setCharacterSize(18);
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getGas()) {
						text.setString("Sell");
					}
					else {
						text.setString("Buy");
					}
					window.draw(text);
					text.setCharacterSize(25);
					text.setPosition(105, 510);
					text.setString("Electricity: " + std::to_string(static_cast<PrivateProperty*>(listofcells[cellnumber])->getElectricity()));
					window.draw(text);
					icon.setPosition(260, 510);
					text.setPosition(260, 515);
					window.draw(icon);
					text.setCharacterSize(18);
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getElectricity()) {
						text.setString("Sell");
					}
					else {
						text.setString("Buy");
					}
					window.draw(text);
				}
			}
		}
		else {
			text.setString("Repurchase");
			text.setCharacterSize(20);
			text.setPosition(317, 560);
			window.draw(text);
			sf::RectangleShape lock(sf::Vector2f(330, 330));
			sf::Image lockimg;
			sf::Texture lockgraphic;
			lockimg.loadFromFile("assets/upgrade/Lock.png");
			lockgraphic.loadFromImage(lockimg);
			lock.setTexture(&lockgraphic);
			lock.setPosition(100, 180);
			window.draw(lock);
		}
		text.setString("Sell to other Players");
		text.setPosition(455, 105);
		text.setCharacterSize(20);
		sf::RectangleShape shape(sf::Vector2f(200, 40));
		shape.setPosition(450, 100);
		shape.setFillColor(sf::Color::White);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color::Black);
		window.draw(shape);
		window.draw(text);
		window.display();
	}
	ispublicprop = publicprop;
	cellnumtoreturn = cellnumber;
}
void Board::doublerentforutilities() {
	static int controller = 0;
	for (int i = 0; i < this->numberofplayers; i++) {
		if (this->listofplayers[i] != 0) {
			if (listofplayers[i][0].ownallutilities()) {
				controller = 1;
				break;
			}
			else {
			}
		}
	}
	if (controller == 1) {
		static_cast<PublicProperty*>(listofcells[12])->setDoubleRent(1);
		static_cast<PublicProperty*>(listofcells[28])->setDoubleRent(1);
		static_cast<PublicProperty*>(listofcells[37])->setDoubleRent(1);
		static_cast<PublicProperty*>(listofcells[39])->setDoubleRent(1);
		controller = 0;
	}
	else {
		controller = 0;
		static_cast<PublicProperty*>(listofcells[12])->setDoubleRent(0);
		static_cast<PublicProperty*>(listofcells[28])->setDoubleRent(0);
		static_cast<PublicProperty*>(listofcells[37])->setDoubleRent(0);
		static_cast<PublicProperty*>(listofcells[39])->setDoubleRent(0);
	}
}
void Board::doublerentforstations() {
	static int controller = 0;
	for (int i = 0; i < this->numberofplayers; i++) {
		if (this->listofplayers[thisplayersturn] != 0) {
			if (this->listofplayers[i] != 0) {
				if (listofplayers[i][0].ownallstations()) {
					controller = 1;
					break;
				}
				else {
				}
			}
		}
	}
	if (controller == 1) {
		static_cast<PublicProperty*>(listofcells[5])->setDoubleRent(1);
		static_cast<PublicProperty*>(listofcells[15])->setDoubleRent(1);
		static_cast<PublicProperty*>(listofcells[25])->setDoubleRent(1);
		static_cast<PublicProperty*>(listofcells[35])->setDoubleRent(1);
		controller = 0;
	}
	else {
		static_cast<PublicProperty*>(listofcells[5])->setDoubleRent(0);
		static_cast<PublicProperty*>(listofcells[15])->setDoubleRent(0);
		static_cast<PublicProperty*>(listofcells[25])->setDoubleRent(0);
		static_cast<PublicProperty*>(listofcells[35])->setDoubleRent(0);
	}

}
int  Board::whichoptionselectedcard(sf::Vector2i coordinates) {
	int topleftx = 0, toplefty = 0;
	for (int i = 0, count = 1; i < 3; i++) {
		toplefty = 280 + i * 60;
		for (int j = 0; j < 2; j++, count++) {
			topleftx = 250 + j * 50;
			if ((coordinates.x > topleftx&& coordinates.x < topleftx + 35) && (coordinates.y > toplefty&& coordinates.y < toplefty + 35)) {
				return count;
			}

		}
	}
	topleftx = 200;
	toplefty = 460;
	if ((coordinates.x > topleftx&& coordinates.x < topleftx + 35) && (coordinates.y > toplefty&& coordinates.y < toplefty + 35)) {
		return 7;
	}
	topleftx = 347;
	toplefty = 460;
	if ((coordinates.x > topleftx&& coordinates.x < topleftx + 35) && (coordinates.y > toplefty&& coordinates.y < toplefty + 35)) {
		return 8;
	}
	topleftx = 270;
	toplefty = 510;
	if ((coordinates.x > topleftx&& coordinates.x < topleftx + 35) && (coordinates.y > toplefty&& coordinates.y < toplefty + 35)) {
		return 9;
	}
	topleftx = 316;
	toplefty = 547;
	if ((coordinates.x > topleftx&& coordinates.x < topleftx + 120) && (coordinates.y > toplefty&& coordinates.y < toplefty + 50)) {
		return 10;
	}
	topleftx = 450;
	toplefty = 110;
	if ((coordinates.x > topleftx && coordinates.x < topleftx + 200) && (coordinates.y > toplefty && coordinates.y < toplefty + 40)) {
		return 11;
	}
	return -1;
}
void Board::upgradebuttonprocessing(int& selectinput, int propertyID, int cellnumber, bool ispublicprop, sf::RenderWindow& window) {
	if (selectinput != -1) {
		if (!ispublicprop) {
			if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(propertyID)) {
				int cell1 = -1, cell2 = -2, cell3 = -3;
				if (propertyID == (1) || propertyID == (2)) {
					cell1 = 1;
					cell2 = 3;
					cell3 = 3;
				}
				if (propertyID == (4) || propertyID == (5) || propertyID == (6)) {
					cell1 = 6;
					cell2 = 8;
					cell3 = 9;
				}

				if (propertyID == (7) || propertyID == (9) || propertyID == (10)) {
					cell1 = 11;
					cell2 = 13;
					cell3 = 14;
				}

				if (propertyID == (12) || propertyID == (13) || propertyID == (14)) {
					cell1 = 16;
					cell2 = 18;
					cell3 = 19;
				}

				if (propertyID == (15) || propertyID == (16) || propertyID == (17)) {
					cell1 = 21;
					cell2 = 23;
					cell3 = 24;
				}

				if (propertyID == (19) || propertyID == (20) || propertyID == (22)) {
					cell1 = 26;
					cell2 = 27;
					cell3 = 29;
				}

				if (propertyID == (23) || propertyID == (24) || propertyID == (25)) {
					cell1 = 31;
					cell2 = 33;
					cell3 = 34;
				}
				switch (selectinput) {
				case 1:
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofhouses() < static_cast<PrivateProperty*>(listofcells[cellnumber])->gethighesthouses() || (static_cast<PrivateProperty*>(listofcells[cell1])->getnumberofhouses() == static_cast<PrivateProperty*>(listofcells[cell1])->gethighesthouses() && static_cast<PrivateProperty*>(listofcells[cell2])->getnumberofhouses() == static_cast<PrivateProperty*>(listofcells[cell2])->gethighesthouses() && static_cast<PrivateProperty*>(listofcells[cell3])->getnumberofhouses() == static_cast<PrivateProperty*>(listofcells[cell3])->gethighesthouses())) {
						if (this->listofplayers[thisplayersturn][0].withdrawcash(100)) {
							static_cast<PrivateProperty*>(listofcells[cellnumber])->addhouse();
						}
					}
					break;
				case 2:
					this->listofplayers[thisplayersturn][0].depositcash(static_cast<PrivateProperty*>(listofcells[cellnumber])->removehouse());
					break;
				case 3:
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofshops() < static_cast<PrivateProperty*>(listofcells[cellnumber])->gethighestshops() || (static_cast<PrivateProperty*>(listofcells[cell1])->getnumberofshops() == static_cast<PrivateProperty*>(listofcells[cell1])->gethighestshops() && static_cast<PrivateProperty*>(listofcells[cell2])->getnumberofshops() == static_cast<PrivateProperty*>(listofcells[cell2])->gethighestshops() && static_cast<PrivateProperty*>(listofcells[cell3])->getnumberofshops() == static_cast<PrivateProperty*>(listofcells[cell3])->gethighestshops())) {
						if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofhouses() >= 3) {
							static_cast<PrivateProperty*>(listofcells[cellnumber])->addshop(true);
						}
						else {
							if (this->listofplayers[thisplayersturn][0].withdrawcash(300)) {
								static_cast<PrivateProperty*>(listofcells[cellnumber])->addshop(false);
							}
						}
					}
					break;
				case 4:
					this->listofplayers[thisplayersturn][0].depositcash(static_cast<PrivateProperty*>(listofcells[cellnumber])->removeshop());
					break;
				case 5:
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofhotels() < static_cast<PrivateProperty*>(listofcells[cellnumber])->gethighesthotels() || (static_cast<PrivateProperty*>(listofcells[cell1])->getnumberofhotels() == static_cast<PrivateProperty*>(listofcells[cell1])->gethighesthotels() && static_cast<PrivateProperty*>(listofcells[cell2])->getnumberofhotels() == static_cast<PrivateProperty*>(listofcells[cell2])->gethighesthotels() && static_cast<PrivateProperty*>(listofcells[cell3])->getnumberofhotels() == static_cast<PrivateProperty*>(listofcells[cell3])->gethighesthotels())) {
						static_cast<PrivateProperty*>(listofcells[cellnumber])->addhotel();
					}
					break;
				case 6:
					this->listofplayers[thisplayersturn][0].depositcash(static_cast<PrivateProperty*>(listofcells[cellnumber])->removehotel());
					break;
				case 7:
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getWifi()) {
						this->listofplayers[thisplayersturn][0].depositcash(15);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->setWifi(0);
					}
					else {
						if (this->listofplayers[thisplayersturn][0].withdrawcash(30)) {
							static_cast<PrivateProperty*>(listofcells[cellnumber])->setWifi(1);
						}
					}
					break;
				case 8:
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getGas()) {
						this->listofplayers[thisplayersturn][0].depositcash(25);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->setGas(0);
					}
					else {
						if (this->listofplayers[thisplayersturn][0].withdrawcash(50)) {
							static_cast<PrivateProperty*>(listofcells[cellnumber])->setGas(1);
						}
					}
					break;
				case 9:
					if (static_cast<PrivateProperty*>(listofcells[cellnumber])->getElectricity()) {
						this->listofplayers[thisplayersturn][0].depositcash(25);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->setElectricity(0);
					}
					else {
						if (this->listofplayers[thisplayersturn][0].withdrawcash(50)) {
							static_cast<PrivateProperty*>(listofcells[cellnumber])->setElectricity(1);
						}
					}
					break;
				}
			}
		}
		if (selectinput == 10) {
			if (static_cast<Property*>(listofcells[cellnumber])->getmortgageflag() == false) {
				if (ispublicprop) {
					this->listofplayers[thisplayersturn][0].depositcash(static_cast<PublicProperty*>(listofcells[cellnumber])->sellpublicproperty());
				}
				else {
					this->listofplayers[thisplayersturn][0].depositcash(static_cast<PrivateProperty*>(listofcells[cellnumber])->sellprivateproperty());
				}
				static_cast<Property*>(listofcells[cellnumber])->setmortgagedflag(1);
			}
			else {
				if (this->listofplayers[thisplayersturn][0].withdrawcash(static_cast<Property*>(listofcells[cellnumber])->getMortgage() * 1.2)) {

					static_cast<Property*>(listofcells[cellnumber])->setmortgagedflag(0);
				}
			}
		}
		selectinput = -1;
	}
}
void Board::updatehighestnumofupgrades() {
	static_cast<PrivateProperty*>(listofcells[1])->resethighest();
	static_cast<PrivateProperty*>(listofcells[1])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[3])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[6])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[8])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[9])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[11])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[13])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[14])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[16])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[18])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[19])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[21])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[23])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[24])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[26])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[27])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[29])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[31])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[33])->updatehighest();
	static_cast<PrivateProperty*>(listofcells[34])->updatehighest();
}
void Board::gotojail(sf::RenderWindow& window) {
	this->listofplayers[thisplayersturn][0].setturnsinjail(2);
	while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 11) {
		this->moveplayerprocess(window, true);
	}
}
bool Board::yesnopopup(sf::RenderWindow& window, std::string message) {
	sf::RectangleShape messageboard(sf::Vector2f(message.length() * 11.5, 75));
	sf::Font font;
	sf::Text text;
	sf::Event event;
	bool exitflag = false;
	font.loadFromFile("assets/TimesNew.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOutlineThickness(1);
	text.setOutlineColor(sf::Color::Black);
	text.setCharacterSize(20);
	text.setString(message);
	messageboard.setPosition(20, 700);
	text.setPosition(30, 710);
	messageboard.setFillColor(sf::Color::Color(65, 73, 80));
	messageboard.setOutlineThickness(3);
	messageboard.setOutlineColor(sf::Color::Black);
	window.clear(sf::Color::White);
	screenview.setCenter(640, 512);
	window.setView(screenview);
	this->drawboard(window);
	this->drawplayers(window);
	window.draw(messageboard);
	window.draw(text);
	window.display();
	while (!exitflag) {
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					return true;
					break;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					return false;
					break;
				}
			}
		}
	}
}
void Board::passedgo() {
	if (this->listofplayers[thisplayersturn][0].getturnsinjail() == 0) {
		listofplayers[thisplayersturn][0].depositcash(500);
	}
}
void Board::processlandedncell(sf::RenderWindow& window) {
	int c = this->listofplayers[thisplayersturn][0].getcurrentlyoncell();
	bool ispublic = false, isprivate = false;
	bool privateprophit = false;
	bool publicprophit = false;
	int numholder = 0;
	int sumtopay = 0;
	int propIDholder = 0;
	int playerIDholder = 0;
	int numberofshops = 0, numberofhouses = 0, numberofhotels = 0;
	bool purchasedflag = false;
	//Private----------------------------
	if (c == 2 || c == 4) {
		privateprophit = true;
	}
	if (c == 7 || c == 9 || c == 10) {
		privateprophit = true;
	}
	if (c == 12 || c == 14 || c == 15) {
		privateprophit = true;
	}
	if (c == 17 || c == 19 || c == 20) {
		privateprophit = true;
	}
	if (c == 22 || c == 24 || c == 25) {
		privateprophit = true;
	}
	if (c == 27 || c == 28 || c == 30) {
		privateprophit = true;
	}
	if (c == 32 || c == 34 || c == 35) {
		privateprophit = true;
	}
	if (privateprophit) {
		propIDholder = static_cast<Property*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getPropert_ID();
		for (int i = 0; i < numberofplayers; i++) {
			if (this->listofplayers[i] != 0) {
				if (this->listofplayers[i][0].propertyowned(propIDholder)) {
					playerIDholder = i;
					purchasedflag = true;
					break;
				}
			}
		}
		if (purchasedflag) {
			if (playerIDholder != thisplayersturn) {
				if (this->listofplayers[playerIDholder][0].playerownsallofthisprivategroup(static_cast<Property*>(listofcells[c - 1])->getPropert_ID())) {
					if (static_cast<PrivateProperty*>(listofcells[c - 1])->getanyupgrades()) {
						sumtopay = static_cast<PrivateProperty*>(listofcells[c - 1])->getprivaterent();
					}
					else {
						sumtopay = static_cast<PrivateProperty*>(listofcells[c - 1])->getprivaterent() * 2;
					}
				}
				else {
					sumtopay = static_cast<PrivateProperty*>(listofcells[c - 1])->getprivaterent();
				}
				if (sumtopay != 0) {
					yesnopopup(window, "Player " + std::to_string(thisplayersturn + 1) + " owes Player " + std::to_string(playerIDholder + 1) + ": " + std::to_string(sumtopay) + " PKR");
					if (!(this->listofplayers[thisplayersturn][0].withdrawcash(sumtopay))) {
						bankruptcyinitiated(window, playerIDholder, sumtopay);
					}
					else {
						this->listofplayers[playerIDholder][0].depositcash(sumtopay);
						yesnopopup(window, "Transaction Completed");
					}
				}
				else {
					yesnopopup(window, "Mortgaged property, no rent owed");
				}
			}
			else {
				yesnopopup(window, "You own this private property");
			}
		}
		else {
			int PropertyCells[28] = { 1,3,5,6,8,9,11,12,13,14,15,16,18,19,21,23,24,25,26,27,28,29,31,33,34,35,37,39 };
			for (int i = 0; i < 28; i++) {
				if (listofplayers[thisplayersturn]->getcurrentlyoncell() - 1 == PropertyCells[i]) {
					if (!PurchaseProperty(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1, false)) {
						Auction(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1);
					}
					break;
				}
			}
		}
	}
	//Private---------------------------
	if (c == 6 || c == 16 || c == 26 || c == 36) {
		//Public stations
		publicprophit = true;
	}
	if (c == 13 || c == 29 || c == 38 || c == 40) {
		//Public utility
		publicprophit = true;
	}
	if (publicprophit) {
		propIDholder = static_cast<Property*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getPropert_ID();
		for (int i = 0; i < numberofplayers; i++) {
			if (this->listofplayers[i] != 0) {
				if (this->listofplayers[i][0].propertyowned(propIDholder)) {
					playerIDholder = i;
					purchasedflag = true;
					break;
				}
			}
		}
		if (purchasedflag) {
			if (playerIDholder != thisplayersturn) {
				sumtopay = static_cast<PublicProperty*>(listofcells[c - 1])->getRentpublic();
				if (sumtopay != 0) {
					yesnopopup(window, "Player " + std::to_string(thisplayersturn + 1) + " owes Player " + std::to_string(playerIDholder + 1) + ": " + std::to_string(sumtopay) + " PKR");
					if (!(this->listofplayers[thisplayersturn][0].withdrawcash(sumtopay))) {
						bankruptcyinitiated(window, playerIDholder, sumtopay);
					}
					else {
						this->listofplayers[playerIDholder][0].depositcash(sumtopay);
						yesnopopup(window, "Transaction Completed");
					}
				}
				else {
					yesnopopup(window, "Mortgaged property, no rent owed");
				}
			}
			else {
				yesnopopup(window, "You own this public property");
			}
		}
		else {
			int PropertyCells[28] = { 1,3,5,6,8,9,11,12,13,14,15,16,18,19,21,23,24,25,26,27,28,29,31,33,34,35,37,39 };
			for (int i = 0; i < 28; i++) {
				if (listofplayers[thisplayersturn]->getcurrentlyoncell() - 1 == PropertyCells[i]) {
					if (!PurchaseProperty(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1, true)) {
						Auction(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1);
					}
					break;
				}
			}
		}
	}
	if (c == 3 || c == 18 || c == 33) {
		//Community
		DrawnCard gotthiscard = static_cast<Chest*>(listofcells[c - 1])->getrandomcard();
		drawcardforchanceorcomm(window, gotthiscard.message, false);
		switch (gotthiscard.id) {
		case 1:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 1) {
				this->moveplayerprocess(window, true);
			}
			this->listofplayers[thisplayersturn][0].depositcash(400);
			break;
		case 2:
			this->listofplayers[thisplayersturn][0].depositcash(200);
			break;
		case 3:
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(200))) {
				bankruptcyinitiated(window, thisplayersturn, 200);
			}
			break;
		case 4:
			this->listofplayers[thisplayersturn][0].depositcash(50);
			break;
		case 5:
			this->listofplayers[thisplayersturn][0].gainGOJFcard();
			break;
		case 6:
			this->listofplayers[thisplayersturn][0].depositcash(150);
			break;
		case 7:
			this->listofplayers[thisplayersturn][0].depositcash(200);
			break;
		case 8:
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(100))) {
				bankruptcyinitiated(window, thisplayersturn, 100);
			}
			break;
		case 9:
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(200))) {
				bankruptcyinitiated(window, thisplayersturn, 200);
			}
			break;
		case 10:
			this->listofplayers[thisplayersturn][0].depositcash(50);
			break;
		case 11:
			this->getplayershousehotelshopamount(numberofhouses, numberofshops, numberofhotels);
			sumtopay = 0;
			sumtopay += numberofhouses * 50;
			sumtopay += numberofhotels * 125;
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(sumtopay))) {
				bankruptcyinitiated(window, thisplayersturn, sumtopay);
			}
			break;
		case 12:
			this->listofplayers[thisplayersturn][0].depositcash(300);
			break;
		case 13:
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(50))) {
				bankruptcyinitiated(window, thisplayersturn, 50);
			}
			break;
		case 14:
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(80))) {
				bankruptcyinitiated(window, thisplayersturn, 80);
			}
			break;
		case 15:
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(50))) {
				bankruptcyinitiated(window, thisplayersturn, 50);
			}
			break;
		}
	}
	if (c == 8 || c == 23 || c == 37) {//Chance
		DrawnCard gotthiscard = static_cast<Chance*>(listofcells[c - 1])->getrandomcard();
		drawcardforchanceorcomm(window, gotthiscard.message, true);
		switch (gotthiscard.id) {
		case 1:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 1) {
				this->moveplayerprocess(window, true);
			}
			this->listofplayers[thisplayersturn][0].depositcash(300);
			break;
		case 2:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 27) {
				this->moveplayerprocess(window, true);
			}
			this->processlandedncell(window);
			break;
		case 3:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 13 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 29 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 38 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 40) {
				this->moveplayerprocess(window, true);
			}
			propIDholder = static_cast<Property*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getPropert_ID();
			for (int i = 0; i < numberofplayers; i++) {
				if (this->listofplayers[i] != 0) {
					if (this->listofplayers[i][0].propertyowned(propIDholder)) {
						playerIDholder = i;
						purchasedflag = true;
						break;
					}
				}
			}
			if (purchasedflag) {
				if (playerIDholder != thisplayersturn) {
					if (!(this->listofplayers[thisplayersturn][0].withdrawcash(static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic()* gamedice.getamountrolled()))) {
						bankruptcyinitiated(window, playerIDholder, static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic()* gamedice.getamountrolled());
					}
					else {
						this->listofplayers[playerIDholder][0].depositcash(static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic()* gamedice.getamountrolled());
					}
				}
			}
			else {
				int PropertyCells[28] = { 1,3,5,6,8,9,11,12,13,14,15,16,18,19,21,23,24,25,26,27,28,29,31,33,34,35,37,39 };
				for (int i = 0; i < 28; i++) {
					if (listofplayers[thisplayersturn]->getcurrentlyoncell() - 1 == PropertyCells[i]) {
						if (!PurchaseProperty(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1, true)) {
							Auction(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1);
						}
						break;
					}
				}
			}
			break;
		case 4:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 6 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 16 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 26 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 36) {
				this->moveplayerprocess(window, true);
			}
			propIDholder = static_cast<Property*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getPropert_ID();
			for (int i = 0; i < numberofplayers; i++) {
				if (this->listofplayers[i] != 0) {
					if (this->listofplayers[i][0].propertyowned(propIDholder)) {
						playerIDholder = i;
						purchasedflag = true;
						break;
					}
				}
			}
			if (purchasedflag) {
				if (playerIDholder != thisplayersturn) {
					if (!(this->listofplayers[thisplayersturn][0].withdrawcash(static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic() * 2))) {
						bankruptcyinitiated(window, playerIDholder, static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic() * 2);
					}
					else {
						this->listofplayers[playerIDholder][0].depositcash(static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic() * 2);
					}
				}
			}
			else {
				int PropertyCells[28] = { 1,3,5,6,8,9,11,12,13,14,15,16,18,19,21,23,24,25,26,27,28,29,31,33,34,35,37,39 };
				for (int i = 0; i < 28; i++) {
					if (listofplayers[thisplayersturn]->getcurrentlyoncell() - 1 == PropertyCells[i]) {
						if (!PurchaseProperty(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1, true)) {
							Auction(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1);
						}
						break;
					}
				}
			}
			break;
		case 5:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 6 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 16 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 26 && this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 36) {
				this->moveplayerprocess(window, true);
			}
			propIDholder = static_cast<Property*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getPropert_ID();
			for (int i = 0; i < numberofplayers; i++) {
				if (this->listofplayers[i] != 0) {
					if (this->listofplayers[i][0].propertyowned(propIDholder)) {
						playerIDholder = i;
						purchasedflag = true;
						break;
					}
				}
			}
			if (purchasedflag) {
				if (playerIDholder != thisplayersturn) {
					if (!(this->listofplayers[thisplayersturn][0].withdrawcash(static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic() * 2))) {
						bankruptcyinitiated(window, playerIDholder, static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic() * 2);
					}
					else {
						this->listofplayers[playerIDholder][0].depositcash(static_cast<PublicProperty*>(listofcells[listofplayers[thisplayersturn][0].getcurrentlyoncell() - 1])->getRentpublic() * 2);
					}
				}
			}
			else {
				int PropertyCells[28] = { 1,3,5,6,8,9,11,12,13,14,15,16,18,19,21,23,24,25,26,27,28,29,31,33,34,35,37,39 };
				for (int i = 0; i < 28; i++) {
					if (listofplayers[thisplayersturn]->getcurrentlyoncell() - 1 == PropertyCells[i]) {
						if (!PurchaseProperty(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1, true)) {
							Auction(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1);
						}
						break;
					}
				}
			}
			break;
		case 6:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 17) {
				this->moveplayerprocess(window, true);
				if (listofplayers[thisplayersturn][0].getcurrentlyoncell() == 1) {
					listofplayers[thisplayersturn][0].depositcash(300);
				}
			}
			this->processlandedncell(window);
			break;
		case 7:
			this->listofplayers[thisplayersturn][0].depositcash(100);
			break;
		case 8:
			this->listofplayers[thisplayersturn][0].gainGOJFcard();
			break;
		case 9:
			numholder = listofplayers[thisplayersturn][0].getcurrentlyoncell() - 4;
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != numholder) {
				this->moveplayerprocess(window, false);
			}
			this->processlandedncell(window);
			break;
		case 10:
			this->getplayershousehotelshopamount(numberofhouses, numberofshops, numberofhotels);
			sumtopay = 0;
			sumtopay += numberofhouses * 50;
			sumtopay += numberofhotels * 100;
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(sumtopay))) {
				bankruptcyinitiated(window, thisplayersturn, sumtopay);
			}
			break;
		case 11:
			if (!(this->listofplayers[thisplayersturn][0].withdrawcash(25))) {
				bankruptcyinitiated(window, thisplayersturn, 25);
			}
			break;
		case 12:
			for (int i = 0; i < numberofplayers; i++) {
				if (i != thisplayersturn) {
					if (!(this->listofplayers[thisplayersturn][0].withdrawcash(25))) {
						bankruptcyinitiated(window, thisplayersturn, 25);
					}
					else {
						if (this->listofplayers[i] != 0) {
							listofplayers[i][0].depositcash(25);
						}
					}
				}
			}
			break;
		case 13:
			listofplayers[thisplayersturn][0].depositcash(150);
			break;
		case 14:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 5) {
				this->moveplayerprocess(window, true);
			}
			break;
		case 15:
			while (this->listofplayers[thisplayersturn][0].getcurrentlyoncell() != 16) {
				this->moveplayerprocess(window, true);
			}
			this->processlandedncell(window);
			break;
		}
	}
	if (c == 5) {//Land Tax
		sumtopay = 0;
		for (int i = 0; i < 41; i++) {
			isprivate = false;
			ispublic = false;
			if (i == 1 || i == 3) {
				isprivate = true;
			}
			if (i == 6 || i == 8 || i == 9) {
				isprivate = true;
			}
			if (i == 11 || i == 13 || i == 14) {
				isprivate = true;
			}
			if (i == 16 || i == 18 || i == 19) {
				isprivate = true;
			}
			if (i == 21 || i == 23 || i == 24) {
				isprivate = true;
			}
			if (i == 26 || i == 27 || i == 29) {
				isprivate = true;
			}
			if (i == 31 || i == 33 || i == 34) {
				isprivate = true;
			}
			if (i == 5 || i == 15 || i == 25 || i == 35) {
				ispublic = true;
			}
			if (i == 12 || i == 28 || i == 37 || i == 39) {
				ispublic = true;
			}
			if (ispublic || isprivate) {
				if (listofplayers[thisplayersturn][0].propertyowned(static_cast<Property*>(listofcells[i])->getPropert_ID())) {
					if (isprivate) {
						sumtopay += static_cast<PrivateProperty*>(listofcells[i])->getPrice();
					}
					if (ispublic) {
						sumtopay += static_cast<PublicProperty*>(listofcells[i])->getPrice();
					}
				}
			}
		}
		sumtopay *= 0.2;
		yesnopopup(window, "Player " + std::to_string(thisplayersturn + 1) + " owes Bank: " + std::to_string(sumtopay) + " PKR Tax");
		if (!(listofplayers[thisplayersturn][0].withdrawcash(sumtopay))) {
			bankruptcyinitiated(window, thisplayersturn, sumtopay);
		}
		else {
			yesnopopup(window, "Transaction Complete");
		}
	}
	if (c == 39) {//Property Tax
		for (int i = 0; i < 41; i++) {
			isprivate = false;
			ispublic = false;
			if (i == 1 || i == 3) {
				isprivate = true;
			}
			if (i == 6 || i == 8 || i == 9) {
				isprivate = true;
			}
			if (i == 11 || i == 13 || i == 14) {
				isprivate = true;
			}
			if (i == 16 || i == 18 || i == 19) {
				isprivate = true;
			}
			if (i == 21 || i == 23 || i == 24) {
				isprivate = true;
			}
			if (i == 26 || i == 27 || i == 29) {
				isprivate = true;
			}
			if (i == 31 || i == 33 || i == 34) {
				isprivate = true;
			}
			if (i == 5 || i == 15 || i == 25 || i == 35) {
				ispublic = true;
			}
			if (i == 12 || i == 28 || i == 37 || i == 39) {
				ispublic = true;
			}
			if (ispublic || isprivate) {
				if (listofplayers[thisplayersturn][0].propertyowned(static_cast<Property*>(listofcells[i])->getPropert_ID())) {
					if (isprivate) {
						sumtopay += static_cast<PrivateProperty*>(listofcells[i])->getPrice();
					}
					if (ispublic) {
						sumtopay += static_cast<PublicProperty*>(listofcells[i])->getPrice();
					}
				}
			}
		}
		sumtopay *= 0.1;
		this->getplayershousehotelshopamount(numberofhouses, numberofshops, numberofhotels);
		sumtopay += numberofhouses * 100 * 0.20;
		sumtopay += numberofhotels * 1000 * 0.30;
		sumtopay += numberofshops * 300 * 0.30;
		yesnopopup(window, "Player " + std::to_string(thisplayersturn + 1) + " owes Bank: " + std::to_string(sumtopay) + " PKR Tax");
		if (!(listofplayers[thisplayersturn][0].withdrawcash(sumtopay))) {
			bankruptcyinitiated(window, thisplayersturn, sumtopay);
		}
		else {
			yesnopopup(window, "Transaction Complete");
		}
	}
	if (c == 21) {
		this->yesnopopup(window, "Pay parking fee of 10 PKR");
		if (!(this->listofplayers[thisplayersturn][0].withdrawcash(10))) {
			bankruptcyinitiated(window, thisplayersturn, 10);
		}
		else {
			yesnopopup(window, "Transaction Complete");
		}
	}
	if (c == 31) {
		this->yesnopopup(window, "Caught Red handed, Go to jail");
		this->gotojail(window);
	}
}
void Board::drawcardforchanceorcomm(sf::RenderWindow& window, std::string message, bool ischance) {
	sf::RectangleShape messageboard(sf::Vector2f(message.length() * 12, 75));
	sf::Font font;
	sf::Text text;
	sf::Event event;
	bool exitflag = false;
	font.loadFromFile("assets/TimesNew.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOutlineThickness(1);
	text.setOutlineColor(sf::Color::Black);
	text.setCharacterSize(22);
	text.setString(message);
	messageboard.setPosition(20, 700);
	text.setPosition(35, 720);
	if (ischance == false) {
		messageboard.setFillColor(sf::Color::Color(3, 136, 211));
	}
	else {
		messageboard.setFillColor(sf::Color::Color(220, 155, 89));
	}
	messageboard.setOutlineThickness(5);
	if (ischance == false) {
		messageboard.setOutlineColor(sf::Color::Color(0, 86, 161));
	}
	else {
		messageboard.setOutlineColor(sf::Color::Color(180, 95, 29));
	}
	window.clear(sf::Color::White);
	screenview.setCenter(640, 512);
	window.setView(screenview);
	this->drawboard(window);
	this->drawplayers(window);
	window.draw(messageboard);
	window.draw(text);
	window.display();
	while (!exitflag) {
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					exitflag = true;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					exitflag = true;
				}
			}
		}
	}
}
void Board::getplayershousehotelshopamount(int& numhouses, int& numshops, int& numhotels) {
	numhouses = 0;
	numhotels = 0;
	numshops = 0;
	if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(1)) {
		numhouses += static_cast<PrivateProperty*>(listofcells[1])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[1])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[1])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[3])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[3])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[3])->getnumberofhotels();
	}
	if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(4)) {
		numhouses += static_cast<PrivateProperty*>(listofcells[6])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[6])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[6])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[8])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[8])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[8])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[9])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[9])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[9])->getnumberofhotels();
	}
	if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(7)) {
		numhouses += static_cast<PrivateProperty*>(listofcells[11])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[11])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[11])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[13])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[13])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[13])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[14])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[14])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[14])->getnumberofhotels();
	}
	if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(12)) {
		numhouses += static_cast<PrivateProperty*>(listofcells[16])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[16])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[16])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[18])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[18])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[18])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[19])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[19])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[19])->getnumberofhotels();
	}
	if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(15)) {
		numhouses += static_cast<PrivateProperty*>(listofcells[21])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[21])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[21])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[23])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[23])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[23])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[24])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[24])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[24])->getnumberofhotels();
	}
	if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(19)) {
		numhouses += static_cast<PrivateProperty*>(listofcells[26])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[26])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[26])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[27])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[27])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[27])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[29])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[29])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[29])->getnumberofhotels();
	}
	if (listofplayers[thisplayersturn][0].playerownsallofthisprivategroup(23)) {
		numhouses += static_cast<PrivateProperty*>(listofcells[31])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[31])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[31])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[33])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[33])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[33])->getnumberofhotels();
		numhouses += static_cast<PrivateProperty*>(listofcells[34])->getnumberofhouses();
		numshops += static_cast<PrivateProperty*>(listofcells[34])->getnumberofshops();
		numhotels += static_cast<PrivateProperty*>(listofcells[34])->getnumberofhotels();
	}
}
void Board::bankruptcyinitiated(sf::RenderWindow& window, int idtoowemoney, int amountowed) {
	while (this->listofplayers[thisplayersturn][0].getbankruptflag() == false && this->listofplayers[thisplayersturn][0].withdrawcash(amountowed) == false) {
		yesnopopup(window, "You are unable to pay " + std::to_string(amountowed) + " PKR, Mortgage to muster funds or commit bankruptcy");
		upgradesystem(window, true);
	}
	if (this->listofplayers[thisplayersturn][0].getbankruptflag() == true) {//Begin removing player
		yesnopopup(window, "Player " + std::to_string(thisplayersturn + 1) + " you have been bankrupted, better luck next time");
		int sumtopay = 0;
		int cellnumber = 0;
		bool isprivate = false;
		int propidlist[28] = {};
		int numberofpropstransferred = 0;
		if (thisplayersturn != idtoowemoney) {
			yesnopopup(window, "Transfering any liquid assets to owed Player " + std::to_string(idtoowemoney+1) + ", Received: " + std::to_string(this->listofplayers[thisplayersturn][0].getbalance()) + " PKR");
			this->listofplayers[idtoowemoney][0].depositcash(this->listofplayers[thisplayersturn][0].getbalance());
		}
		else {
			yesnopopup(window, "Transfering any liquid assets to Bank");
		}
		if (thisplayersturn != idtoowemoney) {
			yesnopopup(window, "Transfering Properties to owed Player " + std::to_string(idtoowemoney+1));
		}
		else {
			yesnopopup(window, "Transfering Properties to Bank, available for purchase");
		}
		for (int i = 1; i <= 28; i++) {
			if (this->listofplayers[thisplayersturn][0].propertyowned(i)) {
				cellnumber = convertpropIDtocellnumber(i);
				if (static_cast<Property*>(listofcells[cellnumber])->getmortgageflag()) {
					sumtopay = static_cast<Property*>(listofcells[cellnumber])->getMortgage();
					if (cellnumber == 5 || cellnumber == 15 || cellnumber == 25 || cellnumber == 35) {
						isprivate = false;
					}
					else {
						isprivate = true;
					}
					if (cellnumber == 12 || cellnumber == 28 || cellnumber == 37 || cellnumber == 39) {
						isprivate = false;
					}
					else {
						isprivate = true;
					}
					if (isprivate) {
						static_cast<PrivateProperty*>(listofcells[cellnumber])->setElectricity(0);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->setGas(0);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->setWifi(0);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->sethouses(0);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->setshops(0);
						static_cast<PrivateProperty*>(listofcells[cellnumber])->sethotels(0);
					}
					if (idtoowemoney != thisplayersturn) {
						if (yesnopopup(window, "Left Clk: Pay " + std::to_string(int(sumtopay * 1.2)) + " PKR to unmortgage property, Right Clk: Only pay " + std::to_string(int(sumtopay * 0.2)) + " PKR interest & remain mortgaged")) {
							if (!(listofplayers[idtoowemoney][0].withdrawcash(sumtopay * 1.2))) {
								int tempholder = thisplayersturn;
								thisplayersturn = idtoowemoney;
								bankruptcyinitiated(window, idtoowemoney, sumtopay * 1.2);
								thisplayersturn = tempholder;
							}
							else {
								yesnopopup(window, "Payment Successful, Unmortgaging property");
								static_cast<Property*>(listofcells[cellnumber])->setmortgagedflag(0);
							}
						}
						else {
							if (!(listofplayers[idtoowemoney][0].withdrawcash(sumtopay * 0.2))) {
								int tempholder = thisplayersturn;
								thisplayersturn = idtoowemoney;
								bankruptcyinitiated(window, idtoowemoney, sumtopay * 0.2);
								thisplayersturn = tempholder;
							}
							else {
								yesnopopup(window, "Payment Successful, ownership granted albeit mortgaged");
							}
						}
					}
					else {

					}
				}
				else {

				}
				propidlist[numberofpropstransferred] = i;
				numberofpropstransferred++;
				changethispropsowner(thisplayersturn, idtoowemoney, i);
			}
		}
		if (idtoowemoney == thisplayersturn) {
			for (int i = 0; i < numberofpropstransferred; i++) {
				Auction(window, listofplayers[thisplayersturn]->getcurrentlyoncell() - 1);				
			}
		}
		delete this->listofplayers[thisplayersturn];
		listofplayers[thisplayersturn] = 0;
		endturn(window);
	}
	else {// Deduct appropritate accounts
		if (thisplayersturn != idtoowemoney) {
			this->listofplayers[idtoowemoney][0].depositcash(amountowed);
		}
		else {
			//Do nothing as bank was owed
		}
		yesnopopup(window, "Successfully dodged bankruptcy, payment of " + std::to_string(amountowed) + " PKR made");
	}
}
void Board::changethispropsowner(int playerIDtoremovefrom, int playeridtoaddto, int propID) {
	this->listofplayers[playerIDtoremovefrom][0].removepropertyID(propID);
	if (playerIDtoremovefrom != playeridtoaddto) {
		this->listofplayers[playeridtoaddto][0].addpropertyID(propID);
	}
	else {
		//Bank owns it
	}
}
int  Board::convertpropIDtocellnumber(int propID) {
	int cellnumber = -1;
	switch (propID) {
	case 1:
		cellnumber = 1;
		break;
	case 2:
		cellnumber = 3;
		break;
	case 3:
		cellnumber = 5;
		break;
	case 4:
		cellnumber = 6;
		break;
	case 5:
		cellnumber = 8;
		break;
	case 6:
		cellnumber = 9;
		break;
	case 7:
		cellnumber = 11;
		break;
	case 8:
		cellnumber = 12;
		break;
	case 9:
		cellnumber = 13;
		break;
	case 10:
		cellnumber = 14;
		break;
	case 11:
		cellnumber = 15;
		break;
	case 12:
		cellnumber = 16;
		break;
	case 13:
		cellnumber = 18;
		break;
	case 14:
		cellnumber = 19;
		break;
	case 15:
		cellnumber = 21;
		break;
	case 16:
		cellnumber = 23;
		break;
	case 17:
		cellnumber = 24;
		break;
	case 18:
		cellnumber = 25;
		break;
	case 19:
		cellnumber = 26;
		break;
	case 20:
		cellnumber = 27;
		break;
	case 21:
		cellnumber = 28;
		break;
	case 22:
		cellnumber = 29;
		break;
	case 23:
		cellnumber = 31;
		break;
	case 24:
		cellnumber = 33;
		break;
	case 25:
		cellnumber = 34;
		break;
	case 26:
		cellnumber = 35;
		break;
	case 27:
		cellnumber = 37;
		break;
	case 28:
		cellnumber = 39;
		break;
	}
	return cellnumber;
}
bool Board::haveWin() {
	uint32_t bankrupts = 0;
	for (int i = 0; i < numberofplayers; i++) {
		if (listofplayers[i] == 0) {
			bankrupts++;
		}
	}
	if (bankrupts == numberofplayers - 1) {
		return true;
	}
	return false;
}
void Board::showWinCard(sf::RenderWindow& window) {
	sf::RectangleShape WinCard;
	WinCard.setSize(sf::Vector2f(600, 800));
	sf::Texture WinTexture;
	WinTexture.loadFromFile("assets/Winner/winCard.png");
	WinCard.setTexture(&WinTexture);
	WinCard.setPosition(600, 500);
	sf::RectangleShape WinnerFace;

	sf::Font font;
	font.loadFromFile("assets/TimesNew.ttf");
	
	sf::Text Cash;
	Cash.setFont(font);
	Cash.setCharacterSize(34);
	Cash.setStyle(sf::Text::Bold);
	Cash.setFillColor(sf::Color::White);
	Cash.setPosition(900, 970);

	for (int i = 0; i < numberofplayers; i++) {
		if (listofplayers[i] != 0) {
			WinnerFace = listofplayers[i]->getplayershape();
			Cash.setString(std::to_string(listofplayers[i]->getbalance()));
			break;
		}
	}

	WinnerFace.setPosition(sf::Vector2f(800, 750));
	WinnerFace.setSize(sf::Vector2f(180, 180));

	sf::Event event;

	bool exitFlag = false;
	sf::Vector2f mousecoords;

	while (!exitFlag) {
		mousecoords.x = sf::Mouse::getPosition(window).x;
		mousecoords.y = sf::Mouse::getPosition(window).y;
		
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (mousecoords.x >= 455 &&
						mousecoords.x <= 730 &&
						mousecoords.y >= 760 &&
						mousecoords.y <= 890) {
						window.close();
					}
				}
				break;
			}
		}

		
		window.draw(WinCard);
		window.draw(WinnerFace);
		window.draw(Cash);
		window.display();
	}
}
void Board::SaveToFile() {
	int numberofupgradedproperties = 0;
	int cellnumber = 0;
	bool ispublic = false;
	std::ofstream fout;
	fout.open("savefiles/safe.txt");
	fout << this->numberofplayers << std::endl;
	fout << this->thisplayersturn << std::endl;
	std::ofstream PlayerFile;
	for (int i = 0; i < numberofplayers; i++) {
		std::string playerPath = "SaveFiles/P" + std::to_string(i + 1) + ".txt";
		PlayerFile.open(playerPath);
		PlayerFile << i << std::endl;
		if (listofplayers[i] != 0) {
			PlayerFile << 1 << std::endl;
			PlayerFile << listofplayers[i]->getbalance() << std::endl;
			PlayerFile << listofplayers[i]->getcurrentlyoncell() << std::endl;
			PlayerFile << listofplayers[i]->getGOJFcard() << std::endl;
			PlayerFile << listofplayers[i]->getnumberofpropertiesowned() << std::endl;
			for (int j = 0; j < listofplayers[i]->getnumberofpropertiesowned(); j++) {
				PlayerFile << listofplayers[i]->getPropertyIDlist()[j] << std::endl;
			}
			PlayerFile << listofplayers[i]->getplayercoordinates().x << " " << listofplayers[i]->getplayercoordinates().y << std::endl;
			PlayerFile << listofplayers[i]->getturnsinjail() << std::endl;
		} 
		else {
			PlayerFile << 0 << std::endl;
			PlayerFile << 0 << std::endl;
			PlayerFile << 0 << std::endl;
			PlayerFile << 0 << std::endl;
			PlayerFile << 0 << std::endl;
			PlayerFile << 0 << std::endl;
			PlayerFile << 0 << std::endl;
		}
		PlayerFile.close();
		fout.close();
	}
	fout.open("savefiles/propertydata.txt");
	for (int i = 1; i <= 28; i++) {
		ispublic = false;
		cellnumber = convertpropIDtocellnumber(i);
		fout << i << std::endl;
		fout << static_cast<Property*>(listofcells[cellnumber])->getmortgageflag() << std::endl;
		if (cellnumber == 5 || cellnumber == 15 || cellnumber == 25 || cellnumber == 35) {
			ispublic = true;
		}
		if (cellnumber == 12 || cellnumber == 28 || cellnumber == 37 || cellnumber == 39) {
			ispublic = true;
		}
		fout << ispublic << std::endl;
		if (ispublic) {
			fout << static_cast<PublicProperty*>(listofcells[cellnumber])->getDoubleRent() << std::endl;
		}
		else {
			fout << static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofhouses() << " ";
			fout << static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofshops() << " ";
			fout << static_cast<PrivateProperty*>(listofcells[cellnumber])->getnumberofhotels() << std::endl;
			fout << static_cast<PrivateProperty*>(listofcells[cellnumber])->getWifi() << " ";
			fout << static_cast<PrivateProperty*>(listofcells[cellnumber])->getGas() << " ";
			fout << static_cast<PrivateProperty*>(listofcells[cellnumber])->getElectricity() << std::endl;
		}
	}
	fout.close();
}
void Board::loadfromfile() {
	int numberofpropsowned=0, index=0;
	int quantityholder = 0;
	bool flagholder = 0;
	int cellnumber = 0;
	int propID = 0;
	bool ispublic = false;
	std::ifstream filereader;
	filereader.open("savefiles/safe.txt");
	filereader >> numberofplayers;
	filereader >> thisplayersturn;
	filereader.close();
	for (int i = 0; i < numberofplayers; i++) {
		std::string playerPath = "SaveFiles/P" + std::to_string(i + 1) + ".txt";
		filereader.open(playerPath);
		filereader >> index;
		filereader >> flagholder;
		if (flagholder != false) {
			filereader >> quantityholder;
			listofplayers[index][0].setbalance(quantityholder);
			filereader >> quantityholder;
			listofplayers[index][0].setcurrentcell(quantityholder);
			filereader >> quantityholder;
			listofplayers[index][0].setGOJFcardamount(quantityholder);
			filereader >> numberofpropsowned;
			for (int j = 0; j < numberofpropsowned; j++) {
				filereader >> quantityholder;
				listofplayers[index][0].addpropertyID(quantityholder);
			}
			int xcoord = 0, ycoord = 0;
			filereader >> xcoord>>ycoord;
			listofplayers[index][0].setplayercoordinates(sf::Vector2f(xcoord, ycoord));
			filereader >> quantityholder;
			listofplayers[index][0].setturnsinjail(quantityholder);
		}
		else {
			delete listofplayers[index];
			listofplayers[index] = 0;
		}
		filereader.close();
	}
	filereader.open("savefiles/propertydata.txt");
	for (int j = 1; j <= 28; j++) {
		filereader >> propID>>flagholder>>ispublic;
		cellnumber = convertpropIDtocellnumber(propID);
		static_cast<Property*>(listofcells[cellnumber])->setmortgagedflag(flagholder);
		if (ispublic) {
			filereader >> flagholder;
			static_cast<PublicProperty*>(listofcells[cellnumber])->setDoubleRent(flagholder);
		}
		else {
			filereader >> quantityholder;
			static_cast<PrivateProperty*>(listofcells[cellnumber])->sethouses(quantityholder);
			filereader >> quantityholder;
			static_cast<PrivateProperty*>(listofcells[cellnumber])->setshops(quantityholder);
			filereader >> quantityholder;
			static_cast<PrivateProperty*>(listofcells[cellnumber])->sethotels(quantityholder);
			filereader >> flagholder;
			static_cast<PrivateProperty*>(listofcells[cellnumber])->setWifi(flagholder);
			filereader >> flagholder;
			static_cast<PrivateProperty*>(listofcells[cellnumber])->setGas(flagholder);
			filereader >> flagholder;
			static_cast<PrivateProperty*>(listofcells[cellnumber])->setElectricity(flagholder);
		}
	}
	filereader.close();
}
#endif