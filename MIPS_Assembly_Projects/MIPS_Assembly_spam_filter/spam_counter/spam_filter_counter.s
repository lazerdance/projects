# v180105

#########################################
# find_str
#########################################
# $a0: haystack
# $a1: len of haystack
# $a2: needle
# $a3: len of needle
# $v0: relative position of needle, -1 if not found

find_str:
    # save $ra on stack
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    # save beginning of haystack
    move $t5, $a0
    # save len of needle
    move $t4, $a3

    # calc end address of haystick and needle
    add $a1, $a1, $a0
    add $a3, $a3, $a2

haystick_loop:
    bge $a0, $a1, haystick_loop_end

    move $t6, $a0
    move $t7, $a2
needle_loop:
    # load char from haystick
    lbu $t0, 0($t6)
    # load char from needle
    lbu $t1, 0($t7)

    bne $t0, $t1, needle_loop_end

    addi $t6, $t6, 1
    addi $t7, $t7, 1

    # reached end of needle
    bge $t7, $a3, found_str

    # reached end of haystick
    bge $t6, $a1, found_nostr

    j needle_loop
needle_loop_end:

    addi $a0, $a0, 1
    j haystick_loop
haystick_loop_end:

found_nostr:
    # prepare registers so found_str: produces -1
    li $t6, 0
    li $t5, 0
    li $t4, 1

found_str:
    sub $v0, $t6, $t5
    sub $v0, $v0, $t4


    # restore $ra from stack
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

#########################################
#  Spamfilter
#########################################
# $v0: Spamscore

spamfilter:
    ### Register gemaess Registerkonventionen sichern
    addi $sp, $sp, -28
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    sw $s2, 12($sp)
    sw $s3, 16($sp)
    sw $s4, 20($sp)
    sw $s5, 24($sp)
    
    ### Badwords liegen im Puffer badwords_buffer
    ### Der Text der E-Mail liegt im Puffer email_buffer
    move $s5, $zero
    
    ### Schleife ueber Bad words (wort1,gewicht1,wort2,gewicht2,...)
    spam_start:
    ## load the value of the comma in $t2
    li $t2, 44
    ## load the value of the badword in $t8
    lw $t8, badwords_size
    ## if badword value < 0 then end the Spam-load Schleife
    bgt $s5, $t8, end_spam
    la $s0, badwords_buffer
    add $s0, $s0, $s5
    move $s2, $zero

    ### lese ein Wort
    badwords_suchen:
	beq $t1, $t2, end_bw_suchen
    ## load character from badword
	lbu $t1, 0($s0)
	addi $s0, $s0, 1
	addi $s2, $s2, 1
	j badwords_suchen           # loop

    end_bw_suchen:
    ## add the badword character value to s5
    add $s5, $s5, $s2
    ## load badword value in t0
    la $t0, badwords_buffer
    ## add charachter value to badword value
    add $t0, $t0, $s5
    move $s4, $zero
    li $t3, 10
    
    find_number:
        lb $t1, 0($t0)    
        addi $t0, $t0, 1	
        addi $s5, $s5, 1
        ## counting the comma value        
        li $t2, 44
        ## if the loop reaches a comma then end the loop
        beq $t1, $t2, end_find_number
        ## load the value of badwords
        lw $t2, badwords_size
        ## badword value < 1 then end the loop
        bgt $s5, $t2, end_find_number
        mult $s4, $t3
        ## move the result for execution using low values command
        mflo $s4
        # Converting ascii to integer
        addi $t1, $t1-48
        # Updating the score
        add $s4, $s4, $t1
        
        j find_number           # loop


    end_find_number:
   
    sub $s0,$s0, $s2
    #Putting back $s2 to it's first value
    addi $s2, $s2, -1
    
    move $s1, $zero
    la $a0, email_buffer
    lw $a1, size

    search_loop:
      add $a2, $zero, $s0
      add $a3, $zero, $s2
      #Invoking find_str
      jal find_str
      ## move the result to a temporary register
      move $t8, $v0
      li $t1, -1
      lw $a1, size
      ## if t
      beq $t1, $t8, spam_start
      add $s3, $s3, $s4
      add $s1, $s1, $t8
      la $a0, email_buffer
      add $s1, $s1, $s2
      add $a0, $a0, $s1
      
      sub $a1, $a1, $s1
      j search_loop
    ## End-Schleife
    end_spam:
    add $v0, $zero, $s3

    ### Register wieder herstellen
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    lw $s2, 12($sp)
    lw $s3, 16($sp)
    lw $s4, 20($sp)
    lw $s5, 24($sp)
    addi $sp, $sp, 28
    jr $ra

#########################################
#

#
# data
#

.data

email_buffer: .asciiz "Hochverehrte Empfaenger,\n\nbei dieser E-Mail handelt es sich nicht um Spam sondern ich moechte Ihnen\nvielmehr ein lukratives Angebot machen: Mein entfernter Onkel hat mir mehr Geld\nhinterlassen als in meine Geldboerse passt. Ich muss Ihnen also etwas abgeben.\nVorher muss ich nur noch einen Spezialumschlag kaufen. Senden Sie mir noch\nheute BTC 1,000 per Western-Union und ich verspreche hoch und heilig Ihnen\nalsbald den gerechten Teil des Vermoegens zu vermachen.\n\nHochachtungsvoll\nAchim Mueller\nSekretaer fuer Vermoegensangelegenheiten\n"

size: .word 538

badwords_buffer: .asciiz "Spam,5,Geld,1,ROrg,0,lukrativ,3,Kohlrabi,10,Weihnachten,3,Onkel,7,Vermoegen,2,Brief,4,Lotto,3"
badwords_size: .word 93

spamscore_text: .asciiz "Der Spamscore betraegt: "

#
# main
#

.text
.globl main

main:
    # Register sichern
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    sw $s0, 4($sp)


    jal spamfilter
    move $s0, $v0


    li $v0, 4
    la $a0, spamscore_text
    syscall
    move $a0, $s0
    li $v0, 1
    syscall

    li $v0, 11
    li $a0, 10
    syscall


    # Register wieder herstellen
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    addi $sp, $sp, 8
    jr $ra

#
# end main
#
