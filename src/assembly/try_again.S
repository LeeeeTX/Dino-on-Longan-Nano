.section .text
.globl Display_try_again
.type Display_try_again, @function
Display_try_again:
	# store ra
	addi sp, sp, -4
	sw	ra, 0(sp)
    # LCD_ShowString(64,  0, (u8 *)("Rank"), WHITE);
	li a0, 64		# x0
	li a1, 0 		# y0
	la a2, rank 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString
    # LCD_ShowString(0, 26, (u8 *)("You are not in rank"), WHITE);
	li a0, 0		# x0
	li a1, 26 		# y0
	la a2, str0 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString
    # LCD_ShowString(42, 42, (u8 *)("Try again!"), WHITE);
	li a0, 42		# x0
	li a1, 42		# y0
	la a2, str1 	# String
	la a3, WHITE 	# color
	lw a3, 0(a3)
	jal LCD_ShowString
	# return
	lw ra, 0(sp)
	addi sp, sp, 4
	ret

.data
# Color
WHITE:	.word 0xFFFF
rank:	.asciz "Rank"	# Strings
str0:	.asciz "You are not in rank"
str1:	.asciz "Try again!"
