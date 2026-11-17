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
	EXPECT_EQ(assembler.encode_instruction("nop"), make_vec_1(0x00));
}

TEST(AssemblerTest, halt)
{
	EXPECT_EQ(assembler.encode_instruction("halt"), make_vec_1(0x76));
}

TEST(AssemblerTest, inc)
{
	// reg8
	EXPECT_EQ(assembler.encode_instruction("inc b"), make_vec_1(0x04));
	EXPECT_EQ(assembler.encode_instruction("inc c"), make_vec_1(0x0c));
	EXPECT_EQ(assembler.encode_instruction("inc d"), make_vec_1(0x14));
	EXPECT_EQ(assembler.encode_instruction("inc e"), make_vec_1(0x1c));
	EXPECT_EQ(assembler.encode_instruction("inc h"), make_vec_1(0x24));
	EXPECT_EQ(assembler.encode_instruction("inc l"), make_vec_1(0x2c));
	EXPECT_EQ(assembler.encode_instruction("inc (hl)"), make_vec_1(0x34));
	EXPECT_EQ(assembler.encode_instruction("inc a"), make_vec_1(0x3c));

	// reg16
	EXPECT_EQ(assembler.encode_instruction("inc bc"), make_vec_1(0x03));
	EXPECT_EQ(assembler.encode_instruction("inc de"), make_vec_1(0x13));
	EXPECT_EQ(assembler.encode_instruction("inc hl"), make_vec_1(0x23));
	EXPECT_EQ(assembler.encode_instruction("inc sp"), make_vec_1(0x33));

	// IX/IY
	EXPECT_EQ(assembler.encode_instruction("inc ix"), make_vec_2(0xdd, 0x23));
	EXPECT_EQ(assembler.encode_instruction("inc iy"), make_vec_2(0xfd, 0x23));
	
	// IX/IY indirect
	// TODO: implement test for this
}
