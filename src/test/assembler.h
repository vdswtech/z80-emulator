#ifndef __TEST__ASSEMBLER_H__
#define __TEST__ASSEMBLER_H__

#include <gtest/gtest.h>
#include "../assembler.h"

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

#endif
