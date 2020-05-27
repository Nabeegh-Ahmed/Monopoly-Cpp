#pragma once
#include <string>
#include "Cell.h"
class Property : public Cell {
	int Property_ID;
	int price;
	int Player_ID;
	int mortgage;
	int rent;
	bool mortgaged;
	std::string group;
public:
	Property() :Cell() {
		static int pid = 1;
		Property_ID = pid;
		price = 0;
		mortgaged = false;
		Player_ID = -1;
		mortgage = 0;
		rent = 0;
		group = "0";
		pid++;
	}
	void setPrice(int price) { this->price = price; this->setMortgage(price * 0.5); }
	void setPlayerID(int id) { this->Player_ID = id; }
	void setMortgage(int mortgage) { this->mortgage = mortgage; }
	void setRent(int rent) { this->rent = rent; }
	void setGroup(std::string group) { this->group = group; }
	void recalculaterent();
	void setmortgagedflag(bool flag) { this->mortgaged = flag; }
	int getPrice() { return price; }
	int getPropert_ID() { return Property_ID; }
	int getMortgage() { return mortgage; }
	int getRent() { return rent; }
	bool getmortgageflag() { return this->mortgaged; }
	std::string getGroup() { return group; }
};