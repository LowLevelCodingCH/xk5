section .text
    [bits 32]
    [extern nitrix_kernel]
    call nitrix_kernel
    jmp $
