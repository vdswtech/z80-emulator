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
		uint8_t encode_number_offset8(std::string_view number_string);
		void write_all(bytes_t bytes);
};

#endif
