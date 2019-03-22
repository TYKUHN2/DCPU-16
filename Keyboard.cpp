#include "Keyboard.h"
#include <SFML/Window/Keyboard.hpp>

constexpr HWClass keyboard{
	HWClass::HUMAN_INTERFACE,
	(int)HWSubclass::KEYBOARD
};

constexpr uint32_t uniqueid		= 0x7406;
constexpr uint16_t vers			= 0x0001;

constexpr HWAPI keyboardapi = {
	0xC,		//Random standard API
	false,		//No extended API
	false,		//Not memory mapped
	false,		//No reset command
	true,		//Can have interrupts
};

constexpr HWType generickeyboard = {
	keyboard,
	keyboardapi,
	uniqueid
};

enum class Commands {
	CLEAR_BUFFER,
	GET_NEXT,
	CHECK_KEY,
	SET_INT,
	SET_MODE
};

inline bool keyPressed(sf::Keyboard::Key key) {
	return sf::Keyboard::isKeyPressed(key);
}

bool checkKey(uint16_t key) {
	bool res = false;

	switch (key) {
	case 0x10:
		return keyPressed(sf::Keyboard::Backspace);
	case 0x11:
		return keyPressed(sf::Keyboard::Return);
	case 0x12:
		return keyPressed(sf::Keyboard::Insert);
	case 0x13:
	case 0x7F:
		return keyPressed(sf::Keyboard::Delete);
	case 0x80:
		return keyPressed(sf::Keyboard::Up);
	case 0x81:
		return keyPressed(sf::Keyboard::Down);
	case 0x82:
		return keyPressed(sf::Keyboard::Left);
	case 0x83:
		return keyPressed(sf::Keyboard::Right);
	case 0x91:
		return keyPressed(sf::Keyboard::LControl) || keyPressed(sf::Keyboard::RControl);
	case 0x92:
		return keyPressed(sf::Keyboard::LAlt) || keyPressed(sf::Keyboard::RAlt);
	case 0x2E:
		return keyPressed(sf::Keyboard::Period);
	case 0x2C:
		return keyPressed(sf::Keyboard::Comma);
	case 0x3B:
		return keyPressed(sf::Keyboard::Semicolon);
	case 0x2D:
		return keyPressed(sf::Keyboard::Subtract);
	case 0x3D:
		return keyPressed(sf::Keyboard::Equal);
	case 0x2F:
		return keyPressed(sf::Keyboard::Slash);
	case 0x5C:
		return keyPressed(sf::Keyboard::Backslash);
	case 0x5B:
		return keyPressed(sf::Keyboard::LBracket);
	case 0x5D:
		return keyPressed(sf::Keyboard::RBracket);
	case 0x27:
		return keyPressed(sf::Keyboard::Quote);
	case 0x60:
		return keyPressed(sf::Keyboard::Tilde);
	case 0x20:
		return keyPressed(sf::Keyboard::Space);
	case 0x7E:
		res = keyPressed(sf::Keyboard::Tilde);
		break;
	case 0x21:
		res = keyPressed(sf::Keyboard::Num1);
		break;
	case 0x22:
		res = keyPressed(sf::Keyboard::Quote);
		break;
	case 0x26:
		res = keyPressed(sf::Keyboard::Num7);
		break;
	case 0x28:
		res = keyPressed(sf::Keyboard::Num9);
		break;
	case 0x29:
		res = keyPressed(sf::Keyboard::Num0);
		break;
	case 0x2A:
		res = keyPressed(sf::Keyboard::Num8);
		break;
	case 0x2B:
		res = keyPressed(sf::Keyboard::Equal);
		break;
	case 0x3A:
		res = keyPressed(sf::Keyboard::Semicolon);
		break;
	case 0x3C:
		res = keyPressed(sf::Keyboard::Comma);
		break;
	case 0x3E:
		res = keyPressed(sf::Keyboard::Period);
		break;
	case 0x3F:
		res = keyPressed(sf::Keyboard::Slash);
		break;
	case 0x40:
		res = keyPressed(sf::Keyboard::Num2);
		break;
	case 0x5E:
		res = keyPressed(sf::Keyboard::Num6);
		break;
	case 0x5F:
		res = keyPressed(sf::Keyboard::Subtract);
		break;
	case 0x7B:
		res = keyPressed(sf::Keyboard::LBracket);
		break;
	case 0x7C:
		res = keyPressed(sf::Keyboard::Backslash);
		break;
	case 0x7D:
		res = keyPressed(sf::Keyboard::RBracket);
		break;
	default:
		break;
	}

	if (key > 0x60 && key < 0x7B)
		return keyPressed((sf::Keyboard::Key)(key - 0x61));
	else if (key > 0x40 && key < 0x5B)
		res = keyPressed((sf::Keyboard::Key)(key - 0x41));
	else if (key > 0x2F && key < 0x3A)
		return keyPressed((sf::Keyboard::Key)(key - 0x16)) || keyPressed((sf::Keyboard::Key)(key + 0x1B));
	else if (key > 0x22 && key < 0x26)
		res = keyPressed((sf::Keyboard::Key)(key - 0x06));

	if (res || key == 0x90)
		return keyPressed(sf::Keyboard::LShift) || keyPressed(sf::Keyboard::RShift);
}

Keyboard::Keyboard(Processor * proc) : Peripheral(proc, Vendors::NYA_ELEKTRISKA, generickeyboard, vers) {

}

void Keyboard::interrupt(uint16_t command) {
	switch ((Commands)command) {
	case Commands::CLEAR_BUFFER:
		buffer.clear();
		return;
	case Commands::GET_NEXT:
		parent->registers.c = buffer.pop();
		return;
	case Commands::CHECK_KEY:
		if (checkKey(parent->registers.b))
			parent->registers.c = 1;
		else
			parent->registers.c = 0;
		return;
	case Commands::SET_INT:
		msg = parent->registers.b;
		return;
	case Commands::SET_MODE:
		mode = (KeyboardMode)(parent->registers.b);
		buffer.clear();
	}
}
