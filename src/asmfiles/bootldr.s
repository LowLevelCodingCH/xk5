[org 0x7c00]                         
mov [BOOT_DISK], dl                        
KERNEL_SIZE equ 50
msga: db 0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,"LOADING KERNEL-1... (nxkernel.o)",0
msgb: db "NXBOOTLOADER",0xa,0x0
printB:
    mov ah, 0x0e
    mov al, [msgb + bx]
    int 0x10
    cmp al, 0
    je kbi
    inc bx
    jmp printB                

mov ah, 0x0e

kbi:
    mov ah, 0
    int 0x16
    mov ah, 0x0e
    int 0x10
    pusha
    mov al, ' '
    int 0x10
    popa
    cmp al, '1'
    je printA
    cmp al, '2'
    je printB ; other kernels
    cmp al, '3'
    je exit
    cmp al, '4'
    je printB
    jmp kbi

printA:
    KERNEL_LOCATION equ 0x1000 
    mov ah, 0x0e
    mov al, [msga + bx]
    int 0x10
    cmp al, 0
    je cont
    inc bx
    jmp printA

cont:
    mov cx, 0x0F
    mov dx, 0x4240

    mov ah, 0x86
    int 0x15
xor ax, ax                          
mov es, ax
mov ds, ax
mov bp, 0x8000
mov sp, bp

mov bx, KERNEL_LOCATION

mov ah, 0x02
mov al, KERNEL_SIZE 
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DISK]
int 0x13                ; no error management, do your homework!

                                    
mov ah, 0x0
mov al, 0x3
int 0x10                ; text mode


CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

jmp $
                                    
BOOT_DISK: db 0

GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start


[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION

                                     
exit:
    hlt
    jmp exit
times 510-($-$$) db 0              
dw 0xaa55



;;; STOLE FROM https://github.com/JoshuaFarmer/OSA86/blob/main/src/boot.asm
;;
;;[org 0x7C00]
;;[bits 16]
;;
;;KERNEL_SIZE equ 50
;;
;;jmp _start
;;
;;; We have to start somewhere, we use _start out of convention.
;;_start:
;;	push dx
;;	; We don't want to be disturbed.
;;	cli
;;	xor ax, ax
;;	mov ss, ax
;;	mov ds, ax
;;	mov es, ax
;;
;;	mov sp, 0x7C00
;;	sti	 
;;
;;	pusha
;;	mov ah, 0x3
;;	int 10h
;;	popa
;;	
;;	; Hide cursor
;;	mov ah, 0x01
;;	mov ch, 0x20
;;	mov cl, 0x1f
;;	int 0x10
;;
;;	; Now we're going to load data from disk.
;;	; No filesystem yet.
;;	mov ah, 2
;;	mov al, KERNEL_SIZE
;;	mov cx, 2
;;	; Some bad BIOS'es don't pass the disk number.
;;	cmp dl, 0xFF
;;	je from_floppy
;;	cmp dl, 0x80
;;	jge from_disk
;;	mov dl, 0x80
;;	jmp from_disk
;;	; Floppies are unreliable, so we have to try a few times.
;;from_floppy:
;;	mov bp, 5
;;	jmp from_disk
;;check_if_repeat:
;;	sub bp, 1
;;	cmp bp, 0
;;	je spin
;;from_disk: 
;;	mov bx, 0x1000
;;	mov es, bx
;;	xor bx, bx
;;	int 0x13
;;	cmp dl, 0xFF
;;	je check_for_errors
;;check_for_errors:
;;	jnc done_with_disk
;;	cmp ah, 0
;;	je done_with_disk
;;	jmp spin
;;
;;done_with_disk:
;;switch32:
;;	lgdt [gdtinfo]
;;
;;	cli
;;	mov eax, cr0
;;	or  eax, 1
;;	mov cr0, eax
;;	jmp clear_prefetch
;;	nop
;;	nop
;;clear_prefetch:
;;	mov ax, 0x10
;;	mov ds, ax
;;	mov ss, ax 
;;	mov esp, 0xD00000
;;	mov ax, 0x8
;;	mov es, ax
;;	mov fs, ax
;;	mov gs, ax
;;
;;	db 0x66
;;	db 0xEA
;;	dd Really32
;;	dw 0x0008
;;
;;[bits 32]
;;
;;Really32:
;;        mov eax, cr0
;;        test eax, 1
;;        jz bruh_fail
;;	call 0x10000
;;.shutdown:
;;	jmp $
;;
;;[bits 16]
;;
;;bruh_fail:
;;        mov ah, 0x0E
;;        mov al, 'b'
;;        int 0x10
;;        mov al, 'r'
;;        int 0x10
;;        mov al, 'u'
;;        int 0x10
;;        mov al, 'h'
;;        int 0x10
;;        jmp $
;;
;;spin:
;;	hlt
;;inf:
;;	jmp inf
;;
;;gdtinfo:
;;	dw gdt_end - gdt - 1 ; Limit (16 bits), subtract 1 because the limit is inclusive
;;	dd gdt			   ; Base (32 bits)
;;
;;gdt:
;;	gdt_null:
;;		dd 0 ; Null descriptor
;;		dd 0
;;	gdt_code:
;;		dw 0xFFFF ; Limit (16 bits)
;;		dw 0	  ; Base (16 bits)
;;		db 0	  ; Base (8 bits)
;;		db 10011010b ; Access byte
;;		db 11001111b ; Granularity
;;		db 0	  ; Base (8 bits)
;;	gdt_data:
;;		dw 0xFFFF ; Limit (16 bits)
;;		dw 0	  ; Base (16 bits)
;;		db 0	  ; Base (8 bits)
;;		db 10010010b ; Access byte
;;		db 11001111b ; Granularity
;;		db 0	  ; Base (8 bits)
;;gdt_end:
;;
;;times 510-($ - $$) db 0
;;dw 0xAA55
