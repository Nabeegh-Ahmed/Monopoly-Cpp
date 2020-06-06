#ifndef PrivateProperty_HEADER
#define PrivateProperty_HEADER
#pragma once
#include "Property.h"
class PrivateProperty : public Property {
	bool wifi;
	bool gas;
	bool electricity;
	static int highestnumberofhouses[7];
	static int highestnumberofhotels[7];
	static int highestnumberofshops[7];
	int numberofhouses;
	int numberofhotels;
	int numberofshops;
	bool anyupgrades;
public:
	PrivateProperty() : Property() {
		for (int i = 0; i < 7; i++) {
			highestnumberofhotels[i] = 0;
			highestnumberofshops[i] = 0;
			highestnumberofhouses[i] = 0;
		}
		numberofhotels = 0;
		numberofshops = 0;
		numberofhouses = 0;
		wifi = false;
		gas = false;
		electricity = false;
		anyupgrades = false;
	}
	bool getWifi() { return wifi; }
	bool getGas() { return gas; }
	void checkforupgrades() {
		bool flag = false;
		if (wifi) {
			flag = true;
		}
		if (gas) {
			flag = true;
		}
		if (electricity) {
			flag = true;
		}
		if (numberofhouses > 0) {
			flag = true;
		}
		if (numberofshops > 0) {
			flag = true;
		}
		if (numberofhotels > 0) {
			flag = true;
		}
		anyupgrades = flag;
	}
	int getprivaterent() {
		if (this->getmortgageflag() == false) {
			checkforupgrades();
			int rentreturned = this->getRent();
			if (wifi) {
				rentreturned += 30 * 0.10;
			}
			if (gas) {
				rentreturned += 50 * 0.20;
			}
			if (electricity) {
				rentreturned += 50 * 0.20;
			}
			if (numberofhouses > 0) {
				rentreturned += (numberofhouses * 100) * 0.30;
			}
			if (numberofshops > 0) {
				rentreturned += (numberofshops * 300) * 0.60;
			}
			if (numberofhotels > 0) {
				rentreturned += (numberofhotels * 1000) * 0.80;
			}
			return rentreturned;
		}
		return 0;
	}
	bool getElectricity() { return electricity; }
	void setWifi(bool val) { wifi = val; }
	void setGas(bool val) { gas = val; }
	void setElectricity(bool val) { electricity = val; }
	void addhouse() {
		this->numberofhouses++;
	}
	bool addshop(bool sellhouses) {
		if (sellhouses) {
			if (numberofhouses >= 3) {
				this->numberofhouses -= 3;
				this->numberofshops++;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			this->numberofshops++;
			return true;
		}

	}
	bool addhotel() {
		if (numberofshops >= 2 && numberofhouses >= 4) {
			this->numberofshops -= 2;
			this->numberofhouses -= 4;
			this->numberofhotels++;
			return true;
		}
		return false;
	}
	int removehouse() {
		if (numberofhouses > 0) {
			this->numberofhouses--;
			return 50;
		}
		return 0;
	}
	int removeshop() {
		if (numberofshops > 0) {
			this->numberofshops--;
			return 150;
		}
		return 0;
	}
	int removehotel() {
		if (numberofhotels > 0) {
			this->numberofhotels--;
			return 500;
		}
		return 0;
	}
	int getnumberofhouses() { return this->numberofhouses; }
	int getnumberofshops() { return this->numberofshops; }
	int getnumberofhotels() { return this->numberofhotels; }
	int sellprivateproperty() {
		if (this->getmortgageflag() == false) {
			int totalamountretreived = 0;
			totalamountretreived += this->getMortgage();
			while (this->numberofhouses > 0) {
				totalamountretreived += removehouse();
			}
			while (this->numberofshops > 0) {
				totalamountretreived += removeshop();
			}
			while (this->numberofhotels > 0) {
				totalamountretreived += removehotel();
			}
			if (wifi) { totalamountretreived += (30) / 2; wifi = false; }
			if (electricity) { totalamountretreived += (50) / 2; electricity = false; }
			if (gas) { totalamountretreived += (50) / 2; gas = false; }
			this->setmortgagedflag(1);
			return totalamountretreived;
		}
		return 0;
	}
	bool getanyupgrades() { this->checkforupgrades(); return this->anyupgrades; }
	void updatehighest() {
		if (numberofhotels > this->highestnumberofhotels[convertpropidtogroup(this->getPropert_ID())]) {
			this->highestnumberofhotels[convertpropidtogroup(this->getPropert_ID())] = numberofhotels;
		}
		if (numberofhouses > this->highestnumberofhouses[convertpropidtogroup(this->getPropert_ID())]) {
			highestnumberofhouses[convertpropidtogroup(this->getPropert_ID())] = numberofhouses;
		}
		if (numberofshops > this->highestnumberofshops[convertpropidtogroup(this->getPropert_ID())]) {
			highestnumberofshops[convertpropidtogroup(this->getPropert_ID())] = numberofshops;
		}
	}
	void resethighest() {
		for (int i = 0; i < 7; i++) {
			highestnumberofhotels[i] = 0;
			highestnumberofshops[i] = 0;
			highestnumberofhouses[i] = 0;
		}
	}
	int gethighesthotels() {
		return highestnumberofhotels[convertpropidtogroup(this->getPropert_ID())];
	}
	int gethighesthouses() {
		return highestnumberofhouses[convertpropidtogroup(this->getPropert_ID())];
	}
	int gethighestshops() {
		return highestnumberofshops[convertpropidtogroup(this->getPropert_ID())];
	}
	int convertpropidtogroup(int propertyID) {
		if (propertyID == (1) || propertyID == (2)) {
			return 0;
		}
		if (propertyID == (4) || propertyID == (5) || propertyID == (6)) {
			return 1;
		}

		if (propertyID == (7) || propertyID == (9) || propertyID == (10)) {
			return 2;
		}

		if (propertyID == (12) || propertyID == (13) || propertyID == (14)) {
			return 3;
		}

		if (propertyID == (15) || propertyID == (16) || propertyID == (17)) {
			return 4;
		}

		if (propertyID == (19) || propertyID == (20) || propertyID == (22)) {
			return 5;
		}

		if (propertyID == (23) || propertyID == (24) || propertyID == (25)) {
			return 6;
		}
		return -1;
	}
	void sethouses(int num) {
		this->numberofhouses = num;
	}
	void setshops(int num) {
		this->numberofshops = num;
	}
	void sethotels(int num) {
		this->numberofhotels = num;
	}
};
#endif