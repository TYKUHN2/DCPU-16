#include "Monitor.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>

Monitor::Monitor(unsigned int x, unsigned int y, unsigned int depth)
{
	sf::VideoMode mode{ x, y, depth };
	sf::String title = "DCPU-16 Monitor";
	uint32_t style = 1; //Titlebar without resize

	window.create( mode, title, style );
}

Monitor::~Monitor()
{
	window.close();
}

void Monitor::tick()
{
	sf::Event nextEvent;
	window.pollEvent(nextEvent);
}
