#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <cstdint>
#include <string>
#include "memory.h"


class ASSEMBLER
{
	private:
		uint16_t mem_ptr;
		Memory* data;
	public:
		ASSEMBLER(Memory* data);
		void write_command_bytes(std::string command);
};

#endif
