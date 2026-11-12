#include "z80.h"
#include "memory.h"

Memory mem;
Z80 z80(&mem);

TEST (REGISTERS, general_purpose)
{
	EXPECT_EQ(z80.get_gp_register_state(a), 0);
	EXPECT_EQ(z80.get_gp_register_state(b), 0);
	EXPECT_EQ(z80.get_gp_register_state(c), 0);
	EXPECT_EQ(z80.get_gp_register_state(d), 0);
	EXPECT_EQ(z80.get_gp_register_state(e), 0);
	EXPECT_EQ(z80.get_gp_register_state(f), 0);
	EXPECT_EQ(z80.get_gp_register_state(h), 0);
	EXPECT_EQ(z80.get_gp_register_state(l), 0);
	EXPECT_EQ(z80.get_gp_register_state(a_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(b_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(c_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(d_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(e_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(f_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(h_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(l_prime), 0);
	EXPECT_EQ(z80.get_af(), 0);
	EXPECT_EQ(z80.get_bc(), 0);
	EXPECT_EQ(z80.get_de(), 0);
	EXPECT_EQ(z80.get_hl(), 0);
}

TEST(REGISTERS, special_purpose)
{
	EXPECT_EQ(z80.get_pc(), 0);
	EXPECT_EQ(z80.get_sp(), 65535);
	EXPECT_EQ(z80.get_ix(), 0);
	EXPECT_EQ(z80.get_iy(), 0);
}

TEST(PROGRAM, nop_and_halt)
{
	/*
	 * Write program to memory
	 * First write a no-op
	 * Then write an halt
	 */
	mem.write8(0, 0);
	mem.write8(0x1, 0x4C);
	EXPECT_EQ(mem.read8(0x0), 0);
	EXPECT_EQ(mem.read8(0x1), 0x4C);
	EXPECT_EQ(z80.get_pc(), 0);
	EXPECT_FALSE(z80.run_status());
	z80.step();
	EXPECT_TRUE(z80.run_status());
	EXPECT_EQ(z80.get_ir(), 0);
	EXPECT_EQ(z80.get_pc(), 1);
	z80.step();
	EXPECT_FALSE(z80.run_status());
	EXPECT_EQ(z80.get_ir(), 0x4C);
	EXPECT_EQ(z80.get_pc(), 2);
}
