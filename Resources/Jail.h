#pragma once
#include "Card.h"
#include "Event.h"
#include <vector>

class Jail : public Event {
	std::vector<int> Player_IDs;
public:
	Jail() : Event(2) {}
	void AddPlayer(int playerId) { Player_IDs.push_back(playerId); }
	std::vector<int> GetJail() { return Player_IDs; }
};