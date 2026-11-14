#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <cstdint>

#define MAX_MEMORY_SIZE 65536

class Memory
{
	private:
		uint8_t bytes[MAX_MEMORY_SIZE] {};
	public:
		Memory();
		~Memory();
		void write8(uint16_t address, uint8_t value);
		void write16(uint16_t address, uint16_t value);
		uint8_t read8(uint16_t address);
		uint16_t read16(uint16_t address);
};
#endif
