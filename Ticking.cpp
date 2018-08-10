#include "Ticking.h"

struct TickList {
	Ticking * ticker;
	TickList * next = nullptr;
};

TickList * firstTicker = nullptr;
TickList * lastTicker = nullptr;

Ticking::Ticking()
{
	TickList * newList = new TickList{
		this
	};

	if (firstTicker == nullptr) {
		firstTicker = newList;
		lastTicker = newList;
	}
	else {
		lastTicker->next = newList;
		lastTicker = newList;
	}
}

Ticking::~Ticking()
{
	TickList * curMon = firstTicker;
	TickList * lastTicker = nullptr;
	while (curMon->ticker != this) {
		lastTicker = curMon;
		curMon = curMon->next;
	}

	if (firstTicker == curMon) {
		firstTicker = nullptr;
		lastTicker = nullptr;
	}
	else {
		lastTicker->next = curMon->next;
		delete curMon;
	}
}

void Ticking::doTick()
{
	TickList * curTick = firstTicker;
	while (curTick != nullptr) {
		curTick->ticker->tick();
		curTick = curTick->next;
	}
}
