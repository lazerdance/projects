# v171215

#########################################
#  read_email
#########################################
# $a0: buffer
# $v0: number of characters read

read_email:
    move $t0, $a0

    # read mail from disk
    li $v0, 13
    la $a0, input_file
    li $a1, 0
    li $a2, 0
    syscall

    # save fd
    move $t1, $v0

    # read to buffer
    li $v0, 14
    move $a0, $t1
    move $a1, $t0 # address of buffer
    li $a2, 4096
    syscall

    move $t0, $v0

    # close file
    li $v0, 16
    move $a0, $t1 # fd
    syscall

    move $v0, $t0

    jr $ra

#########################################
#  write_email
#########################################
# $a0: buffer
# $a1: number of characters to write
# $a2: truncate file

write_email:
    addi $sp, $sp, -16
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)

    move $s0, $a0
    move $s1, $a1

    # open file
    li $v0, 13
    la $a0, output_file

    bne $zero, $a2, write_email_trunc
    j write_email_notrunc
    write_email_trunc:
    li $a1, 0x241       # mode O_WRONLY | O_CREAT | O_TRUNC
    j write_email_else

    write_email_notrunc:
    li $a1, 0x441       # mode O_WRONLY | O_CREAT | O_APPEND

    write_email_else:
    li $a2, 0x1a4
    syscall             # fd in $v0

    move $s2, $v0       # save fd

    li $v0, 15          # write to file
    move $a0, $s2
    move $a1, $s0
    move $a2, $s1
    syscall

    # close file
    li $v0, 16
    move $a0, $s2
    syscall

    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    addi $sp, $sp, 16
    jr $ra


#########################################
#  Ausgabe
#########################################
# a0: buffer
# a1: buffer length
# a2: relative position of subject
# a3: spam flag

print_email:
    ### Register gemass Konventionen sichern
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    sw $s0, 4($sp)

    ### hier implementieren

    beqz $a3, end_schleife               # spamflag := '0' -> dont label the email with spamflag

    la $a0, email_buffer            # load buffer at a0
    add $a0, $a0, $a1               # Adress end = last index / index where last character is stored
    la $a3, spam_flag               # load spam_flag at a3
    add $s0, $zero, $a1             # s0 = 1080 -> counter for loop
    jal shift_schleife                   # call shift_schleife


        #shift_schleife: Alle Charachter 6 Stellen von links nach rechts verschieben wenn nötig bis zur Position des Subjektes 
        # wir fügen den Spamflag danach ein

    shift_schleife:                       # for-loop from (end-adress of buffer) to (position of subject, where spamflag should be pasted)
        blt $s0, $a2, spam_schleife       # for(s=1080; s>=292;)
        lb $t0, ($a0)                # load character at index s
        move $t3, $a0                # t3 = a0
        addi $t3, $a0, 7             # shift the character by the length of [SPAM] = 6
        sb $t0, ($t3)                # store the character at the shifted index
        sub $a0, $a0, 1              # a--
        sub $s0, $s0, 1              # s--

        j shift_schleife                  # wieder shift_schleife aufrufen


##spam_schleife: kopiert den spamflag speichert den, wo wir schon eine Leertaste erstellt haben oben

    spam_schleife:
        addi $a0, $a0, 1             # index of buffer ++
        lb $t0,($a3)                 # load character and save it in t0
        beqz $t0, end_schleife            # t0 = 0 -> End_schleife
        sb $t0, ($a0)                # character speichern
        addi $a3, $a3, 1             # index of spamflag (counter++)
        
        j spam_schleife                   # wieder spam_schleife aufrufen
       

    end_schleife:

    la $a0, email_buffer   # email_buffer auf a0 laden (Argument von write_email)
    li $a1, 1087           # Zahl der characters von email mit spam flag (Argument von write_email)
    # li $a2, 292          # position des Subjektes (Argument von write_email) schon in main gegeben

    jal write_email        # call write_email

    ### gesicherte Register wieder herstellen
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    addi $sp, $sp, 8
    jr $ra


#########################################
#

#
# data
#

.data

input_file: .asciiz "../email1"
output_file: .asciiz "output"
email_buffer: .space 4096

spam_flag: .asciiz "[SPAM] "
spam_flag_length: .word 7

#
# main
#

.text
.globl main

main:
    # Register sichern
    addi $sp, $sp, -12
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)

    # E-Mail einlesen
    la $a0, email_buffer
    jal read_email

    la $a0, email_buffer

    # Groesse
    move $a1, $v0

    # Position des Subjekts
    li $a2, 292

    # Spam
    li $a3, 1

    # E-Mail ausgeben
    jal print_email

    # Register wieder herstellen
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addi $sp, $sp, 12
    jr $ra

#
# end main
#
