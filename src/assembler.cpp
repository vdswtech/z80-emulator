#include "assembler.h"
#include <stdexcept>
#include <unordered_map>

std::unordered_map<std::string, uint8_t> REG8_TO_ID =
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

std::unordered_map<std::string, uint8_t> REG16_QQ_TO_ID = {
	{"BC", 0},
	{"DE", 1},
	{"HL", 2},
	{"AF", 3},
};

std::unordered_map<std::string, uint8_t> REG16_SS_TO_ID = {
	{"BC", 0},
	{"DE", 1},
	{"HL", 2},
	{"SP", 3},
};

std::unordered_map<std::string, uint8_t> REG16_PP_TO_ID = {
	{"BC", 0},
	{"DE", 1},
	{"IX", 2},
	{"SP", 3},
};

std::unordered_map<std::string, uint8_t> REG16_RR_TO_ID = {
	{"BC", 0},
	{"DE", 1},
	{"IY", 2},
	{"SP", 3},
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

std::string get_1_operand(std::string mnemonic, int end_of_mnemonic)
{
	/*
	 * inc a
	 *    ^
	 *    e
	 *    o
	 *    m
 	 */
	if (end_of_mnemonic == mnemonic.npos)
	{
		throw std::invalid_argument("expected 1 operand, got 0");
	}

	int comma_location = mnemonic.find(", ", end_of_mnemonic);

	if (comma_location != mnemonic.npos)
	{
		throw std::invalid_argument("expected 1 operand, got 2");
	}

	return mnemonic.substr(end_of_mnemonic + 1);
}

std::pair<std::string, std::string> get_2_operands(std::string mnemonic, int end_of_mnemonic)
{
	/*
	 * ld a, b
	 *   ^ ^
	 *   e c
	 *   o l
	 *   m
 	 */
	if (end_of_mnemonic == mnemonic.npos)
	{
		throw std::invalid_argument("expected 2 operands, got 0");
	}

	int comma_location = mnemonic.find(", ", end_of_mnemonic);

	if (comma_location == mnemonic.npos)
	{
		throw std::invalid_argument("expected 2 operands, got 1");
	}

	int first_operand_length = comma_location - end_of_mnemonic - 1;

	return std::pair<std::string, std::string>(
		mnemonic.substr(end_of_mnemonic + 1, first_operand_length),
		mnemonic.substr(comma_location + 2)
	);
}

std::vector<uint8_t> Assembler::encode_instruction(std::string instruction)
{
	std::vector<uint8_t> bytes = {};
	int end_of_mnemonic = instruction.find(' ');
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
		 * INC ss (16-bit) 0b 00ss0011
		 * INC IX 0xDD 0x23
		 * INC IY 0xFD 0x23
		 */

	}
	return bytes;
}

void Assembler::write_all(std::vector<uint8_t> bytes)
{
	for (auto byte : bytes)
	{
		memory->write8(mem_ptr, byte);
		mem_ptr++;
	}
}
