#include "z80_commands.h"

void add(uint8_t *registers, register_pos modify, register_pos adding)
{
	registers[0+modify] += registers[0+adding];
}

void add(uint8_t *registers, register_pos reg, uint8_t value)
{
	registers[0+reg] += value;
}

void halt(bool *running)
{
	(*running) = false;
}

void ld(uint8_t *registers, register_pos to, register_pos from)
{
	registers[0+to] = registers[0+from];
}

void ld(uint8_t *registers, register_pos reg, uint8_t value)
{
	registers[0+reg] = value;
}

void nop()
{
	return;
}
