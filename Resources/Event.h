#pragma once
#include <stdint.h>
#include "Cell.h"
struct DrawnCard {
	int id;
	std::string message;
};
class Event : public Cell {
	uint32_t type; // 0 for chest, 1 for chance
public:
	Event(uint32_t event) : Cell() {
		type = event;
	}
	uint32_t getEvent() { return type; }
	void setEvent(uint32_t event) { type = event; }

};