#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class Monitor
{
protected:
	sf::RenderWindow window;

public:
	Monitor(unsigned int, unsigned int, unsigned int);
	~Monitor();
};

