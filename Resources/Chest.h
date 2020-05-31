#pragma once
#include "Event.h"
class Chest : public Event {
	DrawnCard carddetails;
public:
	Chest() : Event(0) {
		carddetails.message = "";
		carddetails.id = -1;
	}
	DrawnCard getrandomcard() {
		carddetails.id = (rand() % (15)) + 1;
		switch (carddetails.id) {
		case 1:
			carddetails.message = "Advance to Go and Collect 400 PKR";
			break;
		case 2:
			carddetails.message = "Bank will pay you 200 PKR";
			break;
		case 3:
			carddetails.message = "Pay Doctor Fee 200 PKR";
			break;
		case 4:
			carddetails.message = "You got 50 PKR from a Sale";
			break;
		case 5:
			carddetails.message = "Get out of Jail card. May be kept until needed or sold for 500 PKR";
			break;
		case 6:
			carddetails.message = "Income Tax refund collect 150 PKR";
			break;
		case 7:
			carddetails.message = "Your health insurance matures collect 200 PKR";
			break;
		case 8:
			carddetails.message = "Pay donation to Hospital 100 PKR";
			break;
		case 9:
			carddetails.message = "Pay Student tax of 200 PKR";
			break;
		case 10:
			carddetails.message = "Collect 50 PKR for your services";
			break;
		case 11:
			carddetails.message = "Pay Street repair charges 50 PKR per House 125 PKR per Hotel";
			break;
		case 12:
			carddetails.message = "You won prize money of 300 PKR";
			break;
		case 13:
			carddetails.message = "Pay water bill of 50 PKR";
			break;
		case 14:
			carddetails.message = "Pay electricity bill of 80 PKR";
			break;
		case 15:
			carddetails.message = "Pay internet bill 50 PKR";
			break;
		}
		return carddetails;
	}
};
