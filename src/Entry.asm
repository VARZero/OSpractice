[ORG 0x8000]
[BITS 16]

;32비트 설정, 메인 커널로 점프(링커로 묶인)

SECTION .text
;ClEAR SCREEN
mov ax, 0xB800
mov es, ax

mov ax, 0
mov bx, 0
mov cx, 80*25*2
CLRSCN:
    mov [es:bx], ax
    add bx, 2
    loop CLRSCN 

KernelLoad:
    mov ax, 0x1000
    mov es, ax
    mov bx, 0

    mov ah, 2
    mov al, 30
    mov ch, 0
    mov cl, 3
    mov dh, 0
    mov dl, 0x00

    int 0x13
    jc KernelLoad

    mov dx, 0x3F2 ;플로피 모터 끄기
    xor al, al
    out dx, al
    
    cli

mov ax, 0x2401
int 0x15

jc a20Err
jmp a20Suc

a20Err:
    ;시스템 콜로 a20 실행
    in al, 0x92
    or al, 0x02
    and al, 0xFE
    out 0x92, al

a20Suc:
    ;커널 실행

    ;이 부분은 인터럽트 처리를 위한 부분이다. 이해가 잘 안되는 부분
    mov al, 0x11 ;PIC 초기화를 위해 al레지스터에 0x11을 넣는 부분이다.
    out 0x20, al ;out 명령을 통해 마스터PIC를 초기화한다.
    dw 0x00eb, 0x00eb ;jmp $+2, $+2 도데체 왜?
    out 0xA0, al ;슬레이브 PIC 초기화
    dw 0x00eb, 0x00eb

    mov al, 0x20 ;마스터PIC 시작점
    out 0x21, al ;슬레이브PIC에 out?
    dw 0x00eb, 0x00eb ;여전히 왜 하는 지 모르겠음
    mov al, 0x28 ; ..? 설명이 필요하다
    out 0xA1, al ; ..?
    dw 0x00eb, 0x00eb

    ;대충 마스터PIC와 슬레이브PIC연결한다는 내용
    mov al, 0x04
    out 0x21, al
    dw 0x00eb, 0x00eb
    mov al, 0x02
    out 0xA1, al
    dw 0x00eb, 0x00eb

    ;이젠 하다하다 8086모드를 쓴다..
    mov al, 0x01
    out 0x21, al
    dw 0x00eb, 0x00eb
    out 0xA1, al
    dw 0x00eb, 0x00eb

    mov al, 0xff ;슬레이브 PIC 다 막아둠
    out 0xA1, al ;위의 연장선
    dw 0x00eb, 0x00eb
    mov al, 0xFB ; (대충 마스터 PIC는 IRQ2번 빼고 다 막는다는 소리)
    out 0x21, al ;위의 연장선

    lgdt [gdtr]
    mov eax, 0x4000003B
    mov cr0, eax
    jmp dword 0x08:( PROTECTEDMODE - $$ + 0x8000 )

[BITS 32]
PROTECTEDMODE:
    ;32비트 보호모드 진입
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ss, ax
    mov esp, 0xFFFE
    mov ebp, 0xFFFF

    jmp dword 0x08: 0x10000

;영역 정리?
align 8, db 0

dw 0x0000

;GDT 부분
gdtr:
    dw gdtend - gdt - 1
    dd ( gdt - $$ + 0x8000 )
gdt:
    NULLDSC:
        dw 0x0000
        dw 0x0000
        db 0x00
        db 0x00
        db 0x00
        db 0x00

    CODEDSC:
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0x9A
        db 0xCF
        db 0x00

    DATADSC:
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0x92
        db 0xCF
        db 0x00

gdtend:

times 512-($-$$) db 0x00