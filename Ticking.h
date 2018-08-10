#pragma once
class Ticking
{
protected:
	Ticking();
	~Ticking();

public:
	static void doTick();

	virtual void tick() = 0;
};

