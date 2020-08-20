## Lab 1 Preliminary Work
## Program 1
## Enter array elements up to size 20 then print

	.text
	.globl __start
	
__start:
	# Print prompt for array size
	la $a0, pmt1
	li $v0, 4
	syscall
	
	# Get array size
	li $v0, 5
	syscall
	sw $v0, size
	
	# Invalid size handling
	
	li $a0, -1
	
	blt $v0, 1, array_size_error
	bge $v0, 20, array_size_error
	
	li $a0, 0
	
	# Get array elements	
	lw $t0, size
	li $t1, 0 # index
	li $t2, 0 # address index
	
array_size_error:
	
	bne $a0, -1, write_loop
	
	la $a0, pmt0
	li $v0, 4
	syscall
	
	j __start
	
write_loop:
	# Loop exit condition
	bge $t1, $t0, print_unreversed
	
	# Print prompt for next element
	la $a0, pmt2
	li $v0, 4
	syscall
	
	# Get next element
	li $v0, 5
	syscall
	sw $v0, arr($t2)
	
	# Loop iteration handling
	addi $t1, $t1, 1
	addi $t2, $t2, 4
	j write_loop
	
# Print array first
print_unreversed:
	j print
	
prepare_to_reverse:
	li $a1, -1

	# Prepare for reversing the array
	move $t3, $t0
	subi $t3, $t3, 1
	li $t4, 4 # Multiplication constant
	
	# $t0 = array size
	li $t1, 0 # array index from left
	mul $t2, $t3, $t4 # array index from right
	
# Reverse array
reverse:
	# Loop condition
	bge $t1, $t2, print
	
	# Swap logic, t3 holds value from left, t4 value from right
	lw $t3, arr($t1)
	lw $t4, arr($t2)
	
	sw $t3, arr($t2)
	sw $t4, arr($t1)
	
	# Increment left index and decrement right index
	addi $t1, $t1, 4
	subi $t2, $t2, 4
	
	j reverse
	
# Print array
print:
	# Print prompt for printing array
	la $a0, pmt3
	li $v0, 4
	syscall
	
	# Prepare next loop
	# t0 = array size
	li $t1, 0 # array index
	li $t2, 0 # address index
	
print_loop:
	# Loop exit condition
	beq $t0, $t1, exit_print_loop

	# Get next element
	lw $a0, arr($t2)
	li $v0, 1
	syscall
	
	# Loop iteration handling
	addi $t1, $t1, 1
	addi $t2, $t2, 4
	
	# If not last element
	blt $t1, $t0, print_comma
	
	j print_loop

# Prints comma in between numbers while printing the array
print_comma: 
	bge $t1, $t0, exit_print_loop
	
	la $a0, comma
	li $v0, 4
	syscall
	
	j print_loop

exit_print_loop:
	
	bne $a1, -1, prepare_to_reverse
	
	# End of Program
	li $v0, 10
	syscall
	
	.data
pmt0:	.asciiz "Array size invalid. Try again.\n"
pmt1:	.asciiz "Enter the size of your array: "
pmt2:	.asciiz "Enter next element: "
pmt3:	.asciiz "\nPrinting array: \n"
comma:	.asciiz ", "
size:	.word	0
arr:	.space	80 # Preallocate array for 20 elements
