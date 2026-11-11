#include "z80.h"

Z80::Z80()
{
	// CPU Control
	pc = 0;
	sp = 0;
	running = false;

	// Registers
	a = (std::uint8_t*) &general_purpose_registers;
	f = (std::uint8_t*) general_purpose_registers + 1;
	b = (std::uint8_t*) general_purpose_registers + 2;
	c = (std::uint8_t*) general_purpose_registers + 3;
	d = (std::uint8_t*) general_purpose_registers + 4;
	e = (std::uint8_t*) general_purpose_registers + 5;
	h = (std::uint8_t*) general_purpose_registers + 6;
	l = (std::uint8_t*) general_purpose_registers + 7;
	af = (std::uint16_t*) &general_purpose_registers;
	bc = (std::uint16_t*) &general_purpose_registers + 2;
	de = (std::uint16_t*) &general_purpose_registers + 4;
	hl = (std::uint16_t*) &general_purpose_registers + 6;
}

Z80::~Z80()
{
}

void Z80::read_instruction()
{
}

void Z80::execute()
{
}

void Z80::step()
{
}

void Z80::reset()
{
	pc = 0;
	sp = 0;
	running = false;
}

uint8_t Z80::get_gp_register_state(register_pos position)
{
	return general_purpose_registers[position];
}
