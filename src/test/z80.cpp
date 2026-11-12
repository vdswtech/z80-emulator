#include "z80.h"

Z80 z80;

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
