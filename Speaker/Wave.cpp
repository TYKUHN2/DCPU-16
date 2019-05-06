#include "Wave.h"
#include "config.h"

Wave::Wave() {
	buffer = new int16_t[SAMPLE_SIZE];
	initialize(1, SAMPLE_RATE);
}

bool Wave::onGetData(sf::SoundStream::Chunk &data) {
	if (freq == 0) {
		data.sampleCount = 0;
		return false;
	}

	generate();

	data.samples = buffer;
	data.sampleCount = SAMPLE_SIZE;

	return true;
}

void Wave::onSeek(sf::Time offset) {
	int64_t time = offset.asMicroseconds();
	float mfreq = (float)freq / 1000;

	pos = ((float)time * mfreq) / 1000;
}

void Wave::setFrequency(uint16_t newfreq) {
	sf::Time curTime = getPlayingOffset();
	stop();
	freq = newfreq;

	if (newfreq != 0) {
		setPlayingOffset(curTime);
		play();
	}
}
