#pragma once
#include <SFML/Audio/SoundStream.hpp>
#include <cstdint>

class Wave : public sf::SoundStream {
protected:
	uint16_t freq = 0;
	double pos = 0;

	int16_t * buffer;

	virtual void generate() = 0;

public:
	Wave();

	bool onGetData(sf::SoundStream::Chunk&) final;
	void onSeek(sf::Time) final;

	void setFrequency(uint16_t);
};

