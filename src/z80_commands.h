#ifndef __Z80_NAMESPACE_H__
#define __Z80_NAMESPACE_H__

#include <cstdint>

// Register location in register array
enum register_pos {
	r_a,       // 0
	r_f,       // 1
	r_b,       // 2
	r_c,       // 3
	r_d,       // 4
	r_e,       // 5
	r_h,       // 6
	r_l,       // 7
	r_a_prime, // 8
	r_f_prime, // 9
	r_b_prime, // 10
	r_c_prime, // 11
	r_d_prime, // 12
	r_e_prime, // 13
	r_h_prime, // 14
	r_l_prime  // 15
};

void add(uint8_t *registers, register_pos modify, register_pos adding);
void add(uint8_t *registers, register_pos reg, uint8_t value);
void halt(bool *running);
void ld(uint8_t *registers, register_pos to, register_pos from);
void ld(uint8_t *registers, register_pos reg, uint8_t value);
void nop();

#endif
