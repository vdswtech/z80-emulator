#include "memory.h"

Memory::Memory() { }
Memory::~Memory() { }

void Memory::write8(std::uint16_t address, std::uint8_t value)
{
	bytes[address] = value;
}
void Memory::write16(std::uint16_t address, std::uint16_t value)
{
	std::uint8_t low = (std::uint8_t) value;
	std::uint8_t high = (std::uint8_t) (value >> 8);
	bytes[address] = low;
	bytes[address + 1] = high;
}
std::uint8_t Memory::read8(std::uint16_t address)
{
	return bytes[address];
}
std::uint16_t Memory::read16(std::uint16_t address)
{
	std::uint8_t low = bytes[address];
	std::uint8_t high = bytes[address + 1];
	return static_cast<uint16_t>(high) << 8 | static_cast<uint16_t>(low);
}
