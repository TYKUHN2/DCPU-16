#include "../Hardware/Peripheral.h"
#include "SineWave.h"

class Speaker : public Peripheral {

	friend int main(int, char*);

	SineWave * chans;

public:
	Speaker(Processor * proc);

	void interrupt(uint16_t) override;
};
