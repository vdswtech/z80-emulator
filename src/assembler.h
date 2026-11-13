#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <cstdint>
#include <string>
#include <vector>
#include "memory.h"

class Assembler
{
	private:
		uint16_t mem_ptr;
		Memory* memory;
	public:
		Assembler(Memory *memory);
		std::vector<std::uint8_t> encode_instruction(std::string instruction);
		void write_all(std::vector<std::uint8_t> bytes);
};

#endif
