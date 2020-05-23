#pragma once
#include "Event.h"
#include "Card.h"

class Chance : public Event {
	Card* card;
public:
	Chance() :Event(0) {
		card = new Card(1);
	}
};