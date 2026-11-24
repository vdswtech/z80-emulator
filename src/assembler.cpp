#include "assembler.h"
#include <cassert>
#include <format>
#include <stdexcept>
#include <unordered_map>

std::unordered_map<std::string_view, uint8_t> REG8_TO_ID =
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

std::unordered_map<std::string_view, uint8_t> REG16_QQ_TO_ID = {
	{"bc", 0},
	{"de", 1},
	{"hl", 2},
	{"af", 3},
};

std::unordered_map<std::string_view, uint8_t> REG16_SS_TO_ID = {
	{"bc", 0},
	{"de", 1},
	{"hl", 2},
	{"sp", 3},
};

std::unordered_map<std::string_view, uint8_t> REG16_PP_TO_ID = {
	{"bc", 0},
	{"de", 1},
	{"ix", 2},
	{"sp", 3},
};

std::unordered_map<std::string_view, uint8_t> REG16_RR_TO_ID = {
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

uint8_t map_reg_to_id(std::unordered_map<std::string_view, uint8_t> reg_to_id, std::string_view reg)
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

uint8_t reg8_to_id(std::string_view reg8)
{
	return map_reg_to_id(REG8_TO_ID, reg8);
}

uint8_t reg16_to_id(std::string_view reg16, Reg16Group group)
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
			throw std::invalid_argument(std::format("unknown index register i%s", index_reg));
	}
}

Assembler::Assembler(Memory *memory)
{
	mem_ptr = 0;
	this->memory = memory;
}

std::vector<std::string_view> get_operands(std::string_view instruction, size_t end_of_mnemonic)
{
	if (end_of_mnemonic == instruction.npos)
	{
		return {};
	}

	size_t comma_location = instruction.find(", ", end_of_mnemonic);

	if (comma_location == instruction.npos)
	{
		/*
		 * inc a
		 *    ^
		 *    e
		 *    o
		 *    m
		 */
		return { instruction.substr(end_of_mnemonic + 1) };
	}
	else
	{
		/*
		 * ld a, b
		 *   ^ ^
		 *   e c
		 *   o l
		 *   m
		 */
		assert(comma_location != instruction.npos);
		size_t first_operand_length = comma_location - end_of_mnemonic - 1;
		return {
			instruction.substr(end_of_mnemonic + 1, first_operand_length),
			instruction.substr(comma_location + 2)
		};
	}
}

void assert_operands_is_length(std::vector<std::string_view>& operands, size_t length)
{
	size_t operands_length = operands.size();
	if (operands_length != length)
	{
		throw std::invalid_argument(std::format("expected %zu operands, got %zu", length, operands_length));
	}
}

void assert_operands_is_between_length(std::vector<std::string_view>& operands, size_t low, size_t high)
{
	size_t operands_length = operands.size();
	if (operands_length < low || operands_length > high)
	{
		throw std::invalid_argument(std::format("expected %zu-%zu operands, got %zu", low, high, operands_length));
	}
}

std::unordered_map<std::string_view, bytes_t> MNEMONIC_TO_BYTES_0_OPERANDS =
{
	{"nop", {0x00}},
	{"rlca", {0x07}},
	{"rrca", {0x0F}},
	{"rla", {0x17}},
	{"rra", {0x1F}},
	{"daa", {0x27}},
	{"cpl", {0x2F}},
	{"scf", {0x37}},
	{"ccf", {0x3F}},
	{"halt", {0x76}},
	{"ret", {0xC9}},
	{"exx", {0xD9}},
	{"di", {0xF3}},
	{"ei", {0xFB}},
	{"neg", {0xED, 0x44}},
	{"retn", {0xED, 0x45}},
	{"reti", {0xED, 0x4D}},
	{"rrd", {0xED, 0x67}},
	{"rld", {0xED, 0x6F}},
	{"ldi", {0xED, 0xA0}},
	{"cpi", {0xED, 0xA1}},
	{"ini", {0xED, 0xA2}},
	{"outi", {0xED, 0xA3}},
	{"ldd",  {0xED, 0xA8}},
	{"cpd",  {0xED, 0xA9}},
	{"ind",  {0xED, 0xAA}},
	{"outd", {0xED, 0xAB}},
	{"ldir",  {0xED, 0xB0}},
	{"cpir",  {0xED, 0xB1}},
	{"inir",  {0xED, 0xB2}},
	{"outir", {0xED, 0xB3}},
	{"lddr",  {0xED, 0xB8}},
	{"cpdr",  {0xED, 0xB9}},
	{"indr",  {0xED, 0xBA}},
	{"outdr", {0xED, 0xBB}},
};

bytes_t encode_0_operand_instruction(std::string_view instruction)
{
	auto bytes_iterator = MNEMONIC_TO_BYTES_0_OPERANDS.find(instruction);
	if (bytes_iterator != MNEMONIC_TO_BYTES_0_OPERANDS.end())
	{
		return bytes_iterator->second;
	}
	else
	{
		throw std::invalid_argument(std::format("Unknown 0-operand instruction %s", instruction));
	}
}

bytes_t Assembler::encode_instruction(std::string_view instruction)
{
	bytes_t bytes = {};
	int end_of_mnemonic = instruction.find(' ');
	std::string_view mnemonic = instruction.substr(0, end_of_mnemonic);
	std::vector<std::string_view> operands = get_operands(instruction, end_of_mnemonic);
	if (operands.size() == 0)
	{
		return encode_0_operand_instruction(instruction);
	}
	else if (mnemonic == "inc")
	{
		assert_operands_is_length(operands, 1);
		std::string_view operand = operands[0];
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
			throw std::invalid_argument(std::format("unknown operand %s to inc", operand));
		}
	}
	else if (mnemonic == "dec")
	{
		assert_operands_is_length(operands, 1);
		std::string_view operand = operands[0];
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
			throw std::invalid_argument(std::format("unknown operand %s to dec", operand));
		}
	}
	return bytes;
}

void Assembler::write_all(bytes_t bytes)
{
	for (auto byte : bytes)
	{
		memory->write8(mem_ptr, byte);
		mem_ptr++;
	}
}
