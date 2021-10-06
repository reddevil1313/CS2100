//Q1.

char string[size] = {...}; // some string
int low, high, matched;

// Translate to MIPS from this point onwards
low = 0;
high = size - 1;
matched = 1; // assume this is a palindrome
             // In C, 1 means true and 0 means false
while ((low < high) && matched)
{
    if (string[low] != string[high])
        matched = 0; // found a mismatch else
    else
    {
        low++;
        high--;
    }
}
// "matched" = 1 (palindrome) or 0 (not palindrome)
/*
Given Mapping:
    low -> $s0;
    high -> $s1;
    matched -> $s3;
    base address and string[] -> $s4;
    size -> $s5
*/
/*
Answer:
a.
        addi $s0, $zero, 0      # lo = 0
        addi $s1, $s5, -1       # hi = size - 1
        addi $s3, $zero, 1      # matched = 1
loop:   slt $t0, $s0, $s1       # (lo < hi) ?
        beq $t0, $zero, exit    # exit if (lo >= hi) 
    (Use slt, beq to implement blt. (blt is a pseudo-instruction; do not sue pseudo-instructions))

        beq $s3, $zero, exit    # exit if (match === 0)
        
        add $t1, $s4, $s0       # addr of str[lo], base address + the index of the element
        lb $t2, 0($t1)          # $t2 = str[lo], We have zero here because 
                                  we don't need to do any displacement as we already calculated the address of t1.
    (All elements should be brought to the registers to do operations, we can't do it on the program. Thus, we use use loadbyte)

        add $t3, $s4, $s1       # addr of str[hi]
        lb $t4, 0($t3)          # $t4 = str[hi]
        beq $t2, $t4, else      # compare str[lo], str[hi]
        addi $s3, $zero, 0      # matched = 0
        j  endw                 # can also be "j loop"
else:   addi $s0, $s0, 1        # lo++
        addi $s1, $s1, -1       # hi--
endw:   j loop
exit:
    
b.
we first find the address then do the left shifts in the loop and and for increment we use the address for addi as well.
*/

//Q2. a.
/*
Instruction MIPS -> Code Encoding
                    # $s1 stores the result, $t0 stores a non-negative number
                      addi $s1, $zero, 0  #Inst. address is 0x00400028
0x00084042          loop: srl $t0, $t0, 1
0x11000002          
0x22310001
                        j loop 
                    exit:

Answer:
addi $s1, $zero, 0
    I format: opcode, rs, rt, immediate. opcode is 8Hex
    rt is $s1, rs is $s0. $s1 = $17 (through the datasheet)
    32-bit -> 001000 00000 10001 000....000 (16bit)
    Hexadecimal -> 20110000

0x11000002
    First convert to binary -> 0001 0001 0000 0000 ..... 0010
    opcode (first 6 bits) -> Value is 4. Refers to inspection branch on equal.
    Format -> I format. Next 2 5-bits are registers. Last 16 bits is immediate.
    First register -> 8 -> $t0
    Second register -> 0 -> $zero
    Immediate -> 2. It represents the jumps in addressing. so we jump 2 instructions as the PC will be pointing at
        the next instruction when evaluating -> Thus, this acts as exit.
    MIPS code: beq $t0 $zero exit

0x22310001
    First convert to binary -> 0010 0010 0011 0001 0000 0000 0000 0001
    opcode -> 8 -> Refers to addi inspection
    Format -> I format. Next 2 5-bits are registers. Last 16 bits is immediate.
    First register -> 17 -> $s1 (rs)
    Second register -> 17 -> $s1 (rt)
    Immediate -> 1

j loop
    opcode -> 2Hex -> 000010
    Format -> J format inspection, 6 bits for opcode, remaining 26 for the address
    How do we find the address of the srl instruction?
        0x00400028 + 4 = 0x0040002C -> 0000 0000 0100 0000 0000 0000 0010 1100
        Conver to binary, remove first 4 bits and then last 2 bits. 
            This is because the address is word adress so the last 2 bits don't really matter. Always going to be zero.
            We remove the first 4 bits as we the PC has the remaining first 4 bits.
    000010 | 0000 0100 0000 0000 0000 0010 11
    Hexadecimal -> 0810000B
*/

//b. Mathematical Expression:  $s1 = [log2($t0)] (floor operation)

//Q3. a.
/*
Variable Mappings:
    address of array[] -> $s0
    target -> $s1 // value to look for in array
    low -> $s2 // lower bound of the subarray
    high -> $s3 // upper bound of the subarray
    mid -> $s4 // middle index of the subarray
    ans -> $s5 // index of the target if found, -1 otherwise. Initialized to -1.

loop:
    slt $t9, $s3, $s2
    bne $t9, $zero, end

    add $s4, $s2, $s3
    [srl $s4, $s4, 1]

    sll $t0, $s4, 2
    add $t0, $s0, $t0 
    [lw $t1, 0($t0)]

    slt $t9, $s1, $t1
    beq $t9, $zero, bigger

    addi $s3, $s4, -1
    j loopEnd

bigger:
    [slt $t9, $s1, $t1] 
    [beq $t9, $zero, equal]

    addi $s2, $s4, 1
   j loopEnd

equal:
    add $s5, $s4, $zero 
    [j end]

loopEnd:
    [j loop]

end:

b) 
    end; is 16 inspections away from bne's next instruction (add $s4, $2, $s3),
    so the immediate value is 16.

c) 
    26 bits to store lpEnd
    base address + 17 times 4 bytes.
    Address at lpEnd is 0xFFFFFF00 + (17x4) = 0xFFFFFF44.
    Removing the first 4 bits and the last 2 bits, we put this into the immediate field.
    Opcode of j is 000010. Hence, 000010 1111 1111 1111 1111 1111 0100 01
    = 0x0BFFFFD1

d) Same encoding.
*/