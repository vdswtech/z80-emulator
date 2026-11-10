#include "memory.h"

Memory memory;

TEST(MemoryTest, initially0)
{
	EXPECT_EQ(memory.read8(0x0), 0);
}

TEST(MemoryTest, read8_write8)
{
	memory.write8(0x0, 0xAB);
	EXPECT_EQ(memory.read8(0x0), 0xAB);
}

TEST(MemoryTest, read16_write16)
{
	memory.write16(0x0, 0xABCD);
	EXPECT_EQ(memory.read16(0x0), 0xABCD);
}
