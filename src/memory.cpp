#include "memory.h"

Memory::Memory() { }
Memory::~Memory() { }

void Memory::write8(uint16_t address, uint8_t value)
{
	bytes[address] = value;
}
void Memory::write16(uint16_t address, uint16_t value)
{
	uint8_t low = (uint8_t) value;
	uint8_t high = (uint8_t) (value >> 8);
	bytes[address] = low;
	bytes[address + 1] = high;
}
uint8_t Memory::read8(uint16_t address)
{
	return bytes[address];
}
uint16_t Memory::read16(uint16_t address)
{
	uint8_t low = bytes[address];
	uint8_t high = bytes[address + 1];
	return static_cast<uint16_t>(high) << 8 | static_cast<uint16_t>(low);
}
