#include "z80.h"
#include "memory.h"

Memory mem;
Z80 z80(&mem);

TEST (REGISTERS, general_purpose)
{
	EXPECT_EQ(z80.get_gp_register_state(r_a), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_b), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_c), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_d), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_e), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_f), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_h), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_l), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_a_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_b_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_c_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_d_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_e_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_f_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_h_prime), 0);
	EXPECT_EQ(z80.get_gp_register_state(r_l_prime), 0);
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

TEST(PROGRAM, increment_register)
{
	/*
	 * Read a register
	 * Check it's value before and after incrementing
	 */
	mem.write8(0x0, 0x3C); // Increment 'a'
	mem.write8(0x1, 0x4);  // Increment 'b'
	mem.write8(0x2, 0xC);  // Increment 'c'
	mem.write8(0x3, 0x14); // Increment 'd'
	mem.write8(0x4, 0x1C); // Increment 'e'
	mem.write8(0x5, 0x24); // Increment 'h'
	mem.write8(0x6, 0x2C); // Increment 'l'
	mem.write8(0x7, 0x4C); // halt
	EXPECT_EQ(mem.read8(0x0), 0x3C);
	EXPECT_EQ(mem.read8(0x1), 0x4);
	EXPECT_EQ(mem.read8(0x2), 0xC);
	EXPECT_EQ(mem.read8(0x3), 0x14);
	EXPECT_EQ(mem.read8(0x4), 0x1C);
	EXPECT_EQ(mem.read8(0x5), 0x24);
	EXPECT_EQ(mem.read8(0x6), 0x2C);
	EXPECT_EQ(mem.read8(0x7), 0x4C);
	
	z80.reset(); // reset from previous test

	EXPECT_EQ(z80.get_gp_register_state(r_a), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_b), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_c), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_d), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_e), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_h), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_l), 0x0);

	z80.step();
	z80.step();
	z80.step();
	z80.step();
	z80.step();
	z80.step();
	z80.step();
	z80.step();

	EXPECT_EQ(z80.get_gp_register_state(r_a), 0x1);
	EXPECT_EQ(z80.get_gp_register_state(r_b), 0x1);
	EXPECT_EQ(z80.get_gp_register_state(r_c), 0x1);
	EXPECT_EQ(z80.get_gp_register_state(r_d), 0x1);
	EXPECT_EQ(z80.get_gp_register_state(r_e), 0x1);
	EXPECT_EQ(z80.get_gp_register_state(r_h), 0x1);
	EXPECT_EQ(z80.get_gp_register_state(r_l), 0x1);
}
