#include "z80.h"
#include "memory.h"
#include <iostream>

Z80::Z80(Memory *memory)
{
	for (uint8_t i=0; i<16; i++)
		general_purpose_registers[i] = 0xFF;

	flag_bytes= { 0x0, 0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,
				  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
			  	  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
				  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
				  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
				  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
				  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
				  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7};

	running = false;
	this->memory = memory;
	reset_register_pointers();
}

Z80::~Z80() { }

void Z80::execute()
{
	// Load instruction and increment program counter
	ir = memory->read8(pc);
	pc++;

	if (ir == 0x2F) // CPL. Invert A register
	{
		*a = ~*a;
		*f += flag_bits.halfcarry;
		*f += flag_bits.addsub;
	}
	/*
	 * TODO: increment commands need flag modifications
	 */
	if (ir == 0x3C) // inc register A
	{
		*a += 1;
	}

	if (ir == 0x4) // inc register B
	{
		*b += 1;
	}

	if (ir == 0xC) // inc register C
	{
		*c += 1;
	}

	if (ir == 0x14) // inc register D
	{
		*d += 1;
	}

	if (ir == 0x1C) // inc register E
	{
		*e += 1;
	}

	if (ir == 0x24) // inc register H
	{
		*h += 1;
	}

	if (ir == 0x2C) // inc register L
	{
		*l += 1;
	}

	if (ir == 0x0) { return; } // NOP

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
	f = (std::uint8_t*) &general_purpose_registers + 1;
	b = (std::uint8_t*) &general_purpose_registers + 2;
	c = (std::uint8_t*) &general_purpose_registers + 3;
	d = (std::uint8_t*) &general_purpose_registers + 4;
	e = (std::uint8_t*) &general_purpose_registers + 5;
	h = (std::uint8_t*) &general_purpose_registers + 6;
	l = (std::uint8_t*) &general_purpose_registers + 7;
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
