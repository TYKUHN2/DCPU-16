#include "Buffer.h"

void Buffer::clear() {
	for (int i = 0; i < buffer.size(); i++)
		buffer.pop();
}

uint8_t Buffer::pop() {
	uint8_t res = 0;
	if (buffer.size() > 0) {
		res = buffer.front();
		buffer.pop();
	}

	return res;
}
