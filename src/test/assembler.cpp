#include "assembler.h"
#include <format>
#include <gtest/gtest.h>

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

TEST_F(AssemblerTest, inc_ixiy_indirect)
{
	InstructionAssembleTest("inc (ix)", { 0xDD, 0x34, 0x00 });
	InstructionAssembleTest("inc (iy)", { 0xFD, 0x34, 0x00 });
	InstructionAssembleTest("inc (ix+127)", { 0xDD, 0x34, 0x7F });
	InstructionAssembleTest("inc (iy-128)", { 0xFD, 0x34, 0x80 });
}

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


TEST_F(AssemblerTest, dec_ixiy_indirect)
{
	InstructionAssembleTest("dec (ix)", { 0xDD, 0x35, 0x00 });
	InstructionAssembleTest("dec (iy)", { 0xFD, 0x35, 0x00 });
	InstructionAssembleTest("dec (ix+127)", { 0xDD, 0x35, 0x7F });
	InstructionAssembleTest("dec (iy-128)", { 0xFD, 0x35, 0x80 });
}

TEST_F(AssemblerTest, ld_hl)
{
	InstructionAssembleTest("ld sp, hl", { 0xF9 });
	InstructionAssembleTest("ld (1234h), hl", { 0x22, 0x34, 0x12 });
	InstructionAssembleTest("ld hl, (1234h)", { 0x2A, 0x34, 0x12 });
}

TEST_F(AssemblerTest, ld_a_special)
{
	InstructionAssembleTest("ld a, (bc)", { 0x0A });
	InstructionAssembleTest("ld a, (de)", { 0x1A });
	InstructionAssembleTest("ld a, i", { 0xED, 0x57 });
	InstructionAssembleTest("ld a, r", { 0xED, 0x5F });
	InstructionAssembleTest("ld (bc), a", { 0x02 });
	InstructionAssembleTest("ld (de), a", { 0x12 });
	InstructionAssembleTest("ld i, a", { 0xED, 0x47 });
	InstructionAssembleTest("ld r, a", { 0xED, 0x4F });
}

TEST_F(AssemblerTest, ld_r_rprime)
{
	InstructionAssembleTest("ld b, b",     { 0x40 });
	InstructionAssembleTest("ld b, c",     { 0x41 });
	InstructionAssembleTest("ld b, d",     { 0x42 });
	InstructionAssembleTest("ld b, e",     { 0x43 });
	InstructionAssembleTest("ld b, h",     { 0x44 });
	InstructionAssembleTest("ld b, l",     { 0x45 });
	InstructionAssembleTest("ld b, (hl)",  { 0x46 });
	InstructionAssembleTest("ld b, a",     { 0x47 });
	InstructionAssembleTest("ld c, b",     { 0x48 });
	InstructionAssembleTest("ld c, c",     { 0x49 });
	InstructionAssembleTest("ld c, d",     { 0x4A });
	InstructionAssembleTest("ld c, e",     { 0x4B });
	InstructionAssembleTest("ld c, h",     { 0x4C });
	InstructionAssembleTest("ld c, l",     { 0x4D });
	InstructionAssembleTest("ld c, (hl)",  { 0x4E });
	InstructionAssembleTest("ld c, a",     { 0x4F });
	InstructionAssembleTest("ld d, b",     { 0x50 });
	InstructionAssembleTest("ld d, c",     { 0x51 });
	InstructionAssembleTest("ld d, d",     { 0x52 });
	InstructionAssembleTest("ld d, e",     { 0x53 });
	InstructionAssembleTest("ld d, h",     { 0x54 });
	InstructionAssembleTest("ld d, l",     { 0x55 });
	InstructionAssembleTest("ld d, (hl)",  { 0x56 });
	InstructionAssembleTest("ld d, a",     { 0x57 });
	InstructionAssembleTest("ld e, b",     { 0x58 });
	InstructionAssembleTest("ld e, c",     { 0x59 });
	InstructionAssembleTest("ld e, d",     { 0x5A });
	InstructionAssembleTest("ld e, e",     { 0x5B });
	InstructionAssembleTest("ld e, h",     { 0x5C });
	InstructionAssembleTest("ld e, l",     { 0x5D });
	InstructionAssembleTest("ld e, (hl)",  { 0x5E });
	InstructionAssembleTest("ld e, a",     { 0x5F });
	InstructionAssembleTest("ld h, b",     { 0x60 });
	InstructionAssembleTest("ld h, c",     { 0x61 });
	InstructionAssembleTest("ld h, d",     { 0x62 });
	InstructionAssembleTest("ld h, e",     { 0x63 });
	InstructionAssembleTest("ld h, h",     { 0x64 });
	InstructionAssembleTest("ld h, l",     { 0x65 });
	InstructionAssembleTest("ld h, (hl)",  { 0x66 });
	InstructionAssembleTest("ld h, a",     { 0x67 });
	InstructionAssembleTest("ld l, b",     { 0x68 });
	InstructionAssembleTest("ld l, c",     { 0x69 });
	InstructionAssembleTest("ld l, d",     { 0x6A });
	InstructionAssembleTest("ld l, e",     { 0x6B });
	InstructionAssembleTest("ld l, h",     { 0x6C });
	InstructionAssembleTest("ld l, l",     { 0x6D });
	InstructionAssembleTest("ld l, (hl)",  { 0x6E });
	InstructionAssembleTest("ld l, a",     { 0x6F });
	InstructionAssembleTest("ld (hl), b",  { 0x70 });
	InstructionAssembleTest("ld (hl), c",  { 0x71 });
	InstructionAssembleTest("ld (hl), d",  { 0x72 });
	InstructionAssembleTest("ld (hl), e",  { 0x73 });
	InstructionAssembleTest("ld (hl), h",  { 0x74 });
	InstructionAssembleTest("ld (hl), l",  { 0x75 });
	// InstructionAssembleTest("halt",     { 0x76 });
	InstructionAssembleTest("ld (hl), a",  { 0x77 });
	InstructionAssembleTest("ld a, b",     { 0x78 });
	InstructionAssembleTest("ld a, c",     { 0x79 });
	InstructionAssembleTest("ld a, d",     { 0x7A });
	InstructionAssembleTest("ld a, e",     { 0x7B });
	InstructionAssembleTest("ld a, h",     { 0x7C });
	InstructionAssembleTest("ld a, l",     { 0x7D });
	InstructionAssembleTest("ld a, (hl)",  { 0x7E });
	InstructionAssembleTest("ld a, a",     { 0x7F });
}

TEST_F(AssemblerTest, ld_r_n)
{
	InstructionAssembleTest("ld b, 12h", { 0x06, 0x12 });
	InstructionAssembleTest("ld c, 12h", { 0x0E, 0x12 });
	InstructionAssembleTest("ld d, 12h", { 0x16, 0x12 });
	InstructionAssembleTest("ld e, 12h", { 0x1E, 0x12 });
	InstructionAssembleTest("ld h, 12h", { 0x26, 0x12 });
	InstructionAssembleTest("ld l, 12h", { 0x2E, 0x12 });
	InstructionAssembleTest("ld (hl), 12h", { 0x36, 0x12 });
	InstructionAssembleTest("ld a, 12h", { 0x3E, 0x12 });
}
