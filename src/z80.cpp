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

void INSTRUCTION::set_sign() { flags.sign = true; }
void INSTRUCTION::set_zero() { flags.zero= true; }
void INSTRUCTION::set_halfcarry() { flags.halfcarry= true; }
void INSTRUCTION::set_parityoverflow() { flags.parityoverflow= true; }
void INSTRUCTION::set_addsubtract() { flags.addsubtract= true; }
void INSTRUCTION::set_carry() { flags.carry= true; }
void INSTRUCTION::unset_sign() { flags.sign = false; }
void INSTRUCTION::unset_zero() { flags.zero= false; }
void INSTRUCTION::unset_halfcarry() { flags.halfcarry= false; }
void INSTRUCTION::unset_parityoverflow() { flags.parityoverflow= false; }
void INSTRUCTION::unset_addsubtract() { flags.addsubtract= false; }
void INSTRUCTION::unset_carry() { flags.carry= false; }

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

