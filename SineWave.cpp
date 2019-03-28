#include "SineWave.h"
#include "config.h"

constexpr double tau = 6.28318;

void SineWave::generate() {
	double inc = (double)freq / SAMPLE_RATE;

	for (unsigned int i = 0; i < SAMPLE_SIZE; i++) {
		buffer[i] = (int16_t)(DEFAULT_VOLUME * sin(pos * tau));
		pos += inc;
	}
}
