; each file has an entry in FST (file segment table) and each block is 512 bytes or 1 sector. a file can have up to 16 chars of length (with extension) and up to 16 blocks (for now)

fst:
	file1: ;file
		blocks1: db 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		sizes1:  db 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		name1:   db "XK$USERS", 0, 0, 0, 0, 0, 0, 0, 0

times 4096-($-$$) db 0 ; fst can max be 4096 bytes long.. 8 sectors. 
