# arrayCount.asm
  .data 
arrayA: .word 1, 2, 3, 4, 5, 6, 7, 8  # arrayA has 8 values
count:  .word 999         # dummy value
str1: .asciiz "User Input X = "

  .text
main:
    # code to setup the variable mappings
    la $t0, arrayA
    la $t8, count

    # code for reading in the user value X
    li   $v0, 4           # system call code for print_string
    la   $a0, str1        # address of string to print
    syscall               # print the string

    li   $v0, 5           # system call code for read_int
    syscall               # store the integer

    move $t6, $v0         # move the integer input to another register

    # code for counting multiples of X in arrayA
    addi $t8, $zero, 0    # Initialise the counter
    addi $t1, $t0, 0      # address of current item
    addi $t2, $t0, 32     # address of last item
    addi $t5, $t6, -1     # making the mask

loop: 
    bge $t1, $t2, end     # comparing address!
    lw $t3, 0($t1)        # loading the value of the element
    and $t7, $t3, $t5     # Applyling the mask to find the remainder
    bne $t7, $zero, skip  # Check if it's equal to zero
    addi $t8, $t8, 1      # Count++

skip: 
    addi $t1, $t1, 4
    j loop

end:

    # code for printing result
    li   $v0, 1           # system call code for print_int
    move $a0, $t8         # integer to print
    syscall               # print the integer

    # code for terminating program
    li  $v0, 10
    syscall
