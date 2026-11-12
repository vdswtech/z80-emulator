#include "assembler.h"

std::unordered_map<std::string, std::uint8_t> REGISTER_TO_ID = 
{
	{"B", 0},
	{"C", 1},
	{"D", 2},
	{"E", 3},
	{"H", 4},
	{"L", 5},
	{"(HL)", 6},
	{"A", 7}
};

Assembler::Assembler(Memory *memory)
{
	mem_ptr = 0;
	this->memory = memory;
}

std::vector<uint8_t> Assembler::encode_instruction(std::string instruction)
{

}

void Assembler::write_all(std::vector<uint8_t> bytes)
{

}
