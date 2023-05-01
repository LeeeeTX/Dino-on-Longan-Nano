.section .text
.globl Render_Setting
.type Render_Setting, @function

Render_Setting:

  addi sp, sp, -4
  sw ra, 0(sp)
  # NOTE: There's no point storing ra and allocate on stack since the function is a dead loop, only way to exit is via exception traps.

  # LCD_ShowString(54, 16, (u8 *)("Setting"), WHITE);
  li a0, 54 # x0
  li a1, 16 # y0
  la a2, title # string
  la a3, WHITE # color
  lw a3, 0(a3) # load it into a3
  jal LCD_ShowString # call the function

  li a0, 28 # x0
  li a1, 40 # y0
  la a2, left # string
  la a3, WHITE # color
  lw a3, 0(a3) # load it into a3
  jal LCD_ShowString # call the function


  li a0, 98 # x0
  li a1, 40 # y0
  la a2, right # string
  la a3, WHITE # color
  lw a3, 0(a3) # load it into a3
  jal LCD_ShowString # call the function

  lw ra, 0(sp)
  addi sp, sp, 4
  ret 

.data
WHITE: .word 0xFFFF # the color white
BLACK: .word 0x0000 # the color black
title: .asciz "Setting" # title of the game
left: .asciz "Easy" # left option
right: .asciz "Hard" # right option
