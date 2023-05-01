.section .text
.globl Display_rank
.type Display_rank, @function
Display_rank:
	# store ra
	addi sp, sp, -4
	sw	ra, 0(sp)
	# Store the parameter
	mv	t0, a0
	mv	t1, a1
	mv	t2, a2
	mv	t3, a3
	# LCD_ShowString(64,  0, (u8 *)("Rank"), WHITE);
	li a0, 64		# x0
	li a1, 0 		# y0
	la a2, rank 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString	
	# LCD_ShowString(32, 16, (u8 *)("#1: "), WHITE);
	li a0, 32		# x0
	li a1, 16 		# y0
	la a2, num1 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString
    # LCD_ShowNum(48, 16, scores[0], 10, WHITE);
	li a0, 48		# x0
	li a1, 16 		# y0
	mv a2, t0 		# score
	li a3, 10
	la a4, WHITE 	# color
	lw a4, 0(a4)
	jal LCD_ShowNum
    # LCD_ShowString(32, 32, (u8 *)("#2: "), WHITE);
	li a0, 32		# x0
	li a1, 32 		# y0
	la a2, num2 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString
    # LCD_ShowNum(48, 32, scores[1], 10, WHITE);
	li a0, 48		# x0
	li a1, 32 		# y0
	mv a2, t1 		# score
	li a3, 10
	la a4, WHITE 	# color
	lw a4, 0(a4)
	jal LCD_ShowNum
    # LCD_ShowString(32, 48, (u8 *)("#3: "), WHITE);
	li a0, 32		# x0
	li a1, 48 		# y0
	la a2, num3 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString
    # LCD_ShowNum(48, 48, scores[2], 10, WHITE);
	li a0, 48		# x0
	li a1, 48 		# y0
	mv a2, t2 		# score
	li a3, 10
	la a4, WHITE 	# color
	lw a4, 0(a4)
	jal LCD_ShowNum
    # LCD_ShowString(32, 64, (u8 *)("#4: "), WHITE);
	li a0, 32		# x0
	li a1, 64 		# y0
	la a2, num4 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString
    # LCD_ShowNum(48, 64, scores[3], 10, WHITE);
	li a0, 48		# x0
	li a1, 64 		# y0
	mv a2, t3 		# score
	li a3, 10
	la a4, WHITE 	# color
	lw a4, 0(a4)
	jal LCD_ShowNum
	# return
	lw ra, 0(sp)
	addi sp, sp, 4
	ret

.data
# Color
WHITE:	.word 0xFFFF
rank:	.asciz "Rank"	# Strings
num1:	.asciz "#1: "
num2:	.asciz "#2: "
num3:	.asciz "#3: "
num4:	.asciz "#4: "