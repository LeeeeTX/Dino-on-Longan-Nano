.section .text
.globl Render_Hard
.type Render_Hard, @function
Render_Hard:
	# store ra
	addi sp, sp, -4
	sw	ra, 0(sp)

	li a0, 98 # x1
	li a1, 58 # yr
	li a2, 133 # x2
	li a3, 58 # y2
	la a4, WHITE
	lw a4, 0(a4) # load the color into a3
	jal LCD_DrawLine 

	li a0, 500
	jal delay_1ms # wait for 0.5 sec

	la a0, BLACK
	lw a0, 0(a0)
	jal LCD_Clear # clear the LCD

	li a0, 28 # x1
	li a1, 32 # y1
	la a2, note # note to display
	la a3, WHITE # color
	lw a3, 0(a3)
	jal LCD_ShowString

	li a0, 1000
	jal delay_1ms # wait for 1s

	la a0, BLACK
	lw a0, 0(a0)
	jal LCD_Clear # clear the LCD

	jal Start_menu

	# return
	lw ra, 0(sp)
	addi sp, sp, 4
	ret

.data
# Color
WHITE:	.word 0xFFFF
BLACK:	.word 0x0000
note:	.asciz "Hard mode now!"
