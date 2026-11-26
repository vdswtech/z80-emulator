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

enum TABLE_NAMES
{
	main, // 0
	ed,   // 1
	cb    // 2
};

struct FLAGS
{
	bool sign           = false;
	bool zero           = false;
	bool halfcarry      = false;
	bool parityoverflow = false;
	bool addsubtract    = false;
	bool carry          = false;
	uint8_t result();
};

struct INSTRUCTION
{
	uint8_t opcode[3];
	uint8_t data[2];
	FLAGS flags;
	void set_sign();
	void set_zero();
	void set_halfcarry();
	void set_parityoverflow();
	void set_addsubtract();
	void set_carry();
	void unset_sign();
	void unset_zero();
	void unset_halfcarry();
	void unset_parityoverflow();
	void unset_addsubtract();
	void unset_carry();
	void set_flag(uint8_t* f);
	uint8_t execute();
};

class Z80
{
	private:
		INSTRUCTION main_instructions[16][16];
		INSTRUCTION ed_instructions[16][16];
		INSTRUCTION cb_instructions[16][16];
		std::uint8_t general_purpose_registers[16]; // register memory location
		Memory* memory;                             // System RAM
		bool running;                               // Is system running?

		// general purpose registers
		uint8_t* a {}; // Accumulator
		uint8_t* f {}; // Flags
		uint8_t* b {};
		uint8_t* c {};
		uint8_t* d {};
		uint8_t* e {};
		uint8_t* h {};
		uint8_t* l {};
		uint16_t* af {};
		uint16_t* bc {};
		uint16_t* de {};
		uint16_t* hl {};

		// special purpose registers
		uint8_t i { 0xFF };     // Interrupt page register
		uint8_t r { 0xFF };     // Memory refresh gegister
		uint8_t ir {0x0};       // Instruction register
		uint16_t ix { 0xFFFF }; // Index X register
		uint16_t iy { 0xFFFF }; // Index Y register
		uint16_t sp { 0xFFFF }; // Stack Pointer
		uint16_t pc {};         // Program Counter
		//INSTRUCTION* lookup_instruction(uint8_t opcode);
	public:
		Z80(Memory* memory);
		~Z80();
		void define_instruction(TABLE_NAMES table, uint8_t* opcode, uint8_t opcode_count, uint8_t* data, uint8_t data_count);
		void step();                                  // Step instruction
		void reset();                                 // Reset variables
		bool run_status();                            // Check if program is running when stepping through a program
		uint8_t get_reg_state(register_pos position); // Get state of general purpose register
		uint8_t get_ir();                             // Get current loaded instruction
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
