#pragma once
#include "Event.h"
class Chance : public Event {
	DrawnCard carddetails;
public:
	Chance() :Event(1) {
		carddetails.message = "";
		carddetails.id = -1;
	}
	DrawnCard getrandomcard() {
		carddetails.id = (rand() % (15)) + 1;
		switch (carddetails.id) {
		case 1:
			carddetails.message = "Advance to Go and Collect 300 PKR";
			break;
		case 2:
			carddetails.message = "Advance to DHA Phase 1";
			break;
		case 3:
			carddetails.message = "Advance token to nearest utility. If unowned by from Bank. If owned, pay to owner 5X the amount shown on dice";
			break;
		case 4:
			carddetails.message = "Advance token to nearest Station. If unowned by from Bank. If owned, pay to owner the double amount";
			break;
		case 5:
			carddetails.message = "Advance token to nearest Station. If unowned by from Bank. If owned, pay to owner the double amount";
			break;
		case 6:
			carddetails.message = "Advance to Model Town 1. If you pass Go collect 300 PKR";
			break;
		case 7:
			carddetails.message = "Bank pays you 100 PKR";
			break;
		case 8:
			carddetails.message = "Get out of Jail. May be kept until needed or sold for 500 PKR";
			break;
		case 9:
			carddetails.message = "Go back 4 blocks";
			break;
		case 10:
			carddetails.message = "Make repair on your property. For each house pay 50 PKR For each hotel pay 100 PKR";
			break;
		case 11:
			carddetails.message = "Pay small Tax of 25 PKR";
			break;
		case 12:
			carddetails.message = "You have been elected as chairperson. Pay 25 PKR to each player";
			break;
		case 13:
			carddetails.message = "Collect 150 PKR from the Bank";
			break;
		case 14:
			carddetails.message = "Advance to Land Tax and do not pay any Tax there";
			break;
		case 15:
			carddetails.message = "Advance token to Metro Station";
			break;
		}
		return carddetails;
	}
};