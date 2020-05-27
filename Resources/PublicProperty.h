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
	int getRentpublic() {
		if (this->getmortgageflag()==false) {
			if (this->doubleRent) {
				return this->getRent() * 2;
			}
			return this->getRent();
		}
		return 0;
	}
	int sellpublicproperty() {
		if (this->getmortgageflag() == false) {
			this->setmortgagedflag(1);
			return this->getMortgage();
		}
		return 0;
	}
};