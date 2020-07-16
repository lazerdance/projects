#v20180326

#
# Prints string to stdout with length $a1
#
# $a0 -> string
# $a1 -> length
#

print:
    # save s-Regs
    addi $sp, $sp, -8
    sw $s0, 0($sp)
    sw $s1, 4($sp)
    
    # save params
    move $s0, $a0
    move $s1, $a1
    
    # alloc buffer (with double word alignment)
    addi $s1, $s1, 1
    andi $t0, $s1, 7
    li $t1, 8
    sub $t1, $t1, $t0
    andi $t1, $t1, 7
    add $s1, $s1, $t1
    sub $sp, $sp, $s1
    
    # copy string
    li $t0, 0
    loop_copy:
    bge $t0, $a1, end_copy
    
    add $t1, $s0, $t0
    lb $t2, 0($t1)
    add $t3, $sp, $t0
    sb $t2, 0($t3)
    
    addi $t0, $t0, 1
    j loop_copy
    
    end_copy:
    # null terminated
    add $t3, $sp, $t0
    sb $zero, 0($t3)
    
    # print buffer
    move $a0, $sp
    li $v0, 4
    syscall
    
    # free buffer
    add $sp, $sp, $s1
    
    # restore s-Regs
    lw $s0, 0($sp)
    lw $s1, 4($sp)
    addi $sp, $sp, 8
    jr $ra

#
# Prints a single linefeed (\n) to stdout
#

print_lf:
    li $a0, 10
    li $v0, 11
    syscall
    
    jr $ra

#########################################
### sed-hook ###
#########################################

#
# Writes the board nicely line by line
#

write_board:
    ### Register gemass Konventionen sichern
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    ## Board und Size in Argumente laden
    la $a0, board
    lw $a1, size
    li $t9, 0               # index t9=0 
    move $s0, $a0           ## board wird in s0 uebertragen zum print

    ## diese Schleife gibt inkrementiert index und Board index und gibt die nächste 10 Zahlen aus
    print_loop:  
    add $a0, $s0, $t9           ## board = board buffer von erste Stelle + Index position
    beq $t9, 100, exit          ## Index = 100 -> End Schleife
    jal print                   ## Print Function aktivieren
    jal print_lf                ## Line Feed Function aktivieren
    addi $t9, $t9, 10           ## increment index
    j print_loop

    exit:
    ### Register wieder herstellen
    lw $ra, 0($sp)
    addi $sp, $sp, 4

    jr $ra
    
#########################################
### sed-hook ###    
#########################################

# data
.data
size:
.word 		10

#	board:			.asciiz		"oo  x xx o x  - o o  x  x  xxx x  -  oo   o - o -  oo o  o- xxxx  x -    o  -   xx    -     ooo - xx"
#	board:			.asciiz		"                                                                                                    "
board:
.asciiz		"    x xx   x  x      x  x  xxx x  x         x   x         x xxxx  x x       x   xx    x         x xx"

#########################################

# main
.text
.globl main

main:
    # save regs
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # call function
    jal write_board
    
    # restore regs
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    
    jr $ra

#########################################
