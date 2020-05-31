#pragma once
#include<string>
#include<stdint.h>


class Card {
	uint32_t type; // 0 for Chest, 1 for Chance
	std::string cards[];
public:
	Card(uint32_t type) {
		this->type = type;
		cards;
		// Set Appropriate type
		// Open the file accordingly and load data into cards[];
	}
	DrawnCard GetOption() {
		// return a random string from cards[]; plus the line number of the message
		// DrawnCard is a struct, put data in it by making a DrawnCard object say card
		// Then card.id = the line number, card.message = the message on that line number
		// return card
	}
};