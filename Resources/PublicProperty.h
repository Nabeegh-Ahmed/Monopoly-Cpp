#pragma once
#include "Property.h"

class PublicProperty : public Property {
	bool doubleRent;
public:
	PublicProperty() : Property() {
		doubleRent = false;
	}
	void setDoubleRent(bool val) { this->doubleRent = val; }
	bool getDoubleRent() { return this->doubleRent; }
};