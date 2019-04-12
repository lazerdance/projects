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

# prints a newline \r\n
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

# v0 --> read char from stdin
read_char:
	li $v0, 12
	syscall
	
	jr $ra

# a0 --> string address to print with spaces in between
print_string_with_spaces:
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	
	move $s0, $a0

loop_print:
	lb $a0, ($s0)	
	beq $a0, $zero, end_print
	
	jal print_char
	
	li $a0, ' '		# print space
	jal print_char
	
	addiu $s0, $s0, 1
	j loop_print
	
end_print:
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	addi $sp, $sp, 8
	
	jr $ra
	
# a0 --> upper bound
#
# v0 --> random int (0 <= int < a0)
get_random_int:
	addi $sp, $sp, -8
	sw $s0, 0($sp)
	sw $s1, 4($sp)
	
	li $s0, 42			## ID of generator
	move $s1, $a0		## Save upper bound
	
	## Get 32-bit system time
	li $v0, 30
	syscall

	## Set SEED for Pseudo-random generator
	move $a1, $a0
	move $a0, $s0
	li $v0, 40
	syscall
	
	## Get random int 
	move $a0, $s0
	move $a1, $s1
	li $v0, 42
	syscall
	move $v0, $a0

	lw $s0, 0($sp)
	lw $s1, 4($sp)
	addi $sp, $sp, 8
	
	jr $ra
	
# a0 --> destination
# a1 --> source
# a2 --> length
memcpy:
	# append NULL-byte
	add $t0, $a0, $a2
	sb $zero, ($t0)
	
cp_loop:
	# compare if end reached
	beqz $a2, cp_done
	addi $a2, $a2, -1
	
	# calculate address
	add $t1, $a1, $a2
	add $t0, $a0, $a2
	
	# copy value
	lbu $t2, ($t1)
	sb $t2, ($t0)
	
	j cp_loop
	
cp_done:	
	jr $ra

# a0 --> search_str
# a1 --> char
#
# v0 --> exists: true|false
find_in_str:
	li $v0, 0

search:	
	lbu $t0, ($a0)
	beq $t0, $zero, end_search
	
	addiu $a0, $a0, 1
	bne $t0, $a1, search
	
	li $v0, 1
	
end_search:	
	jr $ra				# jump back

#########################################

### COPY YOUR FUNCTIONS BELOW!!
	
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
	
find_char_pos:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	li $v0, 0
	# keep looping until whether the required char found or end of string
	loopp:
	add $v0, $v0, 1			# increment result index
	lb $t0, 0($a0)			# load next char
	addi $a0, $a0, 1		# shift to next char in string
	beq $t0, $a1, done		# loaded char = query -> query found
	beqz $t0, not_found		# loaded char = 0 -> end of string -> query not found
	j loopp
	
	not_found:
	li $v0, -1
	# return to first function
	done:
	lw $ra, 0($sp)
	addi $sp, $sp, 4						
	jr $ra				# jump back
		# jump back
	
get_searchword_pos:
	addi $sp, $sp, -4
	sw $ra, 0($sp)	

	li $t5, 0		# global index
	li $t8, 0		# words index
	move $t9, $a0		# move n to t9 because we overwrite a0 for the upper function
	
	## loop that returns the index of first char in a required word index
	counter:
	beq $t8, $t9, enda		# word index = query index -> result found
	move $a0, $a2
	jal find_char_pos		# call the comma finder
	beq $v0, -1, failed
	add $t5, $t5, $v0
	move $a2, $a0
	lb $t2, 0($a2)
	beq $t2, $a1, failed
	add $t8, $t8, 1			# increment word index
	j counter
	
	# query not found -> return -1
	failed:
	li $v0, -1
	
	# return result
	enda:
	move $v0, $t5
	lw $ra, 0($sp)
	addi $sp, $sp, 4		
	jr $ra				# jump back
	
	
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

	
set_try_char:
	cloner:
	 lb $t2,0($a0)			# lade erste Char in t2
	 beqz $t2, end_of_stringgg	# t2 = 0 -> End of String
	 bne $t2, $a2, go_fish		# t2 != a2 also das gesuchte Char -> such weiter
	 sb $t2, ($a1)			# ansonsten schreib es in a1
	 go_fish:
	 addi $a0, $a0, 1		# shifte zum naechsten Char
	 addi $a1, $a1, 1		# shifte zur naechsten Stelle
	j cloner
	
	end_of_stringgg:			# End und zuruek zu main
	
	jr $ra
	
		
#########################################

.data
	list:			.asciiz 	"dies,ist,ein,test,satz,der,soll,ganz,toll,sein,donaudampfschiff,"
	delim: 			.byte		','
	list_length:	.word		11
	
	checked_chars: 	.space 		100
	word_space:		.space 		50
	cmp_word:		.space		50
	
	WORD_STR:		.asciiz		"Word is: "
	READING_STR:	.asciiz 	"Next char: "
	READ_CHAR_STR:	.asciiz		"Char read was: "
	FINISHED_STR:	.asciiz		"Word was found!"
	LOST_STR:		.asciiz		"Word was NOT found, you lost!"
	TRIED_STR: 		.asciiz		"The following chars were tried: "
	TRY_CNT_STR:	.asciiz		"Number of remaining tries: "
	MAX_TRIES:		.word		10
	
#########################################

.text
.globl main

main:
	# save regs
	addi $sp, $sp, -20
	sw $ra, 0($sp)
	sw $s0, 4($sp)				# address of word in list
	sw $s1, 8($sp) 				# length of searched word
	sw $s2, 12($sp)				# entered char
	sw $s3, 16($sp)				# try counter
	
	li $s3, 0
	
	# initialise all memory with zero
	la $a0, checked_chars
	li $a1, 0
	li $a2, 100
	jal memset
	
	la $a0, word_space
	li $a1, 0
	li $a2, 50
	jal memset
	
	la $a0, cmp_word
	li $a1, 0
	li $a2, 50
	jal memset
	
	# get random int for list index
	lw $a0, list_length
	jal get_random_int
	li $v0, 10
	
	# get word start adress
	move $a0, $v0
	lbu $a1, delim
	la $a2, list
	jal get_searchword_pos
	
	# calculate address of search word
	la $t0, list
	add $s0, $v0, $t0
	
	# get next delimiter, for calculating length
	move $a0, $s0
	lbu $a1, delim
	jal find_char_pos
	
	# copy string
	la $a0, cmp_word
	move $a1, $s0
	move $a2, $v0
	jal memcpy

	#Counting the word length of the search word
	#Saving the length in $s1
	la $a0, cmp_word
	jal strlen
	move $s1, $v0
	
	# init word_space with dashes
	la $a0, word_space
	li $a1, '_'
	move $a2, $v0
	jal memset
	
	la $a0, WORD_STR
	jal print_string
	
	la $a0, word_space
	jal print_string_with_spaces
	jal print_newline
	
	#Game loop
game_loop:

	# check if all tries are exeeded	
	lw $t0, MAX_TRIES
	bge $s3, $t0, end_game_lost
	
	la $a0, READING_STR
	jal print_string
	
	#Reading char and saving it in $s2
	jal read_char
	move $s2, $v0
	jal print_newline
	
	la $a0, READ_CHAR_STR
	jal print_string
	
	# echo entered char
	move $a0, $s2
	jal print_char
	jal print_newline
	jal print_newline
	
	#Check if char was already called!
	la $a0, checked_chars
	move $a1, $s2
	jal find_in_str
	
	# if char called, let user try again
	bnez $v0, game_loop
	
	# inc try counter, only if char not in string
	la $a0, cmp_word
	move $a1, $s2
	jal find_char_pos

	li $t0, -1
	bne $v0, $t0, char_in_cmp_word
	addi $s3, $s3, 1

char_in_cmp_word:
	

	#Print counter of tries
	la $a0, TRY_CNT_STR
	jal print_string
	lw $t0, MAX_TRIES
	sub $a0, $t0, $s3
	jal print_int
	jal print_newline

	
	# append entered char to list
	la $a0, checked_chars
	move $a1, $s2
	jal append_to_str
	
	# replace all occurences of char in word_space
	la $a0, cmp_word
	la $a1, word_space
	move $a2, $s2
	jal set_try_char
	
	la $a0, WORD_STR
	jal print_string
	
	# print word so far
	la $a0, word_space
	jal print_string_with_spaces
	jal print_newline
	
	la $a0, TRIED_STR
	jal print_string
	
	# print checked chars
	la $a0, checked_chars
	jal print_string
	jal print_newline
	
	# check if searched word is found
	la $a0, word_space
	la $a1, cmp_word
	jal strcmp
	beqz $v0, game_loop
	
	# print finished string
	la $a0, FINISHED_STR
	jal print_string

end_game:
	# restore regs
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	lw $s3, 16($sp)
	addi $sp, $sp, 20
	
	# exit
	li $v0, 10
	syscall
	
end_game_lost:
	jal print_newline
	jal print_newline
	
	# intro
	la $a0, LOST_STR
	jal print_string
	jal print_newline
	
	# print full word 
	la $a0, WORD_STR
	jal print_string
	
	la $a0, cmp_word
	jal print_string
	jal print_newline
	
	# end game
	j end_game

#########################################
