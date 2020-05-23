#pragma once
#include "Card.h"
#include "Event.h"

class Chest : public Event {
	Card* card;
public:
	Chest() : Event(0) {
		card = new Card(0);
	}
};
