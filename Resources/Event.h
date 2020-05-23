#pragma once
#include <stdint.h>
#include "Cell.h"

class Event : public Cell {
	uint32_t type; // 0 for chest, 1 for chance, 2 for jail
public:
	Event(uint32_t event) : Cell() {
		type = event;
	}
	uint32_t getEvent() { return type; }
	void setEvent(uint32_t event) { type = event; }

};