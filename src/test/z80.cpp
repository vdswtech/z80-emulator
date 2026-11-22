#include "z80.h"
#include "memory.h"

Memory mem;
Z80 z80(&mem);

TEST (REGISTERS, general_purpose)
{
	EXPECT_EQ(z80.get_gp_register_state(r_a), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_b), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_c), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_d), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_e), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_f), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_h), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_l), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_a_prime), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_b_prime), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_c_prime), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_d_prime), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_e_prime), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_f_prime), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_h_prime), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_l_prime), 0xFF);
	EXPECT_EQ(z80.get_af(), 0xFFFF);
	EXPECT_EQ(z80.get_bc(), 0xFFFF);
	EXPECT_EQ(z80.get_de(), 0xFFFF);
	EXPECT_EQ(z80.get_hl(), 0xFFFF);
}

TEST(REGISTERS, special_purpose)
{
	EXPECT_EQ(z80.get_pc(), 0);
	EXPECT_EQ(z80.get_sp(), 0xFFFF);
	EXPECT_EQ(z80.get_ix(), 0xFFFF);
	EXPECT_EQ(z80.get_iy(), 0xFFFF);
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

TEST(PROGRAM, increment_8bit_register)
{
	mem.write8(0x0, 0x3C); // Increment 'a'
	mem.write8(0x1, 0x4);  // Increment 'b'
	mem.write8(0x2, 0xC);  // Increment 'c'
	mem.write8(0x3, 0x14); // Increment 'd'
	mem.write8(0x4, 0x1C); // Increment 'e'
	mem.write8(0x5, 0x24); // Increment 'h'
	mem.write8(0x6, 0x2C); // Increment 'l'
	mem.write8(0x7, 0x4C); // halt

	z80.reset(); // reset from previous test

	EXPECT_EQ(z80.get_gp_register_state(r_a), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_b), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_c), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_d), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_e), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_h), 0xFF);
	EXPECT_EQ(z80.get_gp_register_state(r_l), 0xFF);

	for (uint8_t i=0; i<8; i++)
		z80.step();

	EXPECT_EQ(z80.get_gp_register_state(r_a), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_b), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_c), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_d), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_e), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_h), 0x0);
	EXPECT_EQ(z80.get_gp_register_state(r_l), 0x0);
}

TEST(PROGRAM, increment_16bit_register)
{
	mem.write8(0x0, 0x3);  // Increment 'BC'
	mem.write8(0x1, 0x13); // Increment 'DE'
	mem.write8(0x2, 0x23); // Increment 'HL'
	mem.write8(0x3, 0x33); // Increment 'SP'
	mem.write8(0x4, 0x4C); // halt

	z80.reset(); // reset from previous test

	EXPECT_EQ(z80.get_bc(), 0xFFFF); // Check register 'BC' is set to 0xFFFF
	EXPECT_EQ(z80.get_de(), 0xFFFF); // Check register 'DE' is set to 0xFFFF
	EXPECT_EQ(z80.get_hl(), 0xFFFF); // Check register 'HL' is set to 0xFFFF
	EXPECT_EQ(z80.get_sp(), 0xFFFF); // Check register 'SP' is set to 0xFFFF

	for (uint8_t i=0; i<5; i++)
		z80.step();

	EXPECT_EQ(z80.get_bc(), 0x0); // Check register 'BC' is set to 0x0
	EXPECT_EQ(z80.get_de(), 0x0); // Check register 'DE' is set to 0x0
	EXPECT_EQ(z80.get_hl(), 0x0); // Check register 'HL' is set to 0x0
	EXPECT_EQ(z80.get_sp(), 0x0); // Check register 'SP' is set to 0x0
}

TEST(PROGRAM, single_register_increment_flag_checks)
{
	mem.write8(0x1, 0x4);  // Increment 'b'
	mem.write8(0x2, 0x4C); // halt
	z80.reset(); // reset from previous test

	for (uint8_t i=0; i<2; i++)
		z80.step();

	EXPECT_EQ(z80.get_gp_register_state(r_b), 0x0); // Check register b is set to zero
	EXPECT_EQ(z80.get_gp_register_state(r_f), 0x54);
}

TEST(PROGRAM, multiple_register_increment_flag_checks)
{
	mem.write8(0x1, 0x4);  // Increment 'b'
	mem.write8(0x2, 0x14);  // Increment 'd'
	mem.write8(0x3, 0x4C); // halt
	z80.reset(); // reset from previous test

	for (uint8_t i=0; i<2; i++)
		z80.step();

	EXPECT_EQ(z80.get_gp_register_state(r_b), 0x0); // Check register b is set to zero
	EXPECT_EQ(z80.get_gp_register_state(r_f), 0x54);
}

TEST(PROGRAM, load_data)
{
	mem.write8(0x1, 0x3E); // Load the next memory location into register A;
	mem.write8(0x2, 0x2A); // Let's load the meaning of life.
	mem.write8(0x3, 0x4C); // halt
	z80.reset(); // reset from previous test

	for (uint8_t i=0; i<4; i++)
		z80.step();

	EXPECT_EQ(z80.get_gp_register_state(r_a), 0x2A); // Check register a is set to 42
}

TEST(PROGRAM, load_multiple_data)
{
	mem.write8(0x1, 0x3E); // Load the next memory location into register A;
	mem.write8(0x2, 0x2A); // Let's load the meaning of life.
	mem.write8(0x3, 0x4C); // halt
	z80.reset(); // reset from previous test

	for (uint8_t i=0; i<4; i++)
		z80.step();

	EXPECT_EQ(z80.get_gp_register_state(r_a), 0x2A); // Check register a is set to 42
}
