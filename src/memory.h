#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <cstdint>

#define MAX_MEMORY_SIZE 65536

class Memory
{
	private:
		std::uint8_t bytes[MAX_MEMORY_SIZE] {};
	public:
		Memory();
		~Memory();
		void write8(std::uint16_t address, std::uint8_t value);
		void write16(std::uint16_t address, std::uint16_t value);
		std::uint8_t read8(std::uint16_t address);
		std::uint16_t read16(std::uint16_t address);
}
#endif 
