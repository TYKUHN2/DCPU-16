#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Ticking.h"

typedef unsigned int uint;

class Monitor : public Ticking {
	sf::RenderWindow window;
protected:
	sf::RenderTexture target;

public:
	void tick() override;
	Monitor(uint, uint, uint);
	~Monitor();
};

