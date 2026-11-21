#include "assembler.h"
#include <cstdint>

bytes_t make_vec_1(uint8_t a)
{
	bytes_t ret = { a };
	return ret;
}

bytes_t make_vec_2(uint8_t a, uint8_t b)
{
	bytes_t ret = { a, b };
	return ret;
}

bytes_t make_vec_3(uint8_t a, uint8_t b, uint8_t c)
{
	bytes_t ret = { a, b, c };
	return ret;
}

Memory memory;
Assembler assembler(&memory);

TEST(AssemblerTest, nop)
{
	EXPECT_EQ(assembler.encode_instruction("nop"), bytes_t { 0x00 });
}

TEST(AssemblerTest, halt)
{
	EXPECT_EQ(assembler.encode_instruction("halt"), make_vec_1(0x76));
}

TEST(AssemblerTest, inc_reg8)
{
	EXPECT_EQ(assembler.encode_instruction("inc b"), make_vec_1(0x04));
	EXPECT_EQ(assembler.encode_instruction("inc c"), make_vec_1(0x0C));
	EXPECT_EQ(assembler.encode_instruction("inc d"), make_vec_1(0x14));
	EXPECT_EQ(assembler.encode_instruction("inc e"), make_vec_1(0x1C));
	EXPECT_EQ(assembler.encode_instruction("inc h"), make_vec_1(0x24));
	EXPECT_EQ(assembler.encode_instruction("inc l"), make_vec_1(0x2C));
	EXPECT_EQ(assembler.encode_instruction("inc (hl)"), make_vec_1(0x34));
	EXPECT_EQ(assembler.encode_instruction("inc a"), make_vec_1(0x3C));
}

TEST(AssemblerTest, inc_reg16)
{
	EXPECT_EQ(assembler.encode_instruction("inc bc"), make_vec_1(0x03));
	EXPECT_EQ(assembler.encode_instruction("inc de"), make_vec_1(0x13));
	EXPECT_EQ(assembler.encode_instruction("inc hl"), make_vec_1(0x23));
	EXPECT_EQ(assembler.encode_instruction("inc sp"), make_vec_1(0x33));
}

TEST(AssemblerTest, inc_ixiy)
{
	EXPECT_EQ(assembler.encode_instruction("inc ix"), make_vec_2(0xDD, 0x23));
	EXPECT_EQ(assembler.encode_instruction("inc iy"), make_vec_2(0xFD, 0x23));
}

// TEST(AssemblerTest, inc_ixiy_indirect)
// {
// 	// TODO: implement test for this
// }

TEST(AssemblerTest, dec_reg8)
{
	EXPECT_EQ(assembler.encode_instruction("dec b"), make_vec_1(0x05));
	EXPECT_EQ(assembler.encode_instruction("dec c"), make_vec_1(0x0D));
	EXPECT_EQ(assembler.encode_instruction("dec d"), make_vec_1(0x15));
	EXPECT_EQ(assembler.encode_instruction("dec e"), make_vec_1(0x1D));
	EXPECT_EQ(assembler.encode_instruction("dec h"), make_vec_1(0x25));
	EXPECT_EQ(assembler.encode_instruction("dec l"), make_vec_1(0x2D));
	EXPECT_EQ(assembler.encode_instruction("dec (hl)"), make_vec_1(0x35));
	EXPECT_EQ(assembler.encode_instruction("dec a"), make_vec_1(0x3D));
}

TEST(AssemblerTest, dec_reg16)
{
	EXPECT_EQ(assembler.encode_instruction("dec bc"), make_vec_1(0x0B));
	EXPECT_EQ(assembler.encode_instruction("dec de"), make_vec_1(0x1B));
	EXPECT_EQ(assembler.encode_instruction("dec hl"), make_vec_1(0x2B));
	EXPECT_EQ(assembler.encode_instruction("dec sp"), make_vec_1(0x3B));
}

TEST(AssemblerTest, dec_ixiy)
{
	EXPECT_EQ(assembler.encode_instruction("dec ix"), make_vec_2(0xDD, 0x2B));
	EXPECT_EQ(assembler.encode_instruction("dec iy"), make_vec_2(0xFD, 0x2B));
}

// TEST(AssemblerTest, dec_ixiy_indirect)
// {
// 	// TODO: implement test for this
// }
