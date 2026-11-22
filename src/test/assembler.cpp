#include "assembler.h"

class AssemblerTest : public testing::Test
{
	protected:
		Memory memory {};
		Assembler assembler { Assembler(&memory) };

		void InstructionAssembleTest(std::string instruction, bytes_t value)
		{
			EXPECT_EQ(assembler.encode_instruction(instruction), value);
		}
};

TEST_F(AssemblerTest, nop)
{
	InstructionAssembleTest("nop", { 0x00 });
}

TEST_F(AssemblerTest, halt)
{
	InstructionAssembleTest("halt", { 0x76 });
}

TEST_F(AssemblerTest, inc_reg8)
{
	InstructionAssembleTest("inc b", { 0x04 });
	InstructionAssembleTest("inc c", { 0x0C });
	InstructionAssembleTest("inc d", { 0x14 });
	InstructionAssembleTest("inc e", { 0x1C });
	InstructionAssembleTest("inc h", { 0x24 });
	InstructionAssembleTest("inc l", { 0x2C });
	InstructionAssembleTest("inc (hl)", { 0x34 });
	InstructionAssembleTest("inc a", { 0x3C });
}

TEST_F(AssemblerTest, inc_reg16)
{
	InstructionAssembleTest("inc bc", { 0x03 });
	InstructionAssembleTest("inc de", { 0x13 });
	InstructionAssembleTest("inc hl", { 0x23 });
	InstructionAssembleTest("inc sp", { 0x33 });
}

TEST_F(AssemblerTest, inc_ixiy)
{
	InstructionAssembleTest("inc ix", { 0xDD, 0x23 });
	InstructionAssembleTest("inc iy", { 0xFD, 0x23 });
}

// TEST_F(AssemblerTest, inc_ixiy_indirect)
// {
// 	// TODO: implement test for this
// }

TEST_F(AssemblerTest, dec_reg8)
{
	InstructionAssembleTest("dec b", { 0x05 });
	InstructionAssembleTest("dec c", { 0x0D });
	InstructionAssembleTest("dec d", { 0x15 });
	InstructionAssembleTest("dec e", { 0x1D });
	InstructionAssembleTest("dec h", { 0x25 });
	InstructionAssembleTest("dec l", { 0x2D });
	InstructionAssembleTest("dec (hl)", { 0x35 });
	InstructionAssembleTest("dec a", { 0x3D });
}

TEST_F(AssemblerTest, dec_reg16)
{
	InstructionAssembleTest("dec bc", { 0x0B });
	InstructionAssembleTest("dec de", { 0x1B });
	InstructionAssembleTest("dec hl", { 0x2B });
	InstructionAssembleTest("dec sp", { 0x3B });
}

TEST_F(AssemblerTest, dec_ixiy)
{
	InstructionAssembleTest("dec ix", { 0xDD, 0x2B });
	InstructionAssembleTest("dec iy", { 0xFD, 0x2B });
}

// TEST_F(AssemblerTest, dec_ixiy_indirect)
// {
// 	// TODO: implement test for this
// }
