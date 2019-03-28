#pragma once
#include "Wave.h"

class SquareWave : public Wave {
protected:
	void generate() final;
};

