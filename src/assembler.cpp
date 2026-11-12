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
	std::vector<uint8_t> bytes = {};
	int end_of_mnemonic = instruction.find(" ");
	std::string mnemonic = instruction.substr(end_of_mnemonic);
	if (mnemonic == "NOP")
	{
		bytes.push_back(0x0);
	}
	else if (mnemonic == "HALT")
	{
		bytes.push_back(0x76);
	}
	else if (mnemonic == "INC")
	{

	}
	return bytes;
}

void Assembler::write_all(std::vector<uint8_t> bytes)
{
	for (auto byte : bytes)
	{
		memory.write8(mem_ptr, byte);
		mem_ptr++;
	}
}
