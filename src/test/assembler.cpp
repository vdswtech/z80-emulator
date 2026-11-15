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

TEST(AssemblerTest, nop)
{
	EXPECT_EQ(assembler.encode_instruction("NOP"), make_vec_1(0x00));
}

TEST(AssemblerTest, halt)
{
	EXPECT_EQ(assembler.encode_instruction("HALT"), make_vec_1(0x76));
}

TEST(AssemblerTest, inc)
{
	// reg8
	EXPECT_EQ(assembler.encode_instruction("INC B"), make_vec_1(0x04));
	EXPECT_EQ(assembler.encode_instruction("INC C"), make_vec_1(0x0C));
	EXPECT_EQ(assembler.encode_instruction("INC D"), make_vec_1(0x14));
	EXPECT_EQ(assembler.encode_instruction("INC E"), make_vec_1(0x1C));
	EXPECT_EQ(assembler.encode_instruction("INC H"), make_vec_1(0x24));
	EXPECT_EQ(assembler.encode_instruction("INC L"), make_vec_1(0x2C));
	EXPECT_EQ(assembler.encode_instruction("INC (HL)"), make_vec_1(0x34));
	EXPECT_EQ(assembler.encode_instruction("INC A"), make_vec_1(0x3C));

	// reg16
	EXPECT_EQ(assembler.encode_instruction("INC BC"), make_vec_1(0x03));
	EXPECT_EQ(assembler.encode_instruction("INC DE"), make_vec_1(0x13));
	EXPECT_EQ(assembler.encode_instruction("INC HL"), make_vec_1(0x23));
	EXPECT_EQ(assembler.encode_instruction("INC SP"), make_vec_1(0x33));

	// IX/IY
	EXPECT_EQ(assembler.encode_instruction("INC IX"), make_vec_2(0xDD, 0x23));
	EXPECT_EQ(assembler.encode_instruction("INC IY"), make_vec_2(0xFD, 0x23));
	
	// IX/IY indirect
	// TODO: implement test for this
}
