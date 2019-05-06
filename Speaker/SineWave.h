#pragma once
#include "Wave.h"

class SineWave : public Wave {
protected:
	void generate() final;
};
