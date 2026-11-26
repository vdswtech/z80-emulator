#include <iostream>
#include <iomanip>

#include "assembler.h"
#include "memory.h"
#include "z80.h"
using namespace std;

int main(int argc, char** argv)
{
	Memory memory;
	Z80 z80(&memory);
	return 0;
}
