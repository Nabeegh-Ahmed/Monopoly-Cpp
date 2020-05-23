#pragma once
#include "Property.h"
class PrivateProperty : public Property {
	bool wifi;
	bool gas;
	bool electricity;
public:
	PrivateProperty() : Property() {
		wifi = false;
		gas = false;
		electricity = false;
	}
	bool getWifi() { return wifi; }
	bool getGas() { return gas; }
	bool getElectricity() { return electricity; }
	void setWifi(bool val) { wifi = val; }
	void setGas(bool val) { gas = val; }
	void setElectricity(bool val) { electricity = val; }
};