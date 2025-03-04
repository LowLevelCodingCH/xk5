header_name: db "XKFS"
kernel_version_lower: db 0x06 ; k6
kernel_version_upper: db 0x0A ; x
; in whole: xk6

disk_label: db "A"

max_name_size: db 0x10 ; with extension. whole name
block_size: dw 512
max_file_size_in_blocks: db 8 ; eight blocks

end_signature: db " ."

times 510-($-$$) db 0
dw 0xAA55
