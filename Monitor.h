#pragma onces
#include <SFML/Graphics/RenderWindow.hpp>

class Monitor
{
protected:
	sf::RenderWindow window;

public:
	Monitor(int, int, int);
	~Monitor();
};

