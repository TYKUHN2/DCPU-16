#include "SquareWave.h"
#include "config.h"

#include "Debug.h"

void SquareWave::generate() {
	double inc = (double)freq / SAMPLE_RATE;

	for (int i = 0; i < SAMPLE_SIZE; i++) {
		float part = (float)fmod(pos, 1);

		if (part <= 0.5)
			buffer[i] = DEFAULT_VOLUME;
		else
			buffer[i] = 0;

		pos += inc;
	}
}
