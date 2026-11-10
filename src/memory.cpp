#include "memory.h"

void write8(std::uint16_t address, std::uint8_t value)
{
	bytes[address] = value;
}
void write16(std::uint16_t address, std::uint16_t value)
{
	std::uint8_t low = (std::uint8_t) value;
	std::uint8_t high = (std::uint8_t) (value >> 8);
	bytes[address] = low;
	bytes[address + 1] = high;
}
std::uint8_t read8(std::uint16_t address)
{
	return bytes[address];
}
std::uint16_t read16(std::uint16_t address)
{
	std::uint8_t low = bytes[address];
	std::uint8_t high = bytes[address + 1];
	return std::static_cast<uint16_t>(high) << 8 & std::static_cast<uint16_t>(low);
}
