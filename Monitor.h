#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Ticking.h"

class Monitor :
	public Ticking
{
protected:
	sf::RenderWindow window;

public:
	void tick() override;
	Monitor(unsigned int, unsigned int, unsigned int);
	~Monitor();
};

