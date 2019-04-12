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

########################################################################################

# a0 --> string address
# a1 --> char (as char)
append_to_str:
	# save regs	also pointer zwischenspeichern
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	append:
	lb $t1,0($a0)		# lade 1. Char
	beqz $t1,append_it	# ist die Stelle freie ? schreib drauf
	addi $a0, $a0, 1	# ansonsten such weiter und shifte zur naechsten Stelle
	j append
	
	append_it:
	sb $a1,($a0)		# schreibe a1 an der 1. freie Stelle in a0
	# restore regs	also pointer wieder laden
	lw $ra, 0($sp)
	addi $sp, $sp, 4
		
	jr $ra

		
# a0 --> source address
# a1 --> target address
# a2 --> char (as char)
set_try_char:
	
	cloner:
	 lb $t2,0($a0)			# lade erste Char in t2
	 beqz $t2, end_of_string	# t2 = 0 -> End of String
	 bne $t2, $a2, go_fish		# t2 != a2 also das gesuchte Char -> such weiter
	 sb $t2, ($a1)			# ansonsten schreib es in a1
	 go_fish:
	 addi $a0, $a0, 1		# shifte zum naechsten Char
	 addi $a1, $a1, 1		# shifte zur naechsten Stelle
	j cloner
	
	end_of_string:			# End und zuruek zu main
	
	jr $ra
	
########################################################################################

.data
	search_word:	.asciiz 	"alabaster"
	word_space:		.asciiz 	"_________"
	checked_chars: 	.space 		100
	NULLWORD:		.word		0					# word with content 0 to limit `checked_chars` above

########################################################################################

.text
.globl main

main:
	# save regs
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	# testing functions with char 'a':
	
	la $a0, search_word
	la $a1, word_space
	li $a2, 'a'
	jal set_try_char
	
	la $a0, checked_chars
	li $a1, 'a'
	jal append_to_str
	
	la $a0, word_space
	jal print_string
	jal print_newline
	
	la $a0, checked_chars
	jal print_string
	jal print_newline

	# restore regs
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	
	# exit
	li $v0, 10
	syscall

#########################################

