#ifndef __Z80_H__
#define __Z80_H__

#include <cstdint>
#include <string>

#include "memory.h"

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

struct FLAGS
{
	/*
	 * CPU FLAGS
	 * +-----+--------+-----------------+
	 * | Bit | Symbol | Name            |
	 * +-----+--------+-----------------+
	 * |  7  |   S    | Sign            |
	 * +-----+--------+-----------------+
	 * |  6  |   z    | Zero            |
	 * +-----+--------+-----------------+
	 * |  5  |   F5   | UNDOCUMENTED    |
	 * +-----+--------+-----------------+
	 * |  4  |   H    | Half Carry      |
	 * +-----+--------+-----------------+
	 * |  3  |   F3   | UNDOCUMENTED    |
	 * +-----+--------+-----------------+
	 * |  2  |   P/V  | Parity/Overflow |
	 * +-----+--------+-----------------+
	 * |  1  |   N    | Add/Subtract    |
	 * +-----+--------+-----------------+
	 * |  0  |   C    | Carry           |
	 * +-----+--------+-----------------+
	 */

	uint8_t sign      = 0x80;
	uint8_t zero      = 0x40;
	uint8_t halfcarry = 0x10;
	uint8_t pv        = 0x4;
	uint8_t addsub    = 0x2;
	uint8_t carry     = 0x1;
};

class Z80
{
	private:
		FLAGS flag_bits; // Hex values for switching bit in f register

		// CPU state
		std::uint8_t general_purpose_registers[16] {}; //Registers
		Memory* memory;                                // System RAM
		bool running;                                  // Is system running?

		// general purpose registers
		std::uint8_t* a {}; // Accumulator
		std::uint8_t* b {};
		std::uint8_t* c {};
		std::uint8_t* d {};
		std::uint8_t* e {};
		std::uint8_t* f {}; // Flags
		std::uint8_t* h {};
		std::uint8_t* l {};
		std::uint16_t* af {};
		std::uint16_t* bc {};
		std::uint16_t* de {};
		std::uint16_t* hl {};

		// special purpose registers
		std::uint8_t i {};   // Interrupt page register
		std::uint8_t r {};   // Memory refresh gegister
		std::uint8_t ir {};  // Instruction register
		std::uint16_t ix {}; // Index X register
		std::uint16_t iy {}; // Index Y register
		std::uint16_t sp {}; // Stack Pointer
		std::uint16_t pc {}; // Program Counter

		void execute(); // Execute instruction
	public:
		Z80(Memory* memory);
		~Z80();
		void step();                                          // Step instruction
		void reset();                                         // Reset variables
		bool run_status();                                    // Check if program is running when stepping through a program
		uint8_t get_gp_register_state(register_pos position); // Get state of general purpose register
		uint8_t get_ir();                                     // Get current loaded instruction
		uint16_t get_pc();
		uint16_t get_sp();
		uint16_t get_ix();
		uint16_t get_iy();
		uint16_t get_af();
		uint16_t get_bc();
		uint16_t get_de();
		uint16_t get_hl();
};

#endif
