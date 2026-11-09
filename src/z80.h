#ifndef __Z80_H__
#define __Z80_H__
#include <cstdint>

class Z80
{
	private:
		// general purpose registers
		std::uint8_t general_purpose_registers[16] {};
	public:
		std::uint8_t* a {};
		std::uint8_t* b {};
		std::uint8_t* c {};
		std::uint8_t* d {};
		std::uint8_t* e {};
		std::uint8_t* f {};
		std::uint8_t* h {};
		std::uint8_t* l {};
		std::uint16_t* af {};
		std::uint16_t* bc {};
		std::uint16_t* de {};
		std::uint16_t* hl {};

		// special purpose registers
		std::uint8_t i {};
		std::uint8_t r {};
		std::uint16_t ix {};
		std::uint16_t iy {};
		std::uint16_t sp {};
		std::uint16_t pc {};
		Z80();
		~Z80();
};

#endif
