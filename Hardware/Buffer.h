#pragma once
#include <queue>

class Buffer {
	std::queue<uint8_t> buffer;

public:
	void clear();
	uint8_t pop();
};
