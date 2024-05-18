section .text
  [bits 32]
  [extern kernel]
  call kernel
  jmp $
