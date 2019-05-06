#pragma once
#include <cstdint>

class Wired;

class Port {
	Wired * parent;

protected:
	Port * connection = nullptr;

	bool connect(Port*);
	void disconnect();

	void receive(uint32_t);

public:
	bool connected;

	Port(Wired*);
	Port(Wired*, Port*);
	~Port();

	void send(uint32_t);
};

