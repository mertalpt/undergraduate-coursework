## Program for Part 1 of Lab 2
## Converts hexadecimal string to decimal number
## Mert Alp Taytak

	.text
	.globl __start
	
__start:

	jal interactWithUser	
	
	# Print decimal value
	move $t0, $v0 # $v0 will change for syscalls
	la $a0, str3
	li $v0, 4
	syscall
	
	# $t0 is the value
	move $a0, $t0
	li $v0, 1
	syscall
	
	li $v0, 10
	syscall
	

# Before call: $a0 is the address of the string
# After  call: $v0 is the decimal and non-string representation of the string
convertHexToDec:

	# $a0 has the string address
	# Save return address to stack and get string length
	addi $sp, $sp, -20
	sw $s2, 16($sp)	# It will be modified to hold current character
	sw $s1, 12($sp)	# It will be modified to hold current index in string
	sw $s0, 8($sp) 	# It will be modified to hold decimal value
	sw $a0, 4($sp) 	# It will be modified in strLength
	sw $ra, 0($sp) 	# Save return address for the caller
	
	jal strLength
	
	# $v0 is now string length
	move $s1, $v0
	
	# Restore $ra and $a0 from stack
	lw $ra, 0($sp)
	lw $a0, 4($sp)
	addi $sp, $sp, 8
	
	# Now: $a0 is string address, $s1 is string length
	li $s0, 0 	# Use $s0 for decimal value
	sub $s1, $s1, 1 # Index starts from length - 1
	# Use $s2 as current character
	
	loopConvertHexToDec:
	blt $s1, $zero, isValid # End of loop
	
	lb $s2, 0($a0) # Load current character
	
	blt $s2, 48, isInvalid 		# Character is not a hexadecimal value
	blt $s2, 58, isBetween_09 	# Character is between 0 and 9
	blt $s2, 65, isInvalid 		# Character is not a hexadecimal value
	blt $s2, 71, isBetween_AF 	# Character is between A and F
	blt $s2, 97, isInvalid 		# Character is not a hexadecimal value
	blt $s2, 103, isBetween_af 	# Character is between a and f
	j isInvalid			# Character is not a hexadecimal value
	
	# In the following cases,
	# the general idea is to get the true value of the character
	# by subtracting some value corresponding to its ASCII value
	# then using bitshift to get its decimal value corresponding
	# to its place in the hexadecimal number and adding it to
	# $s0 since it is keeping the final decimal value
		
	isBetween_09: 		# ASCII values 48-57
	sub $s2, $s2, 48 	# Value is now between 0 and 9
	j shiftAndAdd
		
	isBetween_AF: 		# ASCII values 65-70
	sub $s2, $s2, 55 	# Value is now between 10 and 15
	j shiftAndAdd
	
	isBetween_af: 		# ASCII values 97-102
	sub $s2, $s2, 87 	# Value is now between 10 and 15
	j shiftAndAdd
	
	shiftAndAdd:
	sll $s0, $s0, 4		# Multilpy $s0 by 16 per hexadecimal digit for base conversion
	add $s0, $s0, $s2	# Decimal value so far
	sub $s1, $s1, 1		# Decrement index
	add $a0, $a0, 1		# Increment address to next character
	j loopConvertHexToDec	# Next iteration of the loop
	
	isInvalid:		# Exit with error output
	li $v0, -1 		# Defined invalid input value
	j exitConvertHexToDec	# Go to exit
	
	isValid:		# Exit with converted value
	move $v0, $s0		# Move decimal value to return register $v0
	j exitConvertHexToDec	# Go to exit
	
	exitConvertHexToDec:	# Restore $s registers from stack and return to caller
	
	lw $s2, 8($sp)		# Restore old $s2
	lw $s1, 4($sp)		# Restore old $s1
	lw $s0, 0($sp) 		# Restore old $s0
	addi $sp, $sp, 12 	# Reset stack pointer
	jr $ra 			# Return to caller

# Before call: $a0 is the address of the string
# After  call: $v0 is the length of the string
strLength:

	# $a0 has the string address
	
	# $v0 will be the length counter
	li $v0, 0
	
	loopStrLength:	
	lbu $t0, 0($a0) 		# Get next character
	beqz $t0, exitStrLength 	# If the character is null terminating character then exit the loop
	beq $t0, 10, exitStrLength 	# If the character is equal to new line then exit the loop
	add $a0, $a0, 1 		# Move to next character
	add $v0, $v0, 1 		# Increment length counter
	j loopStrLength 		# Loop again
	
	exitStrLength:
	jr $ra 				# Return to caller with string length in $v0

# Before call: Takes no parameters
# After  call: Returns no value
interactWithUser:
	
	# Welcome user
	la $a0, str0
	li $v0, 4
	syscall
	
	interactMainBody:
	# Ask and get string
	la $a0, str1
	li $v0, 4
	syscall
	
	la $a0, hex # Address of input buffer
	la $a1, 10   # Maximum number of input characters, 32 bit = 8 hexadecimal with 1 null terminating character
	li $v0, 8
	syscall
	
	# Save return address to the stack
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	# Call convertHexToDec
	# $a0 should be address of the hexadecimal string
	# $v0 will be the decimal value
	la $a0, hex
	jal convertHexToDec
	
	# Restore return address from the stack
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	
	# If $v0 = -1 then input string was invalid
	beq $v0, -1, invalidString
		
	# Return to caller
	jr $ra
	
	invalidString:
	la $a0, str2
	li $v0, 4
	syscall
	j interactMainBody
	
	.data
dec:	.word 0		# Initialize with 0
str0:	.asciiz "This program converts input hexadecimal string to its decimal equivalent number.\n"
str1:	.asciiz "Please enter your hexadecimal number (no more than 8 characters): "
str2:	.asciiz "Invalid hexadecimal number. Please try again.\n"
str3:	.asciiz "Your number in decimal: "
hex:	.space 10	# 8 hexadecimal digits and 1 null terminating character
