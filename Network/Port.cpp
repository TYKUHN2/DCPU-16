#include "Wired.h"

Port::Port(Wired * parent) : parent(parent) {
}

Port::Port(Wired * parent, Port * remote) : parent(parent) {
	if (!remote->connect(this))
		throw 1;
}

Port::~Port() {
	connection->disconnect();
}

bool Port::connect(Port * remote) {
	if (connection)
		return false;

	connection = remote;
	connected = true;
	return true;
}

void Port::disconnect() {
	connection = nullptr;
	connected = false;
}

void Port::receive(uint32_t data) {
	parent->receive(data, this);
}

void Port::send(uint32_t data) {
	connection->receive(data);
}
