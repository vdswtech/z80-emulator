#include "assembler.h"
#include <cassert>
#include <stdexcept>
#include <unordered_map>

std::unordered_map<std::string, uint8_t> REG8_TO_ID =
{
	{"b", 0},
	{"c", 1},
	{"d", 2},
	{"e", 3},
	{"h", 4},
	{"l", 5},
	{"(hl)", 6},
	{"a", 7}
};

std::unordered_map<std::string, uint8_t> REG16_QQ_TO_ID = {
	{"bc", 0},
	{"de", 1},
	{"hl", 2},
	{"af", 3},
};

std::unordered_map<std::string, uint8_t> REG16_SS_TO_ID = {
	{"bc", 0},
	{"de", 1},
	{"hl", 2},
	{"sp", 3},
};

std::unordered_map<std::string, uint8_t> REG16_PP_TO_ID = {
	{"bc", 0},
	{"de", 1},
	{"ix", 2},
	{"sp", 3},
};

std::unordered_map<std::string, uint8_t> REG16_RR_TO_ID = {
	{"bc", 0},
	{"de", 1},
	{"iy", 2},
	{"sp", 3},
};

enum class Reg16Group {
	QQ,
	SS,
	PP,
	RR
};

uint8_t map_reg_to_id(std::unordered_map<std::string, uint8_t> reg_to_id, std::string reg)
{
	auto reg_id_iterator = reg_to_id.find(reg);
	if (reg_id_iterator != reg_to_id.end())
	{
		return reg_id_iterator->second;
	}
	else
	{
		return 0xFF;
	}
}

uint8_t reg8_to_id(std::string reg8)
{
	return map_reg_to_id(REG8_TO_ID, reg8);
}

uint8_t reg16_to_id(std::string reg16, Reg16Group group)
{
	switch (group)
	{
		case Reg16Group::QQ:
			return map_reg_to_id(REG16_QQ_TO_ID, reg16);
		case Reg16Group::SS:
			return map_reg_to_id(REG16_SS_TO_ID, reg16);
		case Reg16Group::PP:
			return map_reg_to_id(REG16_PP_TO_ID, reg16);
		case Reg16Group::RR:
			return map_reg_to_id(REG16_RR_TO_ID, reg16);
		default:
			throw std::invalid_argument("unknown Reg16Group");
	}
}

uint8_t get_index_reg_byte(char index_reg)
{
	switch (index_reg)
	{
		case 'x':
			return 0xDD;
		case 'y':
			return 0xFD;
		default:
			throw std::invalid_argument("Index register must be X or Y");
	}
}

Assembler::Assembler(Memory *memory)
{
	mem_ptr = 0;
	this->memory = memory;
}

void assert_0_operands(std::string instruction, size_t end_of_mnemonic)
{
	if (end_of_mnemonic != instruction.npos)
	{
		throw std::invalid_argument("expected 0 operands, got more than 0");
	}
}

std::string get_1_operand(std::string instruction, size_t end_of_mnemonic)
{
	/*
	 * inc a
	 *    ^
	 *    e
	 *    o
	 *    m
 	 */
	if (end_of_mnemonic == instruction.npos)
	{
		throw std::invalid_argument("expected 1 operand, got 0");
	}

	size_t comma_location = instruction.find(", ", end_of_mnemonic);

	if (comma_location != instruction.npos)
	{
		throw std::invalid_argument("expected 1 operand, got 2");
	}

	return instruction.substr(end_of_mnemonic + 1);
}

std::pair<std::string, std::string> get_2_operands(std::string instruction, size_t end_of_mnemonic)
{
	/*
	 * ld a, b
	 *   ^ ^
	 *   e c
	 *   o l
	 *   m
 	 */
	if (end_of_mnemonic == instruction.npos)
	{
		throw std::invalid_argument("expected 2 operands, got 0");
	}

	size_t comma_location = instruction.find(", ", end_of_mnemonic);

	if (comma_location == instruction.npos)
	{
		throw std::invalid_argument("expected 2 operands, got 1");
	}

	size_t first_operand_length = comma_location - end_of_mnemonic - 1;

	return std::pair<std::string, std::string>(
		instruction.substr(end_of_mnemonic + 1, first_operand_length),
		instruction.substr(comma_location + 2)
	);
}

std::vector<uint8_t> Assembler::encode_instruction(std::string instruction)
{
	std::vector<uint8_t> bytes = {};
	int end_of_mnemonic = instruction.find(' ');
	std::string mnemonic = instruction.substr(0, end_of_mnemonic);
	if (mnemonic == "nop")
	{
		assert_0_operands(mnemonic, end_of_mnemonic);
		bytes.push_back(0x0);
	}
	else if (mnemonic == "halt")
	{
		assert_0_operands(mnemonic, end_of_mnemonic);
		bytes.push_back(0x76);
	}
	else if (mnemonic == "inc")
	{
		std::string operand = get_1_operand(instruction, end_of_mnemonic);
		// first, handle ix/iy
		// starts with "i"
		if (operand.rfind("i", 0) == 0)
		{
			// inc ix 0xDD 0x23
			// inc iy 0xFD 0x23
			bytes.push_back(get_index_reg_byte(operand[1]));
			bytes.push_back(0x23);
		}
		// then, (ix+d) and (iy+d)
		// starts with "(i"
		else if (operand.rfind("(i", 0) == 0)
		{
			// inc (ix+d) 0xDD 0x34 dd
			// inc (iy+d) 0xFD 0x34 dd
			bytes.push_back(get_index_reg_byte(operand[2]));
			bytes.push_back(0x34);
			// TODO: implement finding and writing the offset
		}
		else if (uint8_t reg8_id = reg8_to_id(operand); reg8_id != 0xFF)
		{
			// inc r (8-bit) 0b 00rrr100
			// inc (hl)
			bytes.push_back(0b00000100 | (reg8_id << 3));
		}
		else if (uint8_t reg16_id = reg16_to_id(operand, Reg16Group::SS); reg16_id != 0xFF)
		{
			// inc ss (16-bit) 0b 00ss0011
			bytes.push_back(0b00000011 | (reg16_id << 4));
		}
		else
		{
			//otherwise, the operand is invalid
			throw std::invalid_argument("unknown operand " + operand + " to inc");
		}
	}
	else if (mnemonic == "dec")
	{
		std::string operand = get_1_operand(instruction, end_of_mnemonic);
		// first, handle ix/iy
		// starts with "i"
		if (operand.rfind("i", 0) == 0)
		{
			// dec ix 0xDD 0x2B
			// dec iy 0xFD 0x2B
			bytes.push_back(get_index_reg_byte(operand[1]));
			bytes.push_back(0x2B);
		}
		// then, (ix+d) and (iy+d)
		// starts with "(i"
		else if (operand.rfind("(i", 0) == 0)
		{
			// dec (ix+d) 0xDD 0x35 dd
			// dec (iy+d) 0xFD 0x35 dd
			bytes.push_back(get_index_reg_byte(operand[2]));
			bytes.push_back(0x35);
			// TODO: implement finding and writing the offset
		}
		else if (uint8_t reg8_id = reg8_to_id(operand); reg8_id != 0xFF)
		{
			// dec r (8-bit) 0b 00rrr101
			// dec (hl)
			bytes.push_back(0b00000101 | (reg8_id << 3));
		}
		else if (uint8_t reg16_id = reg16_to_id(operand, Reg16Group::SS); reg16_id != 0xFF)
		{
			// dec ss (16-bit) 0b 00ss1011
			bytes.push_back(0b00001011 | (reg16_id << 4));
		}
		else
		{
			//otherwise, the operand is invalid
			throw std::invalid_argument("unknown operand " + operand + " to dec");
		}
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
