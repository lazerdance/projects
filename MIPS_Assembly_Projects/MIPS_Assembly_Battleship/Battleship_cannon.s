#v20180326

#
# Returns random 32bit number. Must be initialized with a seed
#
# $a0 -> &seed for init
#
# $v0 -> "random" number
#

get_random_int:
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    # load seed
    lw $t0, 0($a0)

    # taps 1,5,6,31

    # t ap 31
    srl $t1, $t0, 31
    andi $t2, $t1, 1

    # tap 6
    srl $t1, $t0, 6
    andi $t1, $t1, 1
    xor $t2, $t2, $t1

    # tap 5
    srl $t1, $t0, 5
    andi $t1, $t1, 1
    xor $t2, $t2, $t1

    # tap 1
    srl $t1, $t0, 1
    andi $t1, $t1, 1
    xor $t2, $t2, $t1

    # add or result
    sll $t2, $t2, 31
    srl $t0, $t0, 1
    or $t0, $t0, $t2

    # store seed
    sw $t0, 0($a0)

    move $v0, $t0
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

#########################################
### sed-hook ###
#########################################

# 
# Fires a shot and changes board. Always has to return coordinates for valid shots (not shot yet). If none can be found, return "-" (dash).
# 
# $v0 -> ascii letter (capital) which row (z.B. "A")
# $v1 -> ascii number which col (z.B. "2")
# ==> Shoots field "A2"
#
#

    shoot_cannon:

    ### Register gemass Konventionen sichern
    addi $sp, $sp, -28
    sw $ra, ($sp)
    sw $s1, 4($sp)
    sw $s2, 8($sp)
    sw $s3, 12($sp)
    sw $s4, 16($sp)
    sw $s5, 20($sp)
    sw $s7, 24($sp)



    li $t9, 0           ## index i=0
    lw $a1, size        ## size wird in a1 geladen

    mult $a1, $a1
    mflo $s5        ## size*size = 100 Chars

    ## Seed generieren
    seed_factory:
    bgt $t9, $s5, battlefield_over      ## index = Length = 100 -> keine Schusse mehr möglich
    addi $t9, $t9, 1                ## index i = i+1
    la $t7, board
    la $a0, seed                    ## das initial seed laden
    jal get_random_int              ## das erste Seed konvertieren zu einer Zahl von 0 zu 99
    
    blt $v0, $zero, sign_switch     ## ist die Zahl negative -> zu Positiv konvertieren
        div $v0, $s5                ## v0 / $s5
        mfhi $t0                    ## Seed $t0 = v0 / $s5 (0->99)
        j compare                   

    ## konvertiert die Zahl von negativ zu positiv
    sign_switch:
        sub $v0, $zero, $v0         ## N.Z. - 0 = P.Z.
        div $v0, $s5                ## v0 / $s5
        mfhi $t0                    ## Seed $t0 = v0 / $s5 (0->99)

    ## checkt ob die Koordinaten des Seed nie geschossen wurden davor        
    compare:
    add $t7, $t7, $t0           ## Schuss index auf dem Board
    lb $s7, 0($t7)              ## s7 = Schuss index auf Board

    ## die Werte von den Charachters laden
    lb $s1, ship               
    lb $s2, water
    lb $s3, hit
    lb $s4, miss

    seq $t8, $s7, $s1           ## ist diese Koordinaten = Ship -> t8 = 1. Ansonsten t8 = 0
    bnez $t8, strike            ## t8 != 0 -> ist der Schuss erfolgreich
    seq $t8, $s7, $s2           ## ist diese Koordinaten = water -> t8 = 1. Ansonsten t8 = 0
    bnez $t8, strike

    j seed_factory
    
    ## ladet die Koordinaten des Schuss in v0 und v1
    strike:
    sb $s3, 0($t7)             ## gibt Koordinaten auf dem Board
    div		$t0, $a1			# $t0 / $a1
    mflo	$v0					# $v0 = floor($t0 / $a1) 
    mfhi	$v1					# $v1 = $t0 mod $a1 
    
    addi $v0, $v0, 65           ## konvertiert v0 Wert zu einer Buchstabe A->J
    addi $v1, $v1, 48           ## konvertiert v0 Wert zu einer Zahl 0->9

    j game_over

    ## Print -- wenn es keine mehr Schusse vorhanden
    battlefield_over:
    li $v0, 45
    li $v1, 45

    ### beenden und Register wieder herstellen
    game_over:
    lw $ra, ($sp)
    lw $s1, 4($sp)
    lw $s2, 8($sp)
    lw $s3, 12($sp)
    lw $s4, 16($sp)
    lw $s5, 20($sp)
    lw $s7, 24($sp)
    addi $sp, $sp, 28
    
    jr $ra

#########################################
### sed-hook ###
#########################################

# data
    .data
    hit:      .asciiz 	"-"
    ship:     .asciiz 	"x"
    miss:     .asciiz 	"o"
    water:    .asciiz 	" "

    seed:			.word		082079114103
#    seed:           .word       082079114150		

    size: 			.word 		10

	board:			.asciiz		"oo  x xx o x  - o o  x  x  xxx x  -  oo   o - o -  oo o  o- xxxx  x -    o  -   xx    -     ooo - xx"
#	board:			.asciiz		"                                                                                                    "
#    board:			.asciiz		"    x xx   x  x      x  x  xxx x  x         x   x         x xxxx  x x       x   xx    x         x xx"

    delim:			.asciiz		": |"

#########################################

# main
.text
.globl main

main:
    # save s-regs
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    sw $s0, 4($sp)

    # try to get as many shots as possible
    loop:
    # print lf
    li $a0, 10
    li $v0, 11
    syscall

    # shot_cannon
    jal shoot_cannon
    move $s0, $v1

    # print row
    move $a0, $v0
    li $v0, 11
    syscall

    # print col
    move $a0, $s0
    li $v0, 11
    syscall

    # print delimiter
    la $a0, delim
    li $v0, 4
    syscall

    # print board
    la $a0, board
    li $v0, 4
    syscall

    # print end delimiter
    li $a0, 124
    li $v0, 11
    syscall

    # check if possible shot couldn't be found
    li $t0, 0x2D
    bne $s0, $t0, loop

    # print lf
    li $a0, 10
    li $v0, 11
    syscall

    # restore regs
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    addi $sp, $sp, 8

    jr $ra

#########################################
