## Program for Part 2 of Lab 3
## Convert integer input to floating point output
## Mert Alp Taytak

	.text
	.globl __start
	
__start:
	
	# Call subprogram
	jal	convertIntToFloatInterface
	
	# End program
	li	$v0, 10
	syscall

convertIntToFloatInterface:

	# Save to stack
	sub 	$sp, $sp, 8
	sw	$ra, 0($sp)
	sw	$s0, 4($sp)
	
	## Return point to repeat the process
	cIntToFloatLoop:
	
		# Ask and get integer
		la	$a0, strCItF0
		li	$v0, 4
		syscall
		
		# After this call, $v0 holds the input
		li 	$v0, 5
		syscall		
		
		# Call and get float representation
		move	$a0, $v0
		jal 	convertIntToFloat
			
		# Store float representation in $s0
		move	$s0, $v0
		
		## Print floating number
		# Print message
		la 	$a0, strCItF1
		li	$v0, 4
		syscall
		
		# Temporarily store floating number in stack
		sub	$sp, $sp, 4
		sw	$s0, 0($sp)
		
		# Print floating number
		l.s	$f12, ($sp)
		li	$v0, 2
		syscall
		
		# Reset stack pointer
		add	$sp, $sp, 4
		
		## Print float representation
		# Print message
		jal	printNL
		la 	$a0, strCItF2
		li	$v0, 4
		syscall
	
		# Print representation
		move	$a0, $s0
		li	$v0, 35
		syscall
		
		## Handle repeat request
		# Print prompt
		jal	printNL
		la	$a0, strCItF3
		li	$v0, 4
		syscall
		
		# Get input
		li	$v0, 12
		syscall
		
		# Print new line for formatting
		jal	printNL
		
		# Compare input in $v0 to repeat condition
		beq	$v0, 'r', cIntToFloatLoop
		beq	$v0, 'R', cIntToFloatLoop
		
	# Reaching this line means quit
	la 	$a0, strCItF4
	li	$v0, 4
	syscall
	
	# Print new line for formatting
	jal	printNL
	
	# Restore stack
	lw	$ra, 0($sp)
	lw	$s0, 4($sp)
	add	$sp, $sp, 8
	
	# Return to caller
	jr 	$ra
	
convertIntToFloat:	
	
	# Store to stack
	sub	$sp, $sp, 20
	sw	$ra, 0($sp)
	sw	$s0, 4($sp)
	sw	$s1, 8($sp)
	sw	$s2, 12($sp)
	sw	$s3, 16($sp)
	
	# $a0 has the input number, save it to $s0
	move 	$s0, $a0
	
	# Take absolute value of $s0, two's complement breaks conversion for negative numbers
	bge	$s0, $zero, afterAbs
	sub	$s0, $zero, $s0
	
	afterAbs:
	
	# Get sign bit and shift it into its proper position in $s1
	# move $a0, $s0 is unnecessary here
	jal 	getSignBit
	sll	$s1, $v0, 31
	
	# Get exponent and shift it into its proper position in $s2
	move	$a0, $s0
	jal 	getExponent
	sll	$s2, $v0, 23
	
	## Get mantissa and shift it into its proper position in $s3
	# Get mantissa
	move	$a0, $s0
	move	$a1, $v0
	jal 	getMantissa
	move	$s3, $v0
	
	# Get true exponent from excess exponent
	move	$a0, $s0
	jal 	getExponent
	sub	$v0, $v0, 127
	
	# Shift amount for mantissa is 23 - exponent
	li 	$t0, 23
	sub	$t0, $t0, $v0
	
	# Shift mantissa into its proper position
	sllv	$s3, $s3, $t0
	
	# Add shifted sign bit, exponent and mantissa values to get floating representation
	add	$v0, $s1, $s2
	add	$v0, $v0, $s3
	
	# Restore from stack
	lw	$ra, 0($sp)
	lw	$s0, 4($sp)
	lw	$s1, 8($sp)
	lw	$s2, 12($sp)
	lw	$s3, 16($sp)
	add	$sp, $sp, 20
	
	# Return to caller
	jr	$ra
	
getSignBit:
	
	# $a0 has the number
	slt	$v0, $a0, $zero
	
	# Return to caller
	jr 	$ra
	
getExponent:

	# $a0 has the number, $v0 will be the counter
	li	$v0, 0
		
	exponentLoop:
		# Shift by one to remove one power of 2
		srl	$a0, $a0, 1
		add	$v0, $v0, 1
		
		# If $a0 is 0, no more exponents
		bne	$a0, $zero, exponentLoop
		
	# $v0 now has exponent + 1
	# By IEEE 754, we need to add 127 to exponent, in this case 126
	add	$v0, $v0, 126
	
	# Return to caller
	jr	$ra
	
getMantissa:

	# $a0 has the number
	# $a1 has the exponent excess 127
	
	# Get true exponent
	sub	$a1, $a1, 127
	
	# Get most significant bit in its position
	li	$t0, 1
	sllv	$t0, $t0, $a1
	
	# Get mantissa
	sub	$v0, $a0, $t0
	
	# Return to caller
	jr 	$ra		
	
printNL:
	
	# Store to stack
	sub	$sp, $sp, 8
	sw	$v0, 0($sp)
	sw	$a0, 4($sp)
	
	# Print new line
	la 	$a0, endl
	li 	$v0, 4
	syscall
	
	# Restore from stack
	lw	$v0, 0($sp)
	lw	$a0, 4($sp)
	add	$sp, $sp, 8
	
	# Return to caller
	jr 	$ra
	
	.data
endl:		.asciiz "\n"
strCItF0:	.asciiz "Please enter an integer: "
strCItF1:	.asciiz "Your input as a float: "
strCItF2:	.asciiz "Representation of the floating number in memory: "
strCItF3:	.asciiz "Input 'r' or 'R' to repeat: "
strCItF4:	.asciiz "Quitting..."
