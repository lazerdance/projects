#v20180326

#########################################
### ATTENTION: DO NOT CHANGE THE FOLLOWING LINE !!
### sed-hook ###
### Implement your solution below
#########################################

# 
# Checks if file of board has the correct size, only valid chars
# $v0 -> 
#   0 if board is correct
#   1 if board is NOT correct
#

check_board:
### TODO Implement your solution here ###
    
    ### Register gemass Konventionen sichern
    addi $sp, $sp, -36
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)
    sw $s3, 16($sp)
    sw $s4, 20($sp)
    sw $s5, 24($sp)
    sw $s6, 28($sp)
    sw $s7, 32($sp)

    li $v0, 1

    ## Argumente in temporary Registers laden
    la $t0, hit
    lb $s0, 0($t0)

    la $t1, ship
    lb $s1, 0($t1)

    la $t2, miss
    lb $s2, 0($t2)

    la $t3, water
    lb $s3, 0($t3)

    la $t4, lf
    lb $s4, 0($t4)

    ## temporary Registers wieder freigeben
    li $t0, 0
    li $t1, 0
    li $t2, 0
    li $t3, 0
    li $t4, 0

    ## board in zwei Registers laden 
    la $s6, board       ## board index
    la $t9, board

    ## t6 wird index i=0
    li $t6, 0
    ## t7 wird length zugewiesen length = 110 (100 Charachters + 10 lf)
    li $t7, 110
    ## t8 ist die Devisionsfaktor
    lw $t8, size
    add $t8, $t8, 1

    length_calc:
    
    lb $t3, 0($t9)
    beqz $t3, end_calc
    addi $t9, $t9, 1
    j   length_calc

    end_calc:
    la $t3, board
    sub $s7, $t9, $t3

    ## Diese Schleife ueberpruft jeden Character im Board mit den gultigen Characters
    check_loop:
    beq $t6, $s7, check_length          ## beim Ende des Checken uberpruft ob die Laenge des Board gultig ist
    lb $s5, 0($s6)                      ## Der Wert von jedem Character nach dem Increment wird in s5 geladen

    ## ist board index ein Hit wird t0 1 zugewiesen
    seq $t0, $s0, $s5
    ## ist board index ein Ship wird t0 1 zugewiesen
    seq $t1, $s5, $s1
    ## ist board index ein Miss wird t0 1 zugewiesen
    seq $t2, $s5, $s2
    ## ist board index ein Water wird t0 1 zugewiesen
    seq $t3, $s5, $s3
    ## ist board index ein LF wird t0 1 zugewiesen
    seq $t4, $s5, $s4       

    add $t0, $t0, $t1       ## die seq Ergebnisse auf einander addieren
    add $t0, $t0, $t2       ## das gleiche Spiel
    add $t0, $t0, $t3
    add $t0, $t0, $t4       ## am Ende der Addition muss t0 einen Wert von entweder 0 oder 1 haben


    beqz $t0, error         ## wird keine der mogliche Characters getroffen -> t0 = 0 -> character nicht gultig
    li $v0, 0               ## ansonsten t0=1 -> Character ist gulltig
    bnez $t4, check_position    ## LF = t4 = 1 -> checken ob das LF nach 10 Characters liegt. sonst error
    addi $t6, $t6, 1        ## increment index i
    addi $s6, $s6, 1        ## increment board index
    
    j check_loop

    check_position:
    addi $t6, $t6, 1
    div	$t6, $t8			# $t6 / $t8
    mfhi $t9					# $t9 = $t6 mod $t8 
    bnez $t9, error
    addi $s6, $s6, 1
    li $v0, 0

    j check_loop


    error:
    li $v0, 1

    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    lw $s3, 16($sp)
    lw $s4, 20($sp)
    lw $s5, 24($sp)
    lw $s6, 28($sp)
    lw $s7, 32($sp)
    addi $sp, $sp, 36

    jr $ra

    ## messt ob die Laenge von Board 10*10
    check_length:
    li $t0, 110
    div	$s7, $t0			# $t0 / $t1
    mfhi $t6					# $t3 = $t0 mod $t1 
    bnez $t6, error
    li $v0, 0

    end_loop:
    li $v0, 0

    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    lw $s3, 16($sp)
    lw $s4, 20($sp)
    lw $s5, 24($sp)
    lw $s6, 28($sp)
    lw $s7, 32($sp)
    addi $sp, $sp, 36

    jr $ra


#########################################
### Implement your solution above
### ATTENTION: DO NOT CHANGE THE FOLLOWING LINE !!
### sed-hook ###
#########################################

# data
    .data
    hit:			.asciiz 	"-"
    ship:			.asciiz 	"x"
    miss:			.asciiz 	"o"
    water:			.asciiz 	" "

    lf:				.byte		10
    size: 			.word 		10

    board:			.asciiz		"oo  x xx o\n x  - o o \n x  x  xxx\n x  -  oo \n  o - o - \n oo o  o- \nxxxx  x - \n   o  -   \nxx    -   \n  ooo - x \n"
#	board:			.asciiz		"          \n          \n          \n          \n          \n          \n          \n          \n          \n          \n"
#	board:			.asciiz		"    x xx  \n x  x     \n x  x  xxx\n x  x     \n    x   x \n        x \nxxxx  x x \n      x   \nxx    x   \n      x xx\n"

    board_correct: 	.asciiz		"Spielplan ist richtig aufgebaut.\n"
    board_wrong:	.asciiz		"Fehler: Spielplan ist nicht richtig aufgebaut!\n"

#########################################

# main
    .text
    .globl main

    main:
# save regs
    addi $sp, $sp, -4
    sw $ra, 0($sp)

# call function
    jal check_board

# print right message according to result
    beq $v0, $zero, is_corr
    la $a0, board_wrong
    li $v0, 4
    syscall
    j end

    is_corr:
    la $a0, board_correct
    li $v0, 4
    syscall

    end:
# restore regs
    lw $ra, 0($sp)
    addi $sp, $sp, 4

    jr $ra

#########################################
