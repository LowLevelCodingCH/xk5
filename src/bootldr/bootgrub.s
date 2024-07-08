.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set DEADAS,   0xFA11DEAD
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16


stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
mov $0, %eax
push %eax
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	call nitrix_kernel
	cli
1:	hlt
	jmp 1b
.size _start, . - _start

.section .data
sig: .string "BOOTLDR_NITRIX_XK5_XKOS"
