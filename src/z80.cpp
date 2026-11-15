#include "z80.h"
#include "memory.h"
#include <iostream>

Z80::Z80(Memory *memory)
{
	for (uint8_t i=0; i<16; i++)
		general_purpose_registers[i] = 0xFF;

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
	if (ir == 0x3C) { inc(a); } // inc register A

	if (ir == 0x4) { inc(b); } // inc register B

	if (ir == 0xC) { inc(c); } // inc register C

	if (ir == 0x14) { inc(d); } // inc register D

	if (ir == 0x1C) { inc(e); } // inc register E

	if (ir == 0x24) { inc(h); } // inc register H

	if (ir == 0x2C) { inc(l); } // inc register L

	if (ir == 0x0) { return; } // NOP

	if (ir == 0x4C) // Halt
	{
		running = false;
		return;
	}
}

bool Z80::f_c_is_set()
{
	return ((*f) & (1 << 0)) != 0;
}

bool Z80::f_n_is_set()
{
	return ((*f) & (1 << 1)) != 0;
}

bool Z80::f_pv_is_set()
{
	return ((*f) & (1 << 2)) != 0;
}

bool Z80::f_h_is_set()
{
	return ((*f) & (1 << 4)) != 0;
}

bool Z80::f_z_is_set()
{
	return ((*f) & (1 << 6)) != 0;
}

bool Z80::f_s_is_set()
{
	return ((*f) & (1 << 7)) != 0;
}

void Z80::inc(uint8_t* reg)
{
	uint8_t flags = 0;
	if (*reg == 0x7F)
	{
		flags += 0x4;
	}

	*reg += 1;

	if (*reg == 0)
	{
		flags -= 0x80;
		flags += 0x40;
	}
	if (*reg > 0)
	{
		flags -= 0x80;
		flags -= 0x40;
	}
	if (f_n_is_set())
	{
		flags += 0x2;
	}
	update_f(flags);
}

void Z80::update_f(uint8_t value)
{
	*f = value;
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
	for (uint8_t i = 0; i < 16; i++)
	{
		general_purpose_registers[i] = 0xFF;
	}
	reset_register_pointers();
}

void Z80::reset_register_pointers()
{
	a = (uint8_t*) &general_purpose_registers;
	f = (uint8_t*) &general_purpose_registers + 1;
	b = (uint8_t*) &general_purpose_registers + 2;
	c = (uint8_t*) &general_purpose_registers + 3;
	d = (uint8_t*) &general_purpose_registers + 4;
	e = (uint8_t*) &general_purpose_registers + 5;
	h = (uint8_t*) &general_purpose_registers + 6;
	l = (uint8_t*) &general_purpose_registers + 7;
	af = (uint16_t*) &general_purpose_registers;
	bc = (uint16_t*) &general_purpose_registers + 2;
	de = (uint16_t*) &general_purpose_registers + 4;
	hl = (uint16_t*) &general_purpose_registers + 6;
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
