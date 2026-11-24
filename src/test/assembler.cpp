#include "assembler.h"
#include <format>

/**
 * Takes a bytes_t object and returns a string containing the hexadecimal representation of those bytes.
 * Ex. { 0xFD, 0x42 } => "FD42"
 *
 * @param bytes	The bytes object.
 * @return		A string that represents the hexadecimal representation of the bytes.
 */
std::string bytes_to_string(bytes_t& bytes)
{
	std::string output {};
	for (uint8_t byte : bytes)
	{
		output += std::format("{:02X}", byte);
	}
	return output;
}

/**
 * Determine if the instruction assembles to the given bytes. Reports a sensible error message when it doesn't.
 *
 * @param instruction	  The string representing the instruction.
 * @param bytes			  The assembler's output for the bytes that represent the instruction.
 * @param expected_bytes  What the assembler's output is supposed to be.
 * @return				  True if bytes == expected_bytes, and an error message if they aren't equal.
 */
testing::AssertionResult ExpectInstructionAssemblesTo(std::string_view instruction, bytes_t& bytes, bytes_t& expected_bytes)
{
	if (bytes == expected_bytes)
	{
		return testing::AssertionSuccess();
	}
	else
	{
		return testing::AssertionFailure()
			<< instruction << " assembles to " << bytes_to_string(bytes)
			<< ", expected " << bytes_to_string(expected_bytes);
	}
}

class AssemblerTest : public testing::Test
{
	protected:
		Memory memory {};
		Assembler assembler { Assembler(&memory) };

		/**
		 * Test if the given instruction assembles to the given bytes.
		 *
		 * @param instruction	  The string representing the instruction.
		 * @param expected_bytes  What the assembler's output is supposed to be.
		 */
		void InstructionAssembleTest(std::string_view instruction, bytes_t expected_bytes)
		{
			bytes_t bytes = assembler.encode_instruction(instruction);
			EXPECT_TRUE(
				ExpectInstructionAssemblesTo(
					instruction,
					bytes,
					expected_bytes
				)
			);
		}
};

TEST_F(AssemblerTest, no_operand_instructions)
{
	InstructionAssembleTest("nop", { 0x00 });
	InstructionAssembleTest("rlca", { 0x07 });
	InstructionAssembleTest("rrca", { 0x0F });
	InstructionAssembleTest("rla", { 0x17 });
	InstructionAssembleTest("rra", { 0x1F });
	InstructionAssembleTest("daa", { 0x27 });
	InstructionAssembleTest("cpl", { 0x2F });
	InstructionAssembleTest("scf", { 0x37 });
	InstructionAssembleTest("ccf", { 0x3F });
	InstructionAssembleTest("halt", { 0x76 });
	InstructionAssembleTest("ret", { 0xC9 });
	InstructionAssembleTest("exx", { 0xD9 });
	InstructionAssembleTest("di", { 0xF3 });
	InstructionAssembleTest("ei", { 0xFB });
	InstructionAssembleTest("neg", { 0xED, 0x44 });
	InstructionAssembleTest("retn", { 0xED, 0x45 });
	InstructionAssembleTest("reti", { 0xED, 0x4D });
	InstructionAssembleTest("rrd", { 0xED, 0x67 });
	InstructionAssembleTest("rld", { 0xED, 0x6F });
	InstructionAssembleTest("ldi", { 0xED, 0xA0 });
	InstructionAssembleTest("cpi", { 0xED, 0xA1 });
	InstructionAssembleTest("ini", { 0xED, 0xA2 });
	InstructionAssembleTest("outi", { 0xED, 0xA3 });
	InstructionAssembleTest("ldd", { 0xED, 0xA8 });
	InstructionAssembleTest("cpd", { 0xED, 0xA9 });
	InstructionAssembleTest("ind", { 0xED, 0xAA });
	InstructionAssembleTest("outd", { 0xED, 0xAB });
	InstructionAssembleTest("ldir", { 0xED, 0xB0 });
	InstructionAssembleTest("cpir", { 0xED, 0xB1 });
	InstructionAssembleTest("inir", { 0xED, 0xB2 });
	InstructionAssembleTest("outir", { 0xED, 0xB3 });
	InstructionAssembleTest("lddr", { 0xED, 0xB8 });
	InstructionAssembleTest("cpdr", { 0xED, 0xB9 });
	InstructionAssembleTest("indr", { 0xED, 0xBA });
	InstructionAssembleTest("outdr", { 0xED, 0xBB });
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
