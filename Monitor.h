#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Ticking.h"

typedef unsigned int uint;

class Monitor : public Ticking {
protected:
	sf::RenderWindow window;

public:
	void tick() override;
	Monitor(uint, uint, uint);
	~Monitor();
};

