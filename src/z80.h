#ifndef __Z80_H__
#define __Z80_H__

#include <bitset>
#include <cstdint>
#include <string>

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
		std::uint16_t pc;                              // Program Counter
		std::uint16_t sp;                              // Stack Pointer
		std::uint8_t general_purpose_registers[16] {}; //Registers
		std::bitset<8> flags;                          // Flags
		//Memory memory;                                 // System RAM
		bool running;                                  // Is system running?

		enum register_pos {a, b, c, d, e, f, h, l, a-prime, b-prime, c-prime, d-prime, e-prime, f-prime, h-prime, l-prime}; // Register location in register array
		enum flag_pos {sign=7, zero=6, hcarry=4, pv=2, n=1, c=0};                                                           // flag bit

		// general purpose registers
		std::uint8_t* a {};
		std::uint8_t* b {};
		std::uint8_t* c {};
		std::uint8_t* d {};
		std::uint8_t* e {};
		std::uint8_t* f {};
		std::uint8_t* h {};
		std::uint8_t* l {};
		std::uint16_t* af {};
		std::uint16_t* bc {};
		std::uint16_t* de {};
		std::uint16_t* hl {};

		// special purpose registers
		std::uint8_t i {};
		std::uint8_t r {};
		std::uint16_t ix {};
		std::uint16_t iy {};
		std::uint16_t sp {};
		std::uint16_t pc {};
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
