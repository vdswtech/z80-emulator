#include "z80.h"
#include "memory.h"

Z80::Z80()
{
	// CPU Control
	pc = 0;
	sp = 65535;
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
	ix=0;
	iy=0;
	running = false;
	for (uint8_t i=0; i<16; i++)
		general_purpose_registers[i] = 0;
}

uint8_t Z80::get_gp_register_state(register_pos position)
{
	return general_purpose_registers[position];
}
uint16_t Z80::get_pc()
{
	return pc;
}

uint16_t Z80::get_sp()
{
	return sp;
}

uint16_t Z80::get_ix()
{
	return ix;
}

uint16_t Z80::get_iy()
{
	return iy;
}

uint16_t Z80::get_af()
{
	return *af;
}

uint16_t Z80::get_bc()
{
	return *bc;
}

uint16_t Z80::get_de()
{
	return *de;
}

uint16_t Z80::get_hl()
{
	return *hl;
}
