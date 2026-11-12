#include "z80.h"
#include "memory.h"
#include <iostream>

Z80::Z80(Memory *memory)
{
	this->memory = memory;
	reset_register_pointers();
}

Z80::~Z80() { }

void Z80::execute()
{
	// Load instruction and increment program counter
	ir = memory->read8(pc);
	pc++;

	if (ir == 0x0) // NOP
	{
		return;
	}
	if (ir == 0x4C) // Halt
	{
		running = false;
		return;
	}
}

void Z80::step()
{
	if (pc == 0)
		running = true;
	if (running)
		execute();
	return;
}

void Z80::reset()
{
	running = false;

	// Special Purpose Registers
	i = 0xFF;
	r = 0xFF;
	pc = 0;
	sp = 0xFFFF;
	ix = 0xFFFF;
	iy = 0xFFFF;

	// General Purpose Registers
	for (std::uint8_t i = 0; i < 16; i++)
	{
		general_purpose_registers[i] = 0xFF;
	}
	reset_register_pointers();
}

void Z80::reset_register_pointers()
{
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

bool Z80::run_status()
{
	return running;
}

uint8_t Z80::get_gp_register_state(register_pos position)
{
	return general_purpose_registers[position];
}

uint8_t Z80::get_ir()
{
	return ir;
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
