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

int encode_number(std::string_view number_string)
{
	int base {};
	int sign { 1 };
	std::string_view number_part { number_string };
	char start_char = number_string.front();
	char end_char = number_string.back();
	// handle sign
	if (start_char == '-')
	{
		sign = -1;
		number_part.remove_prefix(1);
		start_char = number_string.front();
	}
	else if (start_char == '+')
	{
		number_part.remove_prefix(1);
		start_char = number_string.front();
	}

	// handle base
	if (start_char == '%')
	{
		base = 2;
		number_part.remove_prefix(1);
	}
	else if (end_char == 'b')
	{
		base = 2;
		number_part.remove_suffix(1);
	}
	else if (start_char == 'o')
	{
		base = 8;
		number_part.remove_prefix(1);
	}
	else if (end_char == 'o')
	{
		base = 8;
		number_part.remove_suffix(1);
	}
	else if (start_char == '$')
	{
		base = 16;
		number_part.remove_prefix(1);
	}
	else if (end_char == 'h')
	{
		base = 16;
		number_part.remove_suffix(1);
	}
	else
	{
		base = 10;
		// no characters to remove
	}

	return sign * std::stoi(std::string(number_part), nullptr, base);
}

/**
 * Encode a string representing a number into a signed 8-bit number. Support binary, octal, hexadecimal, or decimal.
 *
 * @param number_string	The string containing the number to encode.
 * @return	The 8-bit number represented. Raise an exception if the number doesn't fit inside 8 bits.
 */
uint8_t encode_number_offset8(std::string_view number_string)
{
	int encoded_number = encode_number(number_string);
	if (encoded_number < INT8_MIN || encoded_number > INT8_MAX)
	{
		throw std::out_of_range(
			std::format("%s is outside the bounds of -128 to 127", number_string)
		);
	}
	else
	{
		union {
			int i;
			unsigned u;
		} x = { encoded_number };
		return static_cast<uint8_t>(x.u);
	}
}

/**
 * Encode a string representing a number into an unsigned 8-bit number. Support binary, octal, hexadecimal, or decimal.
 * 1010000b = %1010000 = 120o = o120 = 80 = 50h = $50
 *
 * @param number_string	The string containing the number to encode.
 * @return	The 8-bit number represented. Raise an exception if the number doesn't fit inside 8 bits.
 */
uint8_t encode_number8(std::string_view number_string)
{
	int encoded_number = encode_number(number_string);
	if (encoded_number < 0 || encoded_number > UINT8_MAX)
	{
		throw std::out_of_range(
			std::format("%s is outside the bounds of 0 to 255", number_string)
		);
	}
	else
	{
		return static_cast<uint8_t>(encoded_number);
	}
}

/**
 * Encode a string representing a number into an unsigned 16-bit number. Support binary, octal, hexadecimal, or decimal.
 *
 * @param number_string	The string containing the number to encode.
 * @return	The 16-bit number represented. Raise an exception if the number doesn't fit inside 16 bits.
 */
std::pair<uint8_t, uint8_t> encode_number16(std::string_view number_string)
{
	int encoded_number = encode_number(number_string);
	if (encoded_number < 0 || encoded_number > UINT16_MAX)
	{
		throw std::out_of_range(
			std::format("%s is outside the bounds of 0 to 65535", number_string)
		);
	}
	else
	{
		return std::pair(encoded_number & 0xFF, encoded_number >> 8);
	}
}

std::unordered_map<std::string_view, bytes_t> A_OPERAND1_TO_BYTES =
{
	{"(bc)", {0x0A}},
	{"(de)", {0x1A}},
	{"i", {0xED, 0x57}},
	{"r", {0xED, 0x5F}},
};

std::unordered_map<std::string_view, bytes_t> A_OPERAND2_TO_BYTES =
{
	{"(bc)", {0x02}},
	{"(de)", {0x12}},
	{"i", {0xED, 0x47}},
	{"r", {0xED, 0x4F}},
};

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
	else if (mnemonic == "ld")
	{
		// ld ss, nn 0b 00ss0001 nnnnnnnn nnnnnnnn
		// ld (nn), ss 0xED 01ss1011 nnnnnnnn nnnnnnnn
		// ld ss, (nn) 0xED 01ss0011 nnnnnnnn nnnnnnnn
		// ld (nn), a  0x32 nnnnnnnn nnnnnnnn
		// ld a, (nn)  0x3A nnnnnnnn nnnnnnnn
		//
		//
		// ld r, (ii+d) 0xDD/0xFD 10rrr110 dddddddd
		// ld (ii+d), r 0xDD/0xFD 10110rrr dddddddd
		// ld (ii+d), n 0xDD/0xFD 0x36 dddddddd nnnnnnnn
		// ld ii, nn 0xDD/0xFD 0x21 nnnnnnnn nnnnnnnn
		// ld (nn), ii 0xDD/0xFD 0x22 nnnnnnnn nnnnnnnn
		// ld ii, (nn) 0xDD/0xFD 0x2A nnnnnnnn nnnnnnnn
		// ld sp, ii 0xDD/0xFD 0xF9
		assert_operands_is_length(operands, 2);
		std::string_view operand1 = operands[0];
		std::string_view operand2 = operands[1];

		if (operand1 == "hl")
		{
			if (operand2.front() == '(')
			{
				// ld hl, (nn) 0x2A nnnnnnnn nnnnnnnn
				bytes.push_back(0x2A);
				operand2.remove_prefix(1);
				operand2.remove_suffix(1);
				auto [lo, hi] = encode_number16(operand2);
				bytes.push_back(lo);
				bytes.push_back(hi);
			}
			else
			{
				throw std::invalid_argument(
					std::format("Unknown instruction ld %s, %s (expected ld hl, (nn))", operand1, operand2)
				);
			}
		}
		else if (operand2 == "hl")
		{
			if (operand1 == "sp")
			{
				// ld sp, hl  0xF9
				bytes.push_back(0xF9);
			}
			else if (operand1.front() == '(')
			{
				// ld (nn), hl 0x22 nnnnnnnn nnnnnnnn
				bytes.push_back(0x22);
				operand1.remove_prefix(1);
				operand1.remove_suffix(1);
				auto [lo, hi] = encode_number16(operand1);
				bytes.push_back(lo);
				bytes.push_back(hi);
			}
			else
			{
				throw std::invalid_argument(
					std::format("Unknown instruction ld %s, %s (expected ld (nn), hl)", operand1, operand2)
				);
			}
		}
		//deal with special "a" ld's
		else if (operand1 == "a")
		{
			// ld a, (bc)  0x0A
			// ld a, (de)  0x1A
			// ld a, i 0xED 0x57
			// ld a, r 0xED 0x5F
			auto bytes_iterator = A_OPERAND1_TO_BYTES.find(operand2);
			if (bytes_iterator != A_OPERAND1_TO_BYTES.end())
			{
				return bytes_iterator->second;
			}
		}
		else if (operand2 == "a")
		{
			// ld (bc), a  0x02
			// ld (de), a  0x12
			// ld i, a 0xED 0x47
			// ld r, a 0xED 0x4F
			auto bytes_iterator = A_OPERAND2_TO_BYTES.find(operand1);
			if (bytes_iterator != A_OPERAND2_TO_BYTES.end())
			{
				return bytes_iterator->second;
			}
		}

		uint8_t op1_reg8_id = reg8_to_id(operand1);
		uint8_t op2_reg8_id = reg8_to_id(operand2);
		if (op1_reg8_id != 0xFF)
		{
			if (op2_reg8_id != 0xFF)
			{
				// ld r, r' 0b 01rrrRRR
				bytes.push_back(0b01000000 | (op1_reg8_id << 3) | op2_reg8_id);
			}
			else
			{
				// ld r, n 0b 00rrr110 nnnnnnnn
				bytes.push_back(0b00000110 | (op1_reg8_id << 3));
				bytes.push_back(encode_number8(operand2));
			}
		}
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
			if (operand[3] == ')')
			{
				// inc (ix), inc (iy)
				bytes.push_back(0x00);
			}
			else
			{
				bytes.push_back(encode_number_offset8(operand.substr(3)));
			}
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
			if (operand[3] == ')')
			{
				// dec (ix), dec (iy)
				bytes.push_back(0x00);
			}
			else
			{
				bytes.push_back(encode_number_offset8(operand.substr(3)));
			}
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
