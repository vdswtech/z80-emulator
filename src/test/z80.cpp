#include "z80.h"

Z80 z80;

/*
 * Test registers
 */

TEST (REG, default_reg_values)
{
	EXPECT_EQ(*z80.a, 0);
	EXPECT_EQ(*z80.b, 0);
	EXPECT_EQ(*z80.c, 0);
	EXPECT_EQ(*z80.d, 0);
	EXPECT_EQ(*z80.e, 0);
	EXPECT_EQ(*z80.f, 0);
	EXPECT_EQ(*z80.h, 0);
	EXPECT_EQ(*z80.l, 0);
	EXPECT_EQ(*z80.af, 0);
	EXPECT_EQ(*z80.bc, 0);
	EXPECT_EQ(*z80.de, 0);
	EXPECT_EQ(*z80.hl, 0);
	EXPECT_EQ(z80.i, 0);
	EXPECT_EQ(z80.r, 0);
	EXPECT_EQ(z80.ix, 0);
	EXPECT_EQ(z80.iy, 0);
	EXPECT_EQ(z80.sp, 0);
	EXPECT_EQ(z80.pc, 0);
}
