#pragma once
#include <string>
#include "Cell.h"
class Property : public Cell {
	int Property_ID;
	int price;
	int Player_ID;
	int mortgage;
	int rent;
	std::string group;
public:
	Property() :Cell() {
		static int pid = 1;
		Property_ID = pid;
		price = 0;
		Player_ID = -1;
		mortgage = 0;
		rent = 0;
		group = "0";
		pid++;
	}
	void setPrice(int price) { this->price = price; }
	void setPlayerID(int id) { this->Player_ID = id; }
	void setMortgage(int mortgage) { this->mortgage = mortgage; }
	void setRent(int rent) { this->rent = rent; }
	void setGroup(std::string group) { this->group = group; }
	int getPrice() { return price; }
	int getPropert_ID() { return Property_ID; }
	int getMortgage() { return mortgage; }
	int getRent() { return rent; }
	std::string getGroup() { return group; }
};