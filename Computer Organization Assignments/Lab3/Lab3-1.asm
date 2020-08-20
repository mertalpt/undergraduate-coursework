## All the code for Preliminary Work of Lab 3
## Author: Mert Alp Taytak

	.text
	.globl __start
	
__start:

	jal userInterface
	
	li $v0, 10
	syscall

# Simple text based interface for preliminary programs
# No error checking
userInterface:

	# Save to stack
	sub	$sp, $sp, 4
	sw 	$ra, 0($sp)
	
	interfaceRepeatPoint:
	
	# Provide menu options
	la	$a0, strMenu
	li 	$v0, 4
	syscall
	
	# Get choice
	li	$v0, 5
	syscall
	
	# Handle choice
	beq 	$v0, 1, menuMul
	beq	$v0, 2, menuSum
	beq	$v0, 3, menuQuit
	j	userInterface # Invalid input
	
	menuMul:
		# Ask and get number 1
		la 	$a0, strMulA
		li	$v0, 4
		syscall
	
		li 	$v0, 5
		syscall
	
		move 	$t0, $v0
	
		# Ask and get number 2
		la	$a0, strMulB
		li	$v0, 4
		syscall
	
		li	$v0, 5
		syscall
	
		move	$a0, $t0
		move 	$a1, $v0
	
		# Call recursiveMultiplication
		jal recursiveMultiplication
	
		# Print result
		move 	$t0, $v0
	
		la 	$a0, strRes
		li 	$v0, 4
		syscall	
	
		move	$a0, $t0
		li 	$v0, 1
		syscall
	
		# Return to menu
		j 	interfaceRepeatPoint

	menuSum:
		# Ask and get number N
		la 	$a0, strSum
		li	$v0, 4
		syscall
	
		li 	$v0, 5
		syscall
	
		move 	$a0, $v0
		
		# Call recursiveSummation
		jal recursiveSummation
		
		# Print result
		move 	$t0, $v0
	
		la 	$a0, strRes
		li 	$v0, 4
		syscall	
	
		move	$a0, $t0
		li 	$v0, 1
		syscall
	
		# Return to menu
		j 	interfaceRepeatPoint
		
	menuQuit:
		# Restore from stack
		lw 	$ra, 0($sp)
		add	$sp, $sp, 4
		jr 	$ra

# Description:
#	Uses recursion to multiply two positive numbers with repeated addition
# Arguments:
# 	a0: Left hand number of the multiplication
# 	a1: Right hand number of the multiplication
# Returns:
# 	v0: Result of the multiplication
recursiveMultiplication:
	
	# Save to stack
	sub	$sp, $sp, 4
	sw 	$ra, 0($sp)
	
	# Check base case
	ble	$a1, $zero, recMulBase
	
	# If not base case, call recursively for a0, a1 - 1
	sub	$a1, $a1, 1
	jal	recursiveMultiplication
	
	# Add number to the result of the recursive call
	# to find the current result of the multiplication
	add	$v0, $v0, $a0
	
	# Return to caller
	lw	$ra, 0($sp)
	add	$sp, $sp, 4
	jr 	$ra
	
	# Handle base case
	recMulBase:
		# Any number multiplied by 0 is 0
		li 	$v0, 0
		
		# Return to caller
		add	$sp, $sp, 4
		jr	$ra
		
# Description:
#	Uses recursion to sum numbers from 1 to N
# Arguments:
# 	a0: N to find sum of numbers from 1 to N
# Returns:
# 	v0: Result of the sum
recursiveSummation:
	
	# Save to stack
	sub	$sp, $sp, 8
	sw	$a0, 4($sp)
	sw 	$ra, 0($sp)
	
	# Check base case
	ble	$a0, $zero, recSumBase
	
	# If not base case, call recursively for a0 - 1
	sub	$a0, $a0, 1
	jal	recursiveSummation
	
	# Add number to the result of the recursive call
	# to find the sum so far
	lw 	$a0, 4($sp)
	add	$v0, $v0, $a0
	
	# Return to caller
	lw	$ra, 0($sp)
	add	$sp, $sp, 8
	jr 	$ra
	
	# Handle base case
	recSumBase:
		# Base case for sum is 0
		li 	$v0, 0
		
		# Return to caller
		add	$sp, $sp, 8
		jr	$ra
		
		
	.data
strMenu:	.asciiz "\n\n\t--- Menu ---\n1. Multiplication A x B\n2. Summation from 1 to N\n3. Quit\n\tPlease enter your choice as the leading number of the option: "
strMulA:	.asciiz "Please enter A in A x B: "
strMulB:	.asciiz "Please enter B in A x B: "
strSum:		.asciiz "Please enter N in Sum 1 to N: "
strRes: 	.asciiz " - Result: "