#include "z80.h"
#include "memory.h"

uint8_t FLAGS::result()
{
	uint8_t flag = 0;
	if (sign)
		flag |= 0b10000000;
	if (zero)
		flag |= 0b01000000;
	if (halfcarry)
		flag |= 0b00010000;
	if (parityoverflow)
		flag |= 0b00000100;
	if (addsubtract)
		flag |= 0b00000010;
	if (carry)
		flag |= 0b00000001;
	return flag;
}

void FLAGS::set_sign() { sign = true; }
void FLAGS::set_zero() { zero= true; }
void FLAGS::set_halfcarry() { halfcarry= true; }
void FLAGS::set_parityoverflow() { parityoverflow= true; }
void FLAGS::set_addsubtract() { addsubtract= true; }
void FLAGS::set_carry() { carry= true; }
void INSTRUCTION::set_flag(uint8_t* f) { (*f) = flags.result(); }

uint8_t INSTRUCTION::execute()
{
	return 0;
}

Z80::Z80(Memory *memory)
{
	for (uint8_t i=0; i<16; i++)
		general_purpose_registers[i] = 0xFF;
	running = false;
	this->memory = memory;
}

Z80::~Z80() { }

