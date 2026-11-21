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

	if (ir == 0x3C) { inc8(a); }                  // inc register A
	if (ir == 0x4) { inc8(b); }                   // inc register B
	if (ir == 0xC) { inc8(c); }                   // inc register C
	if (ir == 0x14) { inc8(d); }                  // inc register D
	if (ir == 0x1C) { inc8(e); }                  // inc register E
	if (ir == 0x24) { inc8(h); }                  // inc register H
	if (ir == 0x2C) { inc8(l); }                  // inc register L
	if (ir == 0x3) { inc16(bc); }                 // inc register BC
	if (ir == 0x13) { inc16(de); }                // inc register DE
	if (ir == 0x23) { inc16(hl); }                // inc register HL
	if (ir == 0x33) { inc16(&sp); }               // inc register SP
	if (ir == 0xDD && ir+1 == 23) { inc16(&ix); } // inc register ix
	if (ir == 0xFD && ir+1 == 23) { inc16(&iy); } // inc register iy

	if (ir == 0x0) { return; } // NOP

	if (ir == 0x4C) // Halt
	{
		running = false;
		return;
	}
	ir++;
}

bool Z80::f_c_is_set() { return ((*f) & (1 << 0)) != 0; }
bool Z80::f_n_is_set() { return ((*f) & (1 << 1)) != 0; }
bool Z80::f_pv_is_set() { return ((*f) & (1 << 2)) != 0; }
bool Z80::f_h_is_set() { return ((*f) & (1 << 4)) != 0; }
bool Z80::f_z_is_set() { return ((*f) & (1 << 6)) != 0; }
bool Z80::f_s_is_set() { return ((*f) & (1 << 7)) != 0; }

void Z80::inc8(uint8_t* reg)
{
	bool rollover = ((*reg) == 0xFF);
	(*reg) += 1;
	update_f8(reg, rollover);
}

void Z80::inc16(uint16_t* reg)
{
	(*reg) += 1;
}

void Z80::update_f8(uint8_t *reg8, bool rollover)
{
	uint8_t flags = 0x0;
	if (rollover)
	{
		flags |= flag_bits.pv;
	}
	if ((*reg8) == 0)
	{
		flags |= flag_bits.zero;
	}
	if (((*reg8) & 0b0000) == 0)
	{
		flags |= flag_bits.halfcarry;
	}
	(*f) = flags;
}

void Z80::step()
{
	/*
	 * If step is called and the program counter is set to zero, start the program
	 * Run execute until running flag is set to false
	 */
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
		general_purpose_registers[i] = 0xFF;
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

bool Z80::run_status() { return running; }
uint8_t Z80::get_gp_register_state(register_pos position) { return general_purpose_registers[position]; }
uint8_t Z80::get_ir() { return ir; }
uint16_t Z80::get_pc() { return pc; }
uint16_t Z80::get_sp() { return sp; }
uint16_t Z80::get_ix() { return ix; }
uint16_t Z80::get_iy() { return iy; }
uint16_t Z80::get_af() { return *af; }
uint16_t Z80::get_bc() { return *bc; }
uint16_t Z80::get_de() { return *de; }
uint16_t Z80::get_hl() { return *hl; }
