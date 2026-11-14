#include "assembler.h"

ASSEMBLER::ASSEMBLER(Memory *data)
{
	mem_ptr=0;
	this->data = data;
}

void ASSEMBLER::write_command_bytes(std::string command)
{
}
