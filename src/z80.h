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
	uint8_t sign      = 0b10000000;
	uint8_t zero      = 0b01000000;
	uint8_t halfcarry = 0b00010000;
	uint8_t pv        = 0b00000100;
	uint8_t addsub    = 0b00000010;
	uint8_t carry     = 0b00000001;
};

class Z80
{
	private:
		FLAGS flag_bits;        // Hex values for switching bit in f register

		// CPU state
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

		// execute functions
		void execute();   // Execute instruction
		void add();       // Execute any add calls
		void increment(); // Execute any increment calls
		void ld();		  // Execute ld calls

		//  Execute helper scripts
		bool f_c_is_set();                                   // Check if Carry flag is set
		bool f_n_is_set();                                   // Check if Add/Subtract flag is set
		bool f_pv_is_set();                                  // Check if Parity/Overflow flag is set
		bool f_h_is_set();                                   // Check if Half Carry flag is set
		bool f_z_is_set();                                   // Check if Zero flag is set
		bool f_s_is_set();                                   // Check if Sign flag is set
		void inc8(uint8_t* reg);                             // Increment a register
		void inc16(uint16_t* reg);                           // Increment a register
		inline void update_f8(uint8_t *reg8, bool rollover); // Update Flags register
	public:
		Z80(Memory* memory);
		~Z80();
		void step();                                          // Step instruction
		void reset();                                         // Reset variables
		void reset_register_pointers();                       // Reset general purpose register pointers to default
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
		uint8_t* get_hl_indirect();
		uint8_t* get_reg8_ptr(uint8_t id);
};

#endif
