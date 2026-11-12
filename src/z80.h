#ifndef __Z80_H__
#define __Z80_H__

#include <cstdint>
#include <string>

#include "memory.h"

enum register_pos {a, b, c, d, e, f, h, l, a_prime, b_prime, c_prime, d_prime, e_prime, f_prime, h_prime, l_prime}; // Register location in register array
enum flag_pos {sign=7, zero=6, hcarry=4, pv=2, n=1, carry=0};                                                       // flag bit

class Z80
{
	private:
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

		// CPU state
		std::uint8_t general_purpose_registers[16] {}; //Registers
		Memory memory;                                 // System RAM
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
		std::uint16_t ix {}; // Index X register
		std::uint16_t iy {}; // Index Y register
		std::uint16_t sp {}; // Stack Pointer
		std::uint16_t pc {}; // Program Counter
	public:
		Z80();
		~Z80();
		void read_instruction();                              // Read instruction from memory
		void execute();                                       // Execute instruction
		void step();                                          // Step instruction
		void reset();                                         // Reset variables
		uint8_t get_gp_register_state(register_pos position); // Get state of general purpose register
};

#endif
