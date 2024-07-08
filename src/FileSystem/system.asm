    ;; Structure:
;;; first 4 bytes: signature
;;; next  1 byte:  null
;;; next  1 byte:  max filename size
;;; next  1 byte:  max dirname size
;;; next  1 byte:  max nested directory size
;;; next  1 byte:  kernel version
;;; next  4 bytes: disk begin location (mostly 0,0,0,0)
;;; next  4 bytes: max file size (in 32 bit number, big endian, unsigned, ammount of bytes)
;;; next  1 byte:  null
;;; next  bytes:   files etc. (no dir support yet.)
metadata:          db "XKFS",0x00,0x0B,0x08,0x02,0x05,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00
    ;; File:
;;; 7 first chars = file name
;;; next 1 char   = .
;;; next 3 chars  = extension
;;; next 4 chars  = file size (int)
;;; next bytes    = file contents (size of the file size)
;;; next 1 byte   = null byte

db "hello00", '.', "txt", 0x00, 0x00, 0x00, 0x0d, "Hello, world!", 0x00
db "hello01", '.', "txt", 0x00, 0x00, 0x00, 0x0a, "1234567890", 0x00
