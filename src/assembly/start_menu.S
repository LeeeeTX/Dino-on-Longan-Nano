.section .text
.globl Start_menu
.type Start_menu, @function

Start_menu:

  # addi sp, sp, -4
  # sw ra, 0(sp)
  # NOTE: There's no point storing ra and allocate on stack since the function is a dead loop, only way to exit is via exception traps.
  # LCD_ShowString(64, 16, (u8 *)("Dino!"), WHITE);
  li a0, 64 # x0
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


  li a0, 88 # x0
  li a1, 40 # y0
  la a2, right # string
  la a3, WHITE # color
  lw a3, 0(a3) # load it into a3
  jal LCD_ShowString # call the function

loop: # the dead event loop
  li a0, 0
  jal Get_Button
  beq a0, x0, exit_if1 # if button 0 is pressed

  li a0, 28 # x0
  li a1, 58 # y0
  li a2, 68 # x1
  li a3, 58 # y1
  la a4, WHITE # color
  lw a4, 0(a4) # load it into a4
  jal LCD_DrawLine

  li a0, 500
  jal delay_1ms

  la a0, BLACK
  lw a0, 0(a0)
  jal LCD_Clear

  jal Start
  j loop

exit_if1:
  li a0, 1
  jal Get_Button
  beq a0, x0, exit_if2 # if button 1 is pressed

  li a0, 88 # x0
  li a1, 58 # y0
  li a2, 145 # x1
  li a3, 58 # y1
  la a4, WHITE # color
  lw a4, 0(a4) # load it into a4
  jal LCD_DrawLine

  li a0, 500
  jal delay_1ms

  la a0, BLACK
  lw a0, 0(a0)
  jal LCD_Clear

  jal Setting
exit_if2:

  j loop 
  
.data
WHITE: .word 0xFFFF # the color white
BLACK: .word 0x0000 # the color black
title: .asciz "Dino!" # title of the game
left: .asciz "Start" # left option
right: .asciz "Setting" # right option
