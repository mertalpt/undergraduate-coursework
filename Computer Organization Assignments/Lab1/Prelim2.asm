## Lab 1 Preliminary Work
## Program 2
## x = (c - d) & 2

	.text
	.globl __start
	
__start:
	la $a0, prompt1
	li $v0, 4
	syscall # Output prompt
	
	li $v0, 5
	syscall # Take input c
	sw $v0, c
	
	la $a0, prompt2
	li $v0, 4
	syscall # Output prompt
	
	li $v0, 5
	syscall # Take input d
	sw $v0, d
	
	# Inputs are taken
	
	lw $t0, c # c into t0
	lw $t1, d # d into t1
	sub $t2, $t0, $t1 # t2 gets t0 - t1 MEANING t2 = c - d
	andi $t3, $t2, 1 # If (c - d) ends with bit 1, result is 1, else result is 0
	
	# Processing is done
	
	la $a0, prompt3
	li $v0, 4
	syscall # Output prompt
	
	move $a0, $t3
	li $v0, 1
	syscall # Print result
	
	# Result is printed
	
	li $v0, 10
	syscall # End of program	

	.data
c:	.word 0
d:	.word 0
prompt1:.asciiz "Please enter c: "
prompt2:.asciiz "Please enter d: "
prompt3:.asciiz "Result of (c - d) % 2 is "