#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <stdint>
#include <string>
#include "memory.h"

enum REG_PAIR {"BC", "DE", "HL", "AF"};
enum REGISTER {'B', 'C', 'D', 'E', 'H', 'L', 'A'};

class ASSEMBLER
{
	private:
		uint16_t mem_ptr;
		Memory *data;
		void halt();
		void push_qq(REG_PAIR reg_pairi);
		void push_ix();
		void push_iy();
		void pop_qq(REG_PAIR reg_pair);
		void pop_ix();
		void pop_iy();
		void ex_af_afi();
		void exx();
		void exsphl();
		void exspix();
		void exspiy();
	public:
		ASSEMBLER(Memory *data);
		void write_command_bytes(std::string command);
};

#endif
