#include "memory.h"


TEST(MemoryTest, initially0)
{
	Memory memory;
	EXPECT_EQ(memory.read8(0x0), 0);
}

TEST(MemoryTest, read8_write8)
{
	Memory memory;
	memory.write8(0x0, 0xAB);
	EXPECT_EQ(memory.read8(0x0), 0xAB);
}

TEST(MemoryTest, read16_write16)
{
	Memory memory;
	memory.write16(0x0, 0xABCD);
	EXPECT_EQ(memory.read16(0x0), 0xABCD);
}

TEST(MemoryTest, write16_read8)
{
	Memory memory;
	memory.write16(0x0, 0xABCD);
	EXPECT_EQ(memory.read8(0x0), 0xCD);
	EXPECT_EQ(memory.read8(0x1), 0xAB);
}

TEST(MemoryTest, write8_read16)
{
	Memory memory;
	memory.write8(0x0, 0xCD);
	memory.write8(0x1, 0xAB);
	EXPECT_EQ(memory.read16(0x0), 0xABCD);
}
