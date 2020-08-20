## Program 2 of Lab 2
## Consists of many subprograms
## Mert Alp TAYTAK

	.text
	.globl __start
		
__start:

	jal monitor
	
	li $v0, 10
	syscall

# Description:
# 	Ask the user for an integer i, 1 <= i <= 100
# 	Create an integer array of size i, using dynamic memory allocation
# 	Initialize the array with random numbers
# Arguments:
#	None
# Returns:
# 	$v0: Address of the first element of the array
# 	$v1: Size of the array
# Dependencies:
#	sizePrompt:asciiz : String to ask user to input array size
#	invSizeErr:asciiz : String to give user invalid size input error message
readArray:

	# Ask for array size
	la $a0, sizePrompt
	li $v0, 4
	syscall
	
	# Get array size
	li $v0, 5
	syscall
	
	# Size checking
	blt $v0, 1, invalidSizeException
	bgt $v0, 100, invalidSizeException
	
	# Size is valid, save $s registers to be used to stack
	add $sp, $sp, -16
	sw $s3, 12($sp)
	sw $s2, 8($sp)
	sw $s1, 4($sp)
	sw $s0, 0($sp)
	
	# Store $v0 in $s0 and $s1
	move $s0, $v0 # $s0 will be used to return the size
	move $s1, $v0 # $s1 will be used as a loop decremental counter
	
	# Allocate array
	sll $a0, $s0, 2 # Multiply size by 4 to get number of bytes to allocate
	li $v0, 9
	syscall
	
	# Save array address in $v0 to $s1 and $s2
	move $s2, $v0 # $s2 will be used as an address counter
	move $s3, $v0 # $s3 will be used to return array address
	
	# Initialize Random Number Generator
	li $a0, 0  # ID of RNG
	li $a1, 26 # Just a random number to seed RNG
	li $v0, 40
	syscall	
	
	# Initialization loop to populate the array with random numbers
	randomInitializationLoop:
		# Get random value
		li $a0, 0
		li $a1, 200 # Range
		li $v0, 42
		syscall
		
		# $a0 has the random value
		# Put the random value into current space
		sw $a0, ($s2)
		
		# Increment counters
		sub $s1, $s1, 1 # Remaining number of unitialized elements decrease by 1
		add $s2, $s2, 4 # Address increases by 4 bytes per integer
		
		# Repeat condition
		bne $s1, $zero, randomInitializationLoop # Repeat if remaining number of unitialized elements is not zero
		
	# Set return values
	move $v0, $s3 # Address
	move $v1, $s0 # Size
		
	# Restore $s registers from stack
	lw $s3, 12($sp)
	lw $s2, 8($sp)
	lw $s1, 4($sp)
	lw $s0, 0($sp)
	add $sp, $sp, 16
	
	# Return to caller
	jr $ra

	# Executed when input array size is invalid
	# Gives an array message and restarts the subprogram
	invalidSizeException:
		la $a0, invSizeErr
		li $v0, 4
		syscall
		
		j readArray

# Description:
# 	Sorts given array in ascending order using bubble sort algorithm
#	Array size can be 1 or more
# 	Does not return a new array, changes the original array
# 	Also changes $t registers
# Arguments:
# 	$a0: Address of the first element of the array
# 	$a1: Size of the array
# Returns:
# 	None
# Dependencies:
# 	getElementAtIndex:subprogram : Gets element of an array in the given index
bubbleSort:

	# Save registers to be used to stack
	add $sp, $sp, -12
	sw $s1, 8($sp)
	sw $s0, 4($sp)
	sw $ra, 0($sp)
	
	# Save address and size to $s registers
	move $s0, $a0 # Address
	move $s1, $a1 # Size
	
	# Initialize unsorted index
	sub $t0, $s1, 1 # $t0 = i = size - 1
	
	# Outer loop
	# Each loop gets the current maximum unsorted to end of the unsorted
	bubbleSortOuterLoop:
		# Reset swap and j counters
		li $t1, 0 # j = 0
		li $t2, 0 # swaps = 0		
		
		# Inner loop
		# Each loop iterates through the array swapping if current element is bigger than the next element
		bubbleSortInnerLoop:
			# Address of current element
			move $a0, $s0 # Address of the beginning of the array
			move $a1, $t1 # Current index
			
			# Get current element
			jal getElementAtIndex
			
			# $v0 now holds the current element, save it in $t3
			move $t3, $v0
			
			# Address of the beginning did not change
			# Index of next element
			move $a0, $s0
			add $a1, $a1, 1
			
			# Get next element
			jal getElementAtIndex
			
			# $v0 now holds the next element, save it in $t4
			move $t4, $v0
			
			# If current element is greater than next element, swap
			bgt $t3, $t4, bubbleSortSwap
			
			# Return point
			# Continues iteration
			bubbleSortSwapReturn:				
				
				# Else increment counters
				add $t1, $t1, 1 # j++
				
				# If j >= i, continue to outerLoop
				bge $t1, $t0, bubbleSortOuterLoopContinue
				
				# Else continue to next innerLoop
				j bubbleSortInnerLoop
				
			# Handles swapping of current element and next element
			bubbleSortSwap:
			# $t3 has the current element
			# $t4 has the next element
			# $a0 is the address of the beginning of the array
			# $a1 has the index of the next element
			# Let $t5 be used as the address of the swapped elements
			move $t5, $a1
			sll $t5, $t5, 2
			add $t5, $t5, $a0
			sw $t3, 0($t5)
			sub $t5, $t5, 4
			sw $t4, 0($t5)
		
			# Increment swaps counter
			add $t2, $t2, 1
			
			# Return
			j bubbleSortSwapReturn
				
		# Decrements i, checks if i >= 0
		# Continues loop if true or breaks if false
		bubbleSortOuterLoopContinue:
		
		# If no swaps were made, branch to exit
		beq $t2, $zero, exitBubbleSort
		
		sub $t0, $t0, 1
		blt $t0, $zero, exitBubbleSort
		j bubbleSortOuterLoop
	
	# Restore values from stack
	# Clean up and return to caller
	exitBubbleSort:
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)	
	add $sp, $sp, 12
	
	jr $ra

# Description:
# 	Returns the minimum and maximum numbers of an integer array
# Arguments:
# 	$a0: Address of the beginning of the array
# 	$a1: Size of the array
# Returns:
# 	$v0: Minimum element
# 	$v1: Maximum element
# Dependencies:
# 	copyArray:subprogram : Creates a duplicate of the given array
# 	getElementAtIndex:subprogram : Gets element of an array in the given index
minMax:
	# Save to stack
	add $sp, $sp, -20
	sw $s1, 16($sp)
	sw $s0, 12($sp)
	sw $a1, 8($sp)
	sw $a0, 4($sp)
	sw $ra, 0($sp)
	
	# Call copy array
	jal copyArray
	
	# $v0 has the address of the new array and $v1 has the size of the new array
	move $s0, $v0 # Address
	move $s1, $v1 # Size
	
	# Sort copied array
	move $a0, $s0
	move $a1, $s1
	
	jal bubbleSort
	
	# Get maximum value, which should be at size - 1 index
	move $a0, $s0
	sub $a1, $s1, 1
	jal getElementAtIndex
	move $v1, $v0
	
	# Get minimum value, which should be at 0 index
	move $a0, $s0
	li $a1, 0
	jal getElementAtIndex
	# $v0 = minimum value
	
	# Restore from stack
	lw $s1, 16($sp)
	lw $s0, 12($sp)
	lw $a1, 8($sp)
	lw $a0, 4($sp)
	lw $ra, 0($sp)
	add $sp, $sp, 20
	
	# Return to caller
	jr $ra
	
# Description:
# 	Returns the number of unique elements in a given array
# Arguments:
# 	$a0: Address of the beginning of the array
# 	$a1: Size of the array
# Returns:
# 	$v0: Number of unique elements in the array
# Dependencies:
# 	copyArray:subprogram : Creates a duplicate of the given array
# 	getElementAtIndex:subprogram : Gets element of an array in the given index
noOfUniqueElements:
	# Save to stack
	add $sp, $sp, -16
	sw $s2, 12($sp)
	sw $s1, 8($sp)
	sw $s0, 4($sp)
	sw $ra, 0($sp)
	
	# Save arguments
	move $s0, $a0 # address
	move $s1, $a1 # size
	
	# Copy array
	
	jal copyArray
	
	move $s2, $v0 # Address of the new array
	
	# Sort array
	move $a0, $s2
	move $a1, $s1
	
	jal bubbleSort
	
	# Parse array
	
	# Initialize counters
	li $t0, 0 # index counter
	li $t1, 0 # unique element counter
	
	# $t2 will hold current element
	# $t3 will hold next element
	
	uniqueLoop: # Loop while $t0 != size - 1
		# Get current element
		move $a0, $s2
		move $a1, $t0
		jal getElementAtIndex
		
		# $t2 = current element
		move $t2, $v0
		
		# Get next element
		# move $a0, $s2, does not change, commented for efficiency
		add $a1, $a1, 1 # index of next element
		jal getElementAtIndex
		
		# $t3 = next element
		move $t3, $v0
		
		# Compare if $t2 = $t3
		beq $t2, $t3, skipToIteration
		
		# Else, increment unique counter
		add $t1, $t1, 1
		
		skipToIteration:
			# Increment index
			add $t0, $t0, 1
			
			# If index < size - 1, loop again
			sub $t4, $s1, 1
			ble $t0, $t4, uniqueLoop
			
	# Else clean up and exit
	
	# Set return values
	move $v0, $t1
	
	# Restore from stack
	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	add $sp, $sp, 16
	
	# Return to caller
	jr $ra
	
# Interface for other subprograms with fancy features if you wish
monitor:

	# Save to stack
	add $sp, $sp, -12
	sw $s1, 8($sp)
	sw $s0, 4($sp)
	sw $ra, 0($sp)
	
	menuReturn:
	
	# Print options
	la $a0, menuStr
	li $v0, 4
	syscall
	
	# Get choice
	li $v0, 5
	syscall
	
	# Branch to options
	beq $v0, 1, option1 # read
	beq $v0, 2, option2 # print
	beq $v0, 3, option3 # sort
	beq $v0, 4, option4 # min/max
	beq $v0, 5, option5 # no of unique
	beq $v0, 6, option6 # quit
	
	j monitor
	
	# Option 1: Read array
	option1:
		jal readArray
		move $s0, $v0 # Address
		move $s1, $v1 # Size
		
		j menuReturn
		
	# Option 2: Print array
	option2:
		move $a0, $s0
		move $a1, $s1
		jal printArray
		
		j menuReturn
		
	# Option 3: Sort array
	option3:
		move $a0, $s0
		move $a1, $s1
		jal bubbleSort
		
		j menuReturn
		
	# Option 4: Find and print min/max
	option4:
		move $a0, $s0
		move $a1, $s1
		jal minMax
		
		# $v0 is min, $v1 is max
		move $t0, $v0
		move $t1, $v1
		
		# Print
		la $a0, minStr
		li $v0, 4
		syscall
		
		move $a0, $t0
		li $v0, 1
		syscall
		
		la $a0, maxStr
		li $v0, 4
		syscall
		
		move $a0, $t1
		li $v0, 1
		syscall
		
		j menuReturn
		
	# Option 5: Find and print number of unique elements
	option5:
		move $a0, $s0
		move $a1, $s1
		jal noOfUniqueElements
		
		# $v0 is number of unique elements
		move $t0, $v0
		
		# Print
		la $a0, uniqueStr
		li $v0, 4
		syscall
		
		move $a0, $t0
		li $v0, 1
		syscall
		
		j menuReturn
		
	# Option 6: Quit
	option6:
		# Restore from stack
		lw $s1, 8($sp)
		lw $s0, 4($sp)
		lw $ra, 0($sp)
		add $sp, $sp, 12
		
		# Return to caller
		jr $ra

# Print Array
printArray:

	# Save to stack
	add $sp, $sp, -12
	sw $s1, 8($sp)
	sw $s0, 4($sp)
	sw $ra, 0($sp)
	
	# Save arguments
	move $s0, $a0 # Address
	move $s1, $a1 # Size
	
	# Initialize counter
	li $t0, 0
	
	# Initialize limit
	sub $t1, $s1, 2 # Loop will be from 0 to size - 2
	
	printLoop:
		move $a0, $s0 # address
		move $a1, $t0 # index
		jal getElementAtIndex
		
		# $v0 is now current element
		move $a0, $v0
		li $v0, 1
		syscall # Print current element
		
		la $a0, commaStr
		li $v0, 4
		syscall # Print comma
		
		# Increment counter
		add $t0, $t0, 1
		
		# If index <= size - 2, loop again
		ble $t0, $t1, printLoop
		
	# Else print last element then quit loop
	move $a0, $s0
	move $a1, $t0
	jal getElementAtIndex
	
	move $a0, $v0
	li $v0, 1
	syscall
	
	# Quit sequence
	
	# Restore from stack
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	add $sp, $sp, 12
	
	# Return to caller
	jr $ra

# Description:
# 	Copies given array and returns its address and size
# Arguments:
# 	$a0: Address of the first element of the array
# 	$a1: Size of the array
# Returns:
# 	$v0: Address of the beginning of the new array
# 	$v1: Size of the new array
# Dependencies:
# 	getElementAtIndex:subprogram : Gets element of an array in the given index
copyArray:
	# Save to stack
	add $sp, $sp, -16
	sw $s2, 12($sp)
	sw $s1, 8($sp)
	sw $s0, 4($sp)
	sw $ra, 0($sp)
	
	# Save address and size of the original array
	move $s0, $a0 # Address
	move $s1, $a1 # Size
	
	# Space to allocate for the new array
	sll $a0, $s1, 2
	li $v0, 9
	syscall
	
	# Save address of the new array
	move $s2, $v0
	
	# Initialize counter
	li $t0, 0
	
	copyArrayLoop:
		# Get element from copied array
		move $a0, $s0 # address
		move $a1, $t0 # index
		jal getElementAtIndex
		
		# $v0 now has the element to copy
		
		# $t1 will hold the address of the element to copy to
		sll $t1, $t0, 2
		add $t1, $t1, $s2
		
		# Store copied value
		sw $v0, 0($t1)
		
		# Increment counter
		add $t0, $t0, 1
		
		# If counter < size, repeat
		blt $t0, $s1, copyArrayLoop
		
	# Else, the loop is over
	# Set return values
	move $v0, $s2
	move $v1, $s1
	
	# Restore from stack
	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	add $sp, $sp, 16
	
	# Return to caller
	jr $ra

# Description:
# 	Returns element of the array at given index
# Arguments:
# 	$a0: Address of the first element of the array
# 	$a1: Index of the element, starting from 0
# Returns:
# 	$v0: Element at given index
# Dependencies:
# 	None
getElementAtIndex:

	# Store $s registers to be used to stack
	add $sp, $sp, -8
	sw $s1, 4($sp)
	sw $s0, 0($sp)
	
	# Move arguments into $s0 and $s1
	move $s0, $a0
	move $s1, $a1

	# Calculate address shift
	sll $s1, $s1, 2
	
	# Find address of the element
	add $s0, $s0, $s1
	
	# Get element
	lw $v0, ($s0)
	
	# Restore $s registers from stack
	lw $s1 4($sp)
	lw $s0 0($sp)
	add $sp, $sp, 8
	
	# Return to caller
	jr $ra

	.data
sizePrompt:	.asciiz "Please enter an array size between 1 and 100: "
invSizeErr:	.asciiz "Invalid size input. Trying again...\n"
menuStr:	.asciiz "\nMenu:\n\t1. Read Array\n\t2. Print Array\n\t3. Sort Array\n\t4. Print minimum and maximum\n\t5. Print number of unique elements\n\t6. Quit\n"
minStr:		.asciiz "Minimum element is: "
maxStr:		.asciiz "\nMaximum element is: "
uniqueStr:	.asciiz "\nNumber of unique elements is: "
commaStr:	.asciiz ", "