#include "assembler.h"
#include <stdexcept>
#include <unordered_map>

std::unordered_map<std::string, std::uint8_t> REG8_TO_ID =
{
	{"B", 0},
	{"C", 1},
	{"D", 2},
	{"E", 3},
	{"H", 4},
	{"L", 5},
	{"(HL)", 6},
	{"A", 7}
};

Assembler::Assembler(Memory *memory)
{
	mem_ptr = 0;
	this->memory = memory;
}

void assert_0_operands(std::string mnemonic, int end_of_mnemonic)
{
	if (end_of_mnemonic != mnemonic.npos)
	{
		throw std::invalid_argument("expected 0 operands, got more than 0");
	}
}

std::vector<std::uint8_t> Assembler::encode_instruction(std::string instruction)
{
	std::vector<std::uint8_t> bytes = {};
	int end_of_mnemonic = instruction.find(" ");
	std::string mnemonic = instruction.substr(0, end_of_mnemonic);
	if (mnemonic == "NOP")
	{
		assert_0_operands(mnemonic, end_of_mnemonic);
		bytes.push_back(0x0);
	}
	else if (mnemonic == "HALT")
	{
		assert_0_operands(mnemonic, end_of_mnemonic);
		bytes.push_back(0x76);
	}
	else if (mnemonic == "INC")
	{
		/*
		 * INC r (8-bit) 0b 00rrr100
		 * INC (HL)
		 * INC (IX+d) 0xDD 0x34 dd
		 * INC (IY+d) 0xFD 0x34 dd
		 *
		 * 
		 * INC rr (16-bit) 0b 00pp0011
		 * INC IX 0xDD 0x23
		 * INC IY 0xFD 0x23
		 */

	}
	return bytes;
}

void Assembler::write_all(std::vector<std::uint8_t> bytes)
{
	for (auto byte : bytes)
	{
		memory->write8(mem_ptr, byte);
		mem_ptr++;
	}
}
