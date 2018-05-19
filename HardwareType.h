#pragma once

struct HardwareType {
	char upperClass : 4;
	char subClass : 4;

	char api : 4;
	
	char extended : 1;
	char mapped : 1;
	char resetable : 1;
	char interrupts : 1;

	short uniqueID;
};
