## Program for Part 5 of Lab 1
## Array operations with a menu
## Written by Mert Alp Taytak
	
	.text
	.globl __start
	
__start:	# Set up the program

	# Ask and get array size
	la $a0, strSize
	li $v0, 4
	syscall
	
	li $v0, 5
	syscall
	sw $v0, size
	
	# Check if array size is valid
	
	li $t0, -1
	
	bgt $v0, 100, sizeInvalid
	blt $v0, 1, sizeInvalid
	
	li $t0, 0
	
	# Set $s0 to size
	move $s0, $v0
	
	# Set $s1 to 0, $s1 will be used as a counter
	li $s1, 0
	
	# Set $s2 to 0, $s2 will be used as an address register
	li $s2, 0
	
	# Set $a0 to address of string to use in the following loop
	la $a0, strNumber
	
sizeInvalid:	# Handles invalid array size

	bne $t0, -1, populationLoop
	la $a0, strInvalid
	li $v0, 4
	syscall
	j __start
	
populationLoop:	# Loop used to populate array
	
	# Ask and get next element
	li $v0, 4
	syscall
	
	li $v0, 5
	syscall
	
	sw $v0, arr($s2)
	
	# Increment counter and address register
	addi $s1, $s1, 1
	addi $s2, $s2, 4
	
	# Loop condition
	blt $s1, $s0, populationLoop

menuLoop:	# Menu loop

	li $v0, 4 # USed to print strings in the following code

	# Print menu
	la $a0, strMenu
	syscall
	
	# Print option A
	la $a0, strMenuA
	syscall
	
	# Print option B
	la $a0, strMenuB
	syscall
	
	# Print option C
	la $a0, strMenuC
	syscall
	
	# Print option D
	la $a0, strMenuD
	syscall
	
	# Print selection prompt
	la $a0, strSelection
	syscall
	
	# Get user input
	li $v0, 12
	syscall
	
	# Branching to options
	
	beq $v0, 'a', menuA
	beq $v0, 'b', menuB
	beq $v0, 'c', menuC
	beq $v0, 'd', menuD
	
	j invalidInput # If no match is found then the input is invalid

menuA:		# Menu option A: Find summation of numbers stored in the array which is greater than an input number

	# Ask and get number
	la $a0, strNumber
	li $v0, 4
	syscall
	
	li $v0, 5
	syscall # $v0 is now number
	
	move $a1, $v0
	j sumGreater
	
	returnToMenuA: # $s4 hold the value to print
	
	la $a0, strGreater
	li $v0, 4
	syscall
	
	move $a0, $s4
	li $v0, 1
	syscall
	
	j menuLoop

menuB:		# Menu option B: Find summation of even and odd numbers and display them

	j sumOddEven

	returnToMenuB:
	
	la $a0, strOdd
	li $v0, 4
	syscall
	
	move $a0, $s7
	li $v0, 1
	syscall
	
	la $a0, strEven
	li $v0, 4
	syscall
	
	move $a0, $s6
	li $v0, 1
	syscall
	
	# Reset sum registers	
	andi $s6, 0
	andi $s7, 0
	
	j menuLoop	

menuC:		# Menu option C: Display the number of occurrences of the array elements divisible by a certain input number

	# Get input number
	la $a0, strNumber
	li $v0, 4
	syscall
	
	li $v0, 5
	syscall
	
	move $a1, $v0 # Argument of countDivisibles for the modulo
	
	j countDivisibles
	
	returnToMenuC:
	
	la $a0, strDivisible # Print prompt
	li $v0, 4
	syscall
	
	move $a0, $s5 # Print result
	li $v0, 1
	syscall
	
	j menuLoop

menuD:		# Menu option D: Quit

	j endOfProgram
	
sumGreater:	# Finds the sum of numbers in the array greater than given number

	# $s0 is array size
	li $s1, 0 # index counter
	# $a1 is input number
	
	sumReturn:
	
	bge $s1, $s0, returnToMenuA # End of loop if index exceeds size
	
	move $a0, $s1
	jal elementAtIndex # $v0 is now element at given index
	
	# Increment index counter
	add $s1, $s1, 1
	
	# Add to sum if condition is met
	move $a0, $v0
	bgt $a0, $a1, adderFour
	
	j sumReturn

sumOddEven:	# Finds sum of odd numbers in the array

	# $s0 is size
	li $s1, 0 # index counter
	li $a1, 2 # argument of modulo
	
oddEvenHelper:	# s1 is index counter, $v1 is sum

	bge $s1, $s0, returnToMenuB # End of loop if index exceeds size
	
	move $a0, $s1
	jal elementAtIndex # $v0 is element at index now
	
	move $s2, $v0 # Keep value in $s2 as $v0 will change
	move $a0, $v0 # Move value to argument
	jal modulo # $v0 is now remainder
	
	move $a0, $s2 # Pass element as argument to addToSum functions
	
	beq $v0, 0, adderTwo
	beq $v0, 1, adderOne
	
	returnToHelper: # Return address
	
	# Increment index
	add $s1, $s1, 1

	j oddEvenHelper # Loop

countDivisibles:# Find number of numbers that are divisible by the given number, !!! $a1 must be set to divisor number BEFORE the call !!!

	# $s0 is array size
	li $s1, 0 # index counter
	
	divisibleReturn:
	
	bge $s1, $s0, returnToMenuC # End of loop if index exceeds size
	
	move $a0, $s1
	jal elementAtIndex # $v0 is now element at index
	
	move $s2, $v0 # Keep value to sum later if necessary
	move $a0, $v0 # Pass value as argument to modulo
	jal modulo # $v0 is now the remainder
	
	# Increment index
	add $s1, $s1, 1
	
	# If remainder is not 0 repeat
	bne $v0, 0, divisibleReturn
	
	# Else add to sum
	j adderThree	

modulo:		# Finds remainder of a number divided by another, CHANGES $v0

	remu $v0, $a0, $a1 # $v0 = $a0 % $a1
	jr $ra # Return to caller
		
invalidInput:

	la $a0, strInvalid
	li $v0, 4
	j menuLoop
	
elementAtIndex:	# Gives element at given index in $v0

	# $a0 is index
	mul $t0, $a0, 4
	lw $v0, arr($t0)
	jr $ra
	
adderOne:	# Adds $a0 to $s7

	add $s7, $s7, $a0
	j returnToHelper

adderTwo:	# Adds $a0 to $s6

	add $s6, $s6, $a0
	j returnToHelper
	
adderThree:	# Adds 1 to $s5

	add $s5, $s5, 1
	j divisibleReturn
	
adderFour:	# Adds $a0 to $s4

	add $s4, $s4, $a0
	j sumReturn
	
endOfProgram:	# End of program
	
	li $v0, 10
	syscall

	.data
arr:		.space 400
size:		.word 0 # Holds array size
strSize:	.asciiz "\nEnter array size: "
strInvalid:	.asciiz "\nSize invalid, please pick a number between 1 and 100\n"
strNumber:	.asciiz "\nEnter your number: "
strSum:		.asciiz "\nSum is: "
strOccurence:	.asciiz "\nNumber of occurences are: "
strMenu:	.asciiz "\n-----\nMenu\n-----\n"
strMenuA:	.asciiz "\ta. Find summation of numbers stored in the array which is greater than an input number.\n"
strMenuB:	.asciiz "\tb. Find summation of even and odd numbers and display them.\n"
strMenuC:	.asciiz "\tc. Display the number of occurrences of the array elements divisible by a certain input number.\n"
strMenuD:	.asciiz "\td. Quit.\n"
strSelection:	.asciiz "\nPlease enter 'a' or 'b' or 'c' or 'd' for your option of choice: "
strOdd:		.asciiz "\nSum of all odd numbers: "
strEven:	.asciiz "\nSum of all even numbers: "
strDivisible:	.asciiz "\nNumber of numbers divisible by your number: "
strGreater:	.asciiz "\nSum of numbers greater than your number: "
