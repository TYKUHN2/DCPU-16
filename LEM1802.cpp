#include "LEM1802.h"
#include "Debug.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

const uint16_t default_font[] = { //Ripped from https://gist.github.com/SylvainBoilard/4645708 THANKS!
	0xb79e, 0x388e, 0x722c, 0x75f4, 0x19bb, 0x7f8f, 0x85f9, 0xb158,
	0x242e, 0x2400, 0x082a, 0x0800, 0x0008, 0x0000, 0x0808, 0x0808,
	0x00ff, 0x0000, 0x00f8, 0x0808, 0x08f8, 0x0000, 0x080f, 0x0000,
	0x000f, 0x0808, 0x00ff, 0x0808, 0x08f8, 0x0808, 0x08ff, 0x0000,
	0x080f, 0x0808, 0x08ff, 0x0808, 0x6633, 0x99cc, 0x9933, 0x66cc,
	0xfef8, 0xe080, 0x7f1f, 0x0701, 0x0107, 0x1f7f, 0x80e0, 0xf8fe,
	0x5500, 0xaa00, 0x55aa, 0x55aa, 0xffaa, 0xff55, 0x0f0f, 0x0f0f,
	0xf0f0, 0xf0f0, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff,
	0x0000, 0x0000, 0x005f, 0x0000, 0x0300, 0x0300, 0x3e14, 0x3e00,
	0x266b, 0x3200, 0x611c, 0x4300, 0x3629, 0x7650, 0x0002, 0x0100,
	0x1c22, 0x4100, 0x4122, 0x1c00, 0x1408, 0x1400, 0x081c, 0x0800,
	0x4020, 0x0000, 0x0808, 0x0800, 0x0040, 0x0000, 0x601c, 0x0300,
	0x3e49, 0x3e00, 0x427f, 0x4000, 0x6259, 0x4600, 0x2249, 0x3600,
	0x0f08, 0x7f00, 0x2745, 0x3900, 0x3e49, 0x3200, 0x6119, 0x0700,
	0x3649, 0x3600, 0x2649, 0x3e00, 0x0024, 0x0000, 0x4024, 0x0000,
	0x0814, 0x2241, 0x1414, 0x1400, 0x4122, 0x1408, 0x0259, 0x0600,
	0x3e59, 0x5e00, 0x7e09, 0x7e00, 0x7f49, 0x3600, 0x3e41, 0x2200,
	0x7f41, 0x3e00, 0x7f49, 0x4100, 0x7f09, 0x0100, 0x3e41, 0x7a00,
	0x7f08, 0x7f00, 0x417f, 0x4100, 0x2040, 0x3f00, 0x7f08, 0x7700,
	0x7f40, 0x4000, 0x7f06, 0x7f00, 0x7f01, 0x7e00, 0x3e41, 0x3e00,
	0x7f09, 0x0600, 0x3e41, 0xbe00, 0x7f09, 0x7600, 0x2649, 0x3200,
	0x017f, 0x0100, 0x3f40, 0x3f00, 0x1f60, 0x1f00, 0x7f30, 0x7f00,
	0x7708, 0x7700, 0x0778, 0x0700, 0x7149, 0x4700, 0x007f, 0x4100,
	0x031c, 0x6000, 0x0041, 0x7f00, 0x0201, 0x0200, 0x8080, 0x8000,
	0x0001, 0x0200, 0x2454, 0x7800, 0x7f44, 0x3800, 0x3844, 0x2800,
	0x3844, 0x7f00, 0x3854, 0x5800, 0x087e, 0x0900, 0x4854, 0x3c00,
	0x7f04, 0x7800, 0x447d, 0x4000, 0x2040, 0x3d00, 0x7f10, 0x6c00,
	0x417f, 0x4000, 0x7c18, 0x7c00, 0x7c04, 0x7800, 0x3844, 0x3800,
	0x7c14, 0x0800, 0x0814, 0x7c00, 0x7c04, 0x0800, 0x4854, 0x2400,
	0x043e, 0x4400, 0x3c40, 0x7c00, 0x1c60, 0x1c00, 0x7c30, 0x7c00,
	0x6c10, 0x6c00, 0x4c50, 0x3c00, 0x6454, 0x4c00, 0x0836, 0x4100,
	0x0077, 0x0000, 0x4136, 0x0800, 0x0201, 0x0201, 0x0205, 0x0200
};

const uint16_t default_palette[] = { //Ripped from https://gist.github.com/SylvainBoilard/5027058 THANKS!
	0x0000, 0x000a, 0x00a0, 0x00aa, 0x0a00, 0x0a0a, 0x0a50, 0x0aaa,
	0x0555, 0x055f, 0x05f5, 0x05ff, 0x0f55, 0x0f5f, 0x0ff5, 0x0fff
};

void dumpFont(uint16_t * mem)
{
	for (char i = 0; i < 128; i++)
	{
		mem[i] = default_font[i];
	}
}

void dumpPalette(uint16_t * mem)
{
	for (char i = 0; i < 16; i++)
	{
		mem[i] = default_palette[i];
	}
}

uint32_t getFont(const uint16_t * ram, char offset)
{
	if (ram == nullptr)
	{
		return (default_font[offset] << 16) | default_font[offset];
	}
	else
	{
		return (ram[offset] << 16) | ram[offset];
	}
}

uint16_t getPalette(const uint16_t * ram, char offset)
{
	if (ram == nullptr)
	{
		return default_palette[offset];
	}
	else
	{
		return ram[offset];
	}
}

sf::Color compileColor(const uint16_t raw)
{
	uint8_t r = (raw >> 8) & 0xF;
	uint8_t g = (raw >> 4) & 0xF;
	uint8_t b = raw & 0xF;

	return sf::Color{ r, g, b };
}

LEM1802::LEM1802(Processor * parent) : Monitor(128, 96, 12), Peripheral(parent, 0x1C6C8B36, 0x734DF615, 0x1802)
{
}

LEM1802::~LEM1802()
{
}

void LEM1802::process() {
	Registers registers = parent->registers;
	uint16_t * memory = parent->memory;

	switch (registers.a)
	{
	case 0x0:
		if (registers.b == 0) {
			sram = nullptr;
			window.clear();
			break;
		}

		sram = memory + registers.b;
		break;

	case 0x1:
		if (registers.b == 0) {
			fram = nullptr;
			break;
		}

		fram = memory + registers.b;
		break;

	case 0x2:
		if (registers.b == 0) {
			pram = nullptr;
			break;
		}

		pram = memory + registers.b;
		break;

	case 0x3:
		border = registers.b & 0xF;
		break;

	case 0x4:
		dumpFont(memory + registers.b);
		parent->charge(256);
		break;

	case 0x5:
		dumpPalette(memory + registers.b);
		parent->charge(16);
		break;
	}

	parent->release();
}

sf::VertexArray compileCharacter(sf::Color fColor, sf::Color bColor, uint32_t glyph)
{
	sf::VertexArray character{sf::Points, 32};

	for (char i = 0; i < 4; i++)
	{
		uint8_t column = (glyph >> (i * 8)) & 0xFF; //Get column from the glyph and constrain to 8 bits

		for (char j = 0; j < 8; j++)
		{
			sf::Vector2f coords{ (float)i, (float)j };

			sf::Vertex pixel{ coords }; //Construct pixel at coordinates

			if (column & (1 << j))
			{
				pixel.color = fColor;
			}
			else
			{
				pixel.color = bColor;
			}

			character[(i * 8) + j] = pixel; //Write out pixel to buffer
		}
	}

	return character;
}

void LEM1802::draw(int x, int y, uint32_t glyph, uint16_t fg, uint16_t bg, bool blink)
{
	if (blink)
	{
		Debug::print("BLINK NOT IMPLEMENTED YET!");
	}

	//Preconstruct colors
	sf::Color fColor = compileColor(fg);
	sf::Color bColor = compileColor(bg);

	fColor = { 255,0,0 };
	bColor = { 255,0,0 };

	//Prepare cell for writing
	sf::RenderTexture cell;

	//Compile character
	sf::VertexArray character = compileCharacter(fColor, bColor, glyph);

	//Draw the cell to a texture
	cell.draw(character);
	sf::Texture cellTex = cell.getTexture();

	sf::RectangleShape realCell{ {4.0, 8.0} };
	realCell.setPosition(x, y);
	realCell.setTexture(&cellTex);

	//Draw out cell to screen
	window.draw(realCell);
}

void LEM1802::tick()
{
	if (sram == nullptr) //Screen is disconnected don't bother redrawing
	{
		return;
	}

	sf::Event waste;
	window.pollEvent(waste);

	window.clear();

	for (char x = 0; x < 32; x++)
	{
		for (char y = 0; y < 12; y++)
		{
			uint16_t character = sram[(x * 12) + y];

			uint32_t glyph = getFont(fram, character & 0x7F);
			uint16_t fg = getPalette(pram, character >> 12);
			uint16_t bg = getPalette(pram, (character >> 8) & 0xF);
			bool blink = (character >> 7) & 0x1;

			draw(x, y, glyph, fg, bg, blink);
		}
	}

	window.display();
}
