#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <cstdint>
#include <string_view>
#include <vector>
#include "memory.h"

typedef std::vector<uint8_t> bytes_t;

class Assembler
{
	private:
		uint16_t mem_ptr;
		Memory* memory;
	public:
		Assembler(Memory *memory);
		bytes_t encode_instruction(std::string_view instruction);
		void write_all(bytes_t bytes);
};

#endif
