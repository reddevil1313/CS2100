//Question 1
#include <stdio.h>

typedef unsigned char byte_t;

void printByte(byte_t);

int main(void)
{
    byte_t a, b;

    a = 5;
    b = 22;
    printf("a  = ");
    printByte(a);
    printf("\n");
    printf("b = ");
    printByte(b);
    printf("\n");
    printf("a|b = ");
    printByte(b >> 2);
    printf("\n");
    return 0;
}

void printByte(byte_t x)
{
    printf("%c%c%c%c%c%c%c%c",
           (x & 0x80 ? '1' : '0'),
           (x & 0x40 ? '1' : '0'),
           (x & 0x20 ? '1' : '0'),
           (x & 0x10 ? '1' : '0'),
           (x & 0x08 ? '1' : '0'),
           (x & 0x04 ? '1' : '0'),
           (x & 0x02 ? '1' : '0'),
           (x & 0x01 ? '1' : '0'));
}

/*
Answer1:
The OR operation will be done on individual bits of each operand bit.
Similarly for all the other bitwise operators.
For a = 5, b = 22, a = 00000101, b = 00010110
| bitwise OR -> 00010111
& bitwise AND -> 00000100
^ bitwise XOR -> 00010011
~ (1's complement) -> 11111010
<<2 (left shift) -> 01011000
>>2 (right shift) -> 00000101

Note: 
Left Shift -> multiplying the number with the shift
Right Shift -> dividing the number with the shift
*/

//Question 2
/*
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
*/

//Alternate Solution:
void swap(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}
// We use the star operator as we need to swap at the address.
// If i don't pass swap(&x, &y) [pass by reference]; I will be doing pass-by-value through swap(x, y)

//Question 3
/*
 a -> $s0
 b -> $s1
 c -> $s2
Operations:
1. We can use the bitwise OR operation. We also need to load the upper immediate as the immediate operand can only be 16 bits.
    a. Use "load upper immediate" lui to set the upper 16-bit (lui $t0 1) -> Thise sets the bit 16 to 1
    b. (ori $t0, $t0, 0b01000001100000100) -> set bits 14,9,8,2
    These two steps are to get the mask into $t0
    c. or $s1, $s1, $t0

2. (Thought: Can we do the bitwise NOR operation?)
    Recall -> x AND 0 = 0, x AND 1 = x, x OR 0 = x, x OR 1 = 1
    We assume that the most significant 24 bits are zeroes.

    a. Extract bits 1,3,7 of b into $t0. -> andi $t0, $s1, 0....10001010
    b. Clear bits 1, 3, 7 of a. -> andi $s0, $s0, 0b1111...111111011110101
    c. or $s0, $s0, $t0

    Full solution:
    andi $t0, $s1, 0b000....10001010
    lui $t1, 0b1111....1111
    ori $t1, $t1, 0b111.....101110101
    and $s0, $s0, $t1
    or $s0, $s0, $t0

3. Recall -> x XOR 0 = x, x XOR 1 = x'. Do left shift by 2 as well (1, 3, 7 -> 2, 4, 8)
    a. xori $t0, $s1, 0000....10001010 -> To replace the 1,3,7 bits of b
    b. andi $t0, $t0, 0000....10001010 -> To extract the 1,3,7 bits of b
    c. sli $t0 $t0 1 -> To shift the bits to 2,4,8
    d. lui $t1, 0b1111....111-> Load the upper immediate with all 1's
    e. ori $t1, $t1, 0b111...011101011 -> To set the lower bits 0 at 2,4 and 8
    f. and $s2, $s2, $t1 -> Extract the 2,4,8 bits of c
    g. or $s2, $s2, $t0 -> Input the reversed shifted bits of b


//Question 4.
a) c = a + b
    add $s2, $s0, $s1
b) d = a + b - c
    add $t0, $s0, $s1; sub $s3, $t0, $s2
c) c = 2b + (a - 2)
    sll $s1, $s1, 1
    subi $s0, $s0, 2
    add $s2, $s1, $s0
d) d = 6a + 3(b - 2c) -> d = 3(2a + b - 2c) -> d = 4(2(a-c) + b) - (2(a-c) + b)
    sub $t0, $s0, $s2
    sll $t0, $t0, 1
    add $t1, $t0, $s1
    sll $t2, $t1, 2
    sub $s3, $t2, $t1



//Question 5. [INCOMPLETE]
$s0 is a 31-bit binary sequence with MSB=0
What is the algorithm doing? -> It is flipping all the LSBs to the MSBs

MIPS CODE:
        (add $t0, $s0, $zero) -> Copy the initial value
        ($t1, 0x8000) -> sets the MSB of $t1 of the 32-bit. (0b1000...000)
    lp:
        (beq $t0, $zero, e) -> if condition is true, branch to the different operation.
        (andi $t2, $t0, 1) -> extracting the LSB of $t0 into $t2
        (beq $t2, $zero, s) -> If extracted LSB is equal to 0, shift to branch s
        (xor $s0, $s0, $t1) -> $t1 contains the MSB of my 32-bit. Thus, we are flipping the MSB using the mask $t1.

    s:
        (srl $t0, $t0, 1) -> Right shift $s0 by 1 bit.
        j lp -> jumps to the branch lp

    e:
        Base Case to avoid unnecessary operations on $zero

QUESTION PARTS:
    a) 
        1. Hexadecimal 31
            How many iterations? 
                The number of bits i.e 31.
            What happens in each iteration?
                Toggle the MSB of $s0 if $t2 = 1 where $t2 contains the LSB

            Binary Notation: 00000.000110001
            After the MIPS Code: 10001100000000000...00000
            In Hexadecimal: 8C000000

        2. Hexadecimal value 0x0AAAAAAA
            Binary Notation: 0b1010101010101010101010101010
            After the MIPS Code: 111111.....111111
            In hexadecimal: FFFFFFFF

    b) It reverses the bits by copying the LSB to the MSB


*/