| Bit | Symbol | Name            |
| :-: | :----: | :-------------: |
|  8  |   S    | Sign            |
|  7  |   Z    | Zero            |
|  6  |   F5   | UNDOCUMENTED    |
|  5  |   H    | Half Carry      |
|  4  |   F3   | UNDOCUMENTED    |
|  3  |   P/V  | Parity/Overflow |
|  2  |   N    | Add/Subtract    |
|  1  |   C    | Carry           |

Flag Meanings

* Sign: Set if value is negative, unset if positive
* Zero: Set if value is zero, unset otherwise
* F5: copy of bit 5
* Half carry: true if there is a carry from bit 3 to bit 4
* F3: copy of bit 3
* Parity/Overflow: Parity set if even number of bits is set; Overflow set if the 2-complement result does not fit in the register
* Add/Subtract: Set if the last operation was subtraction, unset otherwise
* Carry: Set if result does not fit in the register
