## Program for Part 4 of Lab 1
## ((A/B) - (C+D)) % E
## Mert Alp Taytak

	.text
	.globl __start
	
__start:
	
	# Print prompt
	la $a0, str1
	li $v0, 4
	syscall
	
	bounceFromError:
	
	# Get A, B, C, D, E
	li $t0, 5 # Array size
	li $t1, 0 # Index counter
	li $t2, 0 # Address counter
	
	loop: # For i = 0, i < 5, i++
	
	bge $t1, $t0, endLoop # Break condition
	
	# Print prompt
	la $a0, str2
	li $v0, 4
	syscall
	
	# Get input
	li $v0, 5
	syscall
	
	# Store to array
	sw $v0, arr($t2)
	
	# Increment counters
	addi $t1, $t1, 1
	addi $t2, $t2, 4
	
	j loop
	
	endLoop:
	
	# Load values to saved temporaries
	lw $s0, arr    # A
	lw $s1, arr+4  # B
	lw $s2, arr+8  # C
	lw $s3, arr+12 # D
	lw $s4, arr+16 # E
	
	# $t0 = (A / B)
	move $a0, $s0
	move $a1, $s1
	jal division
	move $t0, $v0
	
	# $t1 = (C + D)
	move $a0, $s2
	move $a1, $s3
	jal addition
	move $t1, $v0
	
	# $t2 = (A/B) - (C+D)
	move $a0, $t0
	move $a1, $t1
	jal subtraction
	move $t2, $v0
	
	# $t3 = ((A/B) - (C*D)) % E
	move $a0, $t2
	move $a1, $s4
	jal remainder
	move $t3, $v0
	
	# Print result
	la $a0, str3
	li $v0, 4
	syscall
	
	move $a0, $t3
	li $v0, 1
	syscall
	
	# End of program
	j endOfProgram

endOfProgram:

	li $v0, 10
	syscall
	
divByZeroErr:

	la $a0, str4
	li $v0, 4
	syscall
	
	j bounceFromError
	
addition: # Returns A + B in $v0

	# $a0 = A
	# $a1 = B
	
	add $v0, $a0, $a1
	
	# Return value in $v0
	jr $ra
	
subtraction: # Returns A - B in $v0

	# $a0 = A
	# $a1 = B
	
	sub $v0, $a0, $a1
	
	# Return value in $v0
	jr $ra
		
multiplication: # Returns A * B in $v0

	# $a0 = A
	# $a1 = B
	
	mul $v0, $a0, $a1
	
	# Return value in $v0
	jr $ra
	
division: # Returns A / B in $v0
	
	# Division by 0 handling
	beq $a1, 0, divByZeroErr
	
	# $a0 = A
	# $a1 = B
	
	div $v0, $a0, $a1
	
	# Return value in $v0
	jr $ra
	
remainder: # Return A % B in $v0

	# Division by 0 handling
	beq $a1, 0, divByZeroErr
	
	# $a0 = A
	# $a1 = B
	
	rem $v0, $a0, $a1
	
	# Restrain result to positive values
	blt $v0, 0, remainderToPos
	
	# Return value in $v0
	jr $ra
	
	remainderToPos:
	
	# Add B once to restrain result to positive
	add $v0, $v0, $a1
	
	# Return value in $v0
	jr $ra

	.data
arr:	.space 20
str1:	.asciiz "This program computes ((A/B) - (C+D)) % E\nPlease enter your numbers in the order: A, B, C, D, E\n-----\n"
str2:	.asciiz "\tEnter your number: "
str3:	.asciiz "-----\nResult: "
str4:	.asciiz "-----\nDivision by zero error! One of the inputs is invalid. Please try again.\n-----\n"