#include <iostream>
#include <iomanip>
#include "z80.h"
using namespace std;

int main(int argc, char** argv)
{
	Z80 z80;
	cout << "value of a: 0x" << std::hex << setfill('0') << setw(2) << *z80.a << "\n";
	cout << "value of b: 0x" << std::hex << setfill('0') << setw(2) << *z80.b << "\n";
	cout << "value of c: 0x" << std::hex << setfill('0') << setw(2) << *z80.c << "\n";
	cout << "value of d: 0x" << std::hex << setfill('0') << setw(2) << *z80.d << "\n";
	cout << "value of e: 0x" << std::hex << setfill('0') << setw(2) << *z80.e << "\n";
	cout << "value of f: 0x" << std::hex << setfill('0') << setw(2) << *z80.f << "\n";
	cout << "value of h: 0x" << std::hex << setfill('0') << setw(2) << *z80.h << "\n";
	cout << "value of l: 0x" << std::hex << setfill('0') << setw(2) << *z80.l << "\n";
	cout << "value of af: 0x" << std::hex << setfill('0') << setw(4) << *z80.af << "\n";
	cout << "value of bc: 0x" << std::hex << setfill('0') << setw(4) << *z80.bc << "\n";
	cout << "value of de: 0x" << std::hex << setfill('0') << setw(4) << *z80.de << "\n";
	cout << "value of hl: 0x" << std::hex << setfill('0') << setw(4) << *z80.hl << "\n";
	return 0;
}
