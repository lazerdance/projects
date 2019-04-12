########################################################################################

.text
j main	# workaround if MARS isn't set up properly

# a0 --> int to print
print_int:
	li $v0, 1
	syscall

	jr $ra
	
# a0 --> char to print
print_char:
	li $v0, 11
	syscall
	
	jr $ra

print_newline:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	addi $a0, $0, '\r'
	jal print_char
	
	addi $a0, $0, '\n'
	jal print_char
	
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	
	jr $ra

# a0 --> string address to print
print_string:
	li $v0, 4
	syscall

	jr $ra

############################################
	
# a0 --> destination address
# a1 --> char
# a2 --> length (as int)
memset:

	li $t1, 0	# index fuer die Schreib Funktion
	
	loop:
	beq $t1, $a2, abort		# springt raus wenn wir genuge 'e' schreiben. also wenn wir Stringlaenge erreichen
	sb $a1, ($a0)			# e schreiben in a0
	addi $a0, $a0, 1		# zur naechster freier Stelle in a0 schieben
	add $t1, $t1, 1			# Chars Zaehler inkrementieren
	j loop
	
	## geht zurueck zu main
	abort:
	jr $ra
	
# a0 --> string address
#
# v0 --> length of str
strlen:
	addi $sp, $sp, -8
	sw $ra, 0($sp)			# Stack pointer fuer main spaeter speichern
	sw $s0, 4($sp)			## Speicher fuer s0 reservieren
	
	li $t5,0		# Chars-Zaehler
	
	count:
	lb $s0, 0($a0)		## lade den ersten Char in s0
	addi $a0, $a0, 1	# schiebe zum naechsten Char in der String in a0
	beqz $s0, end_of_string		# beenden beim Erreichen von Null in der String
	add $t5, $t5, 1		# den Zaehler inkrementieren
	j count			# nochmal loopen
	
	end_of_string:		# gibt die Anzahl der Chars in der String zurueck in v0
	add $v0, $zero, $t5
	# restore regs
	lw $ra, 0($sp)		# Stack pointer zuruckholen
	lw $s0, 4($sp)		## Speicher fuer s0 wieder freigeben
	addi $sp, $sp, 8
	
	jr $ra
	
	
# a0 --> string1 address
# a1 --> string2 address
#
# v0 --> equal: true (1)|false (0)
strcmp:
	
	string_compare:
	lb $t0,0($a0)			# lade den ersten Char in t0
	addi $a0,$a0,1			# verschiebe zum naechsten Char
	lb $t1,0($a1)			# lade den ersten Char in t1
	addi $a1,$a1,1			# verschiebe zum naechsten Char
	sub $t2, $t1, $t0		# t2 = t1 - t0		Chars als Integers subtrahieren
	bnez $t2, not_equal		# t2 != 0 -> Chars sind verschieden
	beqz $t0, equal			# Chars sind gleich und gleich 0 -> Ende der Strings gleichzeitig
	j string_compare		# loopen bis Null-Terminierung oder bis Ungleichheit
	
	
	## gibt 0 zurueck fuer ungleiche Strings
	 not_equal:
	 li $v0, 0
	 j end
	## gibt 1 zurueck fuer gleiche Strings	  
	equal:
	li $v0, 1
	
	## geh zurueck zu main
	end:
	jr $ra
	
########################################################################################

.data
	teststring1:		.asciiz		"teststring1_test"	# string 1
	teststring2:		.asciiz		"eeeeeeeeeee"		# string 2
	testspace:			.space 		50					# 50 bytes of space for experimenting with
	NULLWORD:			.word		0					# word with content 0 to limit `testspace` above

########################################################################################

.text
.globl main

main:
	
	# save regs
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	# compare both teststring1 and teststring2
	la $a0, teststring1
	la $a1, teststring2
	jal strcmp
	
	# print result of strcmp
	move $a0, $v0
	jal print_int
	jal print_newline
	
	# get length of teststring1
	la $a0, teststring2
	jal strlen
	
	# initialise testspace with 'e'
	la $a0, testspace
	li $a1, 'e'
	move $a2, $v0
	jal memset
	
	# compare both teststring1 and testspace if set properly
	la $a0, testspace
	la $a1, teststring2
	jal strcmp
	
	# print result of strcmp
	move $a0, $v0
	jal print_int
	jal print_newline
	
	# restore regs
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	
	# exit
	li $v0, 10
	syscall

#########################################
