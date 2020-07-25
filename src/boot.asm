[ORG 0x7c00]
[BITS 16]

EntryLoad:
    mov ax, 0x0800
    mov es, ax
    mov bx, 0

    mov ah, 2
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x00

    int 0x13
    jc EntryLoad

jmp 0x8000

times 510-($-$$) db 0x00
db 0x55
db 0xAA