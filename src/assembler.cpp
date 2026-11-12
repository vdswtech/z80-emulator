#include "assembler.h"

ASSEMBLER(Memory *data)
{
	mem_ptr=0;
	this->data = data;
}

void write_command_bytes(std::string command)
{
}
