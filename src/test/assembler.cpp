#include "assembler.h"
#include <cstdint>
#include <vector>

std::vector<uint8_t> make_vec_1(uint8_t a)
{
	std::vector<uint8_t> ret = { a };
	return ret;
}

std::vector<uint8_t> make_vec_2(uint8_t a, uint8_t b)
{
	std::vector<uint8_t> ret = { a, b };
	return ret;
}

std::vector<uint8_t> make_vec_3(uint8_t a, uint8_t b, uint8_t c)
{
	std::vector<uint8_t> ret = { a, b, c };
	return ret;
}

Memory memory;
Assembler assembler(&memory);

TEST(AssemblerTest, nop_instruction)
{
	EXPECT_EQ(assembler.encode_instruction("NOP"), make_vec_1(0x00));
}

TEST(AssemblerTest, halt_instruction)
{
	EXPECT_EQ(assembler.encode_instruction("HALT"), make_vec_1(0x76));
}
