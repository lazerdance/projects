
.data	
	nSamples:	.word 4
	minValue:	.word 0
	maxValue:	.word 255
	const1:		.word 1
	const2:		.word 2
	const3:		.word 3
	sample0:	.word 15
	sample1:	.word -4
	sample2:	.word 221
	sample3:	.word 317
	eop:		.word 1

.text
.globl main

	main:
		lw $t0, nSamples
		and $t1, $zero, $zero
		lw $t8, minValue
		lw $t9, maxValue
		lw $t5, const1
		lw $t6, const2
		lw $t7, const3
		
		
	loop:
		beq $t1, $t0, end
		
		beq $t1, $zero, loadFirst
		beq $t1, $t5, loadSecond
		beq $t1, $t6, loadThird
		beq $t1, $t7, loadFourth
		
		loadFirst:
			lw $t2, sample0
			beq $zero, $zero clip
		
		loadSecond:
			lw $t2, sample1
			beq $zero, $zero clip
		
		loadThird:
			lw $t2, sample2
			beq $zero, $zero clip
		
		loadFourth:
			lw $t2, sample3
			beq $zero, $zero clip
		
		clip:
			slt $t3, $t2, $t8
			beq $t3, $t5, clipMin
			slt $t3, $t9, $t2
			beq $t3, $t5, clipMax
			beq $zero, $zero, incLoopCounter
			
			clipMin:
				add $t2, $t8, $zero
				beq $zero, $zero, storeSample
				
			clipMax:
				add $t2, $t9, $zero
				beq $zero, $zero, storeSample
				
		storeSample:
		
			beq $t1, $zero, storeFirst
			beq $t1, $t5, storeSecond
			beq $t1, $t6, storeThird
			beq $t1, $t7, storeFourth
			
			storeFirst:
				sw $t2, sample0
				beq $zero, $zero incLoopCounter
	
			storeSecond:
				sw $t2, sample1
				beq $zero, $zero incLoopCounter
	
			storeThird:
				sw $t2, sample2
				beq $zero, $zero incLoopCounter
	
			storeFourth:
				sw $t2, sample3
				beq $zero, $zero incLoopCounter
		
		incLoopCounter:
			add $t1, $t1, $t5
			beq $zero, $zero, loop
	
	end:
		lw $v0, eop
