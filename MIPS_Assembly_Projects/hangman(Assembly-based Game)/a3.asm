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
# a1 --> delimiter (as char)
#
# v0 --> position of occurence of char
find_char_pos:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	li $t3, 1		# Zaehler bei 1 Anfagen (falls das erste Char is Delimiter)
	
	counter2:
	 lb $t2, 0($a0)		# lade den naechsten Char von der String in t2
	 beq $t2, $a1, delimiter_found	# t2 = delim -> geh zurueck in der ersten Funktion
	 beqz $t2, end_of_string	# t2 = 0 -> End of String und Delimiter nicht gefunden
	 addi $a0, $a0, 1		# zum naechsten Char shiften
	 add $t3, $t3, 1		# Zaehler + 1
	j counter2
	
	end_of_string:		# Ende und kein Delimiter -> error -> -1
	li $v0, -1
	j done
	
	delimiter_found:	# Delimiter gefunden -> v0 = Zaehler
	add $v0, $zero, $t3
	
	done:			# zurueck zu 1. Fkt
	lw $ra, 0($sp)
	addi $sp, $sp, 4	
	jr $ra				# jump back
	
# a0 --> n (for list[n], as int)
# a1 --> delimeter (as char)
# a2 --> word list address
#
# v0 --> position in list where word starts
get_searchword_pos:
	# save regs
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	sw $s0, 4($sp)		## Speicher fuer s0 alloziieren
	
	add $s0, $zero, $a0	# a0 in s0 uebertragen weil wir ueberschreiben a0 spaeter
	li $t0, 0		# counter fuer die besuchte Woerter
	
	counter:
	 beq $t0, $s0, end		# suchen bis n Woerter, dann ausspringen
	 lb $t2, 0($a2)			# lade den ersten Char in t2
	 beq $t2, $a1, count_one	# t2 = delimiter -> direkt 1 zum Ergebnis
	 beqz $t2, failed		# t2 = 0 -> Anfrage nicht gefunden
	 add $a0, $zero, $a2		# uebertrage a2 an a0 fuer die 2. Funktion
	 jal find_char_pos		# ruf den delimiter finder an
	 bltz $v0, failed		# Ergebnis vom delimiter finder = -1 -> delimiter wurde nicht gefunden
	 add $t1, $t1, $v0		# ansonsten das Ergebnis davon an t1 addieren
	 add $a2, $zero, $a0		# a2 aktualisieren fuer das naechste loop
	 addi $a2, $a2, 1		# zum naechsten Char shiften
	 add $t0, $t0, 1		# woerter_zaehler inkrementieren
	j counter
	
	failed:				# gib -1 zurueck an main
	li $v0, -1
	j return
	
	count_one:		
	add $t1, $t1, 1		# 1 zum aktuellen Ergebnis addieren
	addi $a2, $a2, 1	# zum naechsten Char shiften
	j counter
	
	end:
	add $v0, $zero, $t1	# bei erfolgreicher Suche Ergebnis in v0 uebertragen
	return:
	# restore regs
	lw $ra, 0($sp)
	lw $s0, 4($sp)			# Speicher wieder freigeben
	addi $sp, $sp, 8	
	jr $ra				# jump back
	
########################################################################################

.data
	string:			.asciiz 	"dies,ist,ein,test,satz,der,soll,ganz,toll,sein,"
	delim: 			.byte		','

########################################################################################

.text
.globl main

main:
	# save regs
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	# Call function get position [3]
	li $a0, 3
	lbu $a1, delim
	la $a2, string
	jal get_searchword_pos
	
	# print result of get_searchword_pos, should be 13
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

