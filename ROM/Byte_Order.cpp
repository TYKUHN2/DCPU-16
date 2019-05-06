#include "Byte_Order.h"
#include <cstring>

void swapBO(char * buffer, int size, int length) { //In place memory swap
	char * temp = new char[size];

	for (int i = 0; i < length * size; i += size) {
		memcpy(temp, buffer + i, size);

		for (char j = 1; j <= size; j++) {
			buffer[i + (j - 1)] = temp[size - j];
		}
	}

	delete temp;
}
