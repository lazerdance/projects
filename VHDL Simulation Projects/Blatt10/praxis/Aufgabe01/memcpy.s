
.data
	src0:	.word 15
	src1:	.word 255
	src2:	.word 4095
	src3:	.word 65535
	dst0:	.word 0
	dst1:	.word 0
	dst2:	.word 0
	dst3:	.word 0
	eop:    .word 1
	
	
	

.text
.globl main

	main:	
		lw $t0, src0
		lw $t1, src1
		lw $t2, src2
		lw $t3, src3

		sw $t0, dst0
		sw $t1, dst1
		sw $t2, dst2
		sw $t3, dst3
		
		lw $v0, eop     # signal end of program
