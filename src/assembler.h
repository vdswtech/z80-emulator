#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <stdint>
#include <string>
#include "memory.h"

class Assembler
{
	private:
		uint16_t mem_ptr;
		Memory* memory;
	public:
		Assembler(Memory *memory);
		std::vector<uint8_t> encode_instruction(std::string instruction);
		void write_all(std::vector<uint8_t> bytes);
};

#endif
