# arrayCount.asm
  .data 
arrayA: .space 32        # 8 element integer array
count:  .word 999        # dummy value
str1: .asciiz "User Input X = "

  .text
main:
    # code to setup the variable mappings
    la $t0, arrayA
    la $t8, count

    # code for reading in the input values of the array
    addi $t8, $zero, 0    # Initialise the counter
readLoop:
    beq $t8, 32, read     #branch if equal to 32, 8 items
    li $v0, 5             #read int
    syscall 
    sw $v0, arrayA($t8)   #store input in array ERROR HERE
    addi  $t8, $t8, 4     #add by 4 to count
    b readLoop

    # code for reading in the user value X
read:  
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
