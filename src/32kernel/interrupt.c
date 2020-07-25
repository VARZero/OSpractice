#include "types.h"
#include "interrupt.h"
#include "keyboard/keyboard.h"

//GDT 설정과 비슷하다 합니다.

struct IDT inttable[3];
struct IDTR idtr = { 256 * 8 - 1, 0 };

unsigned char keyt[2] = {'A',0};
unsigned char keybuf;

void init_intdesc() {
    int i, j;
    unsigned int ptr;
    unsigned short *isr;   

    { // 0x00 : isr_ignort 
        ptr = (unsigned int)idt_ignore;
        inttable[0].selector = (unsigned short)0x08;
        inttable[0].type = (unsigned short)0x8E00;
        inttable[0].offsetl = (unsigned short)(ptr & 0xFFFF);
        inttable[0].offseth = (unsigned short)(ptr >> 16);
    }

    { // 0x01 : isr_timer 
        ptr = (unsigned int)idt_timer;
        inttable[1].selector = (unsigned short)0x08;
        inttable[1].type = (unsigned short)0x8E00;
        inttable[1].offsetl = (unsigned short)(ptr & 0xFFFF);
        inttable[1].offseth = (unsigned short)(ptr >> 16);
    }

    { // 0x02 : isr_keyboard 
        ptr = (unsigned int)idt_keyboard;
        inttable[2].selector = (unsigned short)0x08;
        inttable[2].type = (unsigned short)0x8E00;
        inttable[2].offsetl = (unsigned short)(ptr & 0xFFFF);
        inttable[2].offseth = (unsigned short)(ptr >> 16);
    }
    
    { // 0x80 : isr_SystemCall 
        ptr = (unsigned int)idt_SystemCall;
        inttable[3].selector = (unsigned short)0x08;
        inttable[3].type = (unsigned short)0x8E00;
        inttable[3].offsetl = (unsigned short)(ptr & 0xFFFF);
        inttable[3].offseth = (unsigned short)(ptr >> 16);
    }

    for(i = 0;i < 256;i++)
    {
        isr = (unsigned short*)(0x0 + i * 8);
        *isr = inttable[0].offsetl;
        *(isr + 1) = inttable[0].selector;
        *(isr + 2) = inttable[0].type;
        *(isr + 3) = inttable[0].offseth;
    }
    
    {   //타이머 ISR
        isr = (unsigned short*)(0x0 + 8 * 0x20);
        *isr = inttable[1].offsetl;
        *(isr + 1) = inttable[1].selector;
        *(isr + 2) = inttable[1].type;
        *(isr + 3) = inttable[1].offseth;
    }
    {   //키보드 ISR
        isr = (unsigned short*)(0x0 + 8 * 0x21);
        *isr = inttable[2].offsetl;
        *(isr + 1) = inttable[2].selector;
        *(isr + 2) = inttable[2].type;
        *(isr + 3) = inttable[2].offseth;
    }
    {   //시스템콜 ISR
        isr = (unsigned short*)(0x0 + 8 * 0x80);
        *isr = inttable[3].offsetl;
        *(isr + 1) = inttable[3].selector;
        *(isr + 2) = inttable[3].type;
        *(isr + 3) = inttable[3].offseth;
    }
    __asm__ __volatile__(
        "mov al, 0xAE;"
        "out 0x64, al;"
    );

    __asm__ __volatile__("mov eax, %0"::"r"(&idtr));
    __asm__ __volatile__("lidt [eax]");
    __asm__ __volatile__("mov al, 0xFC");
    __asm__ __volatile__("out 0x21, al");
    __asm__ __volatile__("sti");

    return;
}
void idt_ignore() {
    __asm__ __volatile__
    (
        "push gs;"
        "push fs;"
        "push es;"
        "push ds;"
        "pushad;"
        "pushfd;"
        "mov al, 0x20;"
        "out 0x20, al;"
    );
    DEBUG_draw_text(0, 5, "ignore interrupt..");
    __asm__ __volatile__
    (
        "popfd;"
        "popad;"
        "pop ds;"
        "pop es;"
        "pop fs;"
        "pop gs;"
        "leave;"
        "nop;"
        "iretd;"
    );
}
void idt_timer() {
    __asm__ __volatile__
    (
        "push gs;"
        "push fs;"
        "push es;"
        "push ds;"
        "pushad;"
        "pushfd;"
        "mov al, 0x20;"
        "out 0x20, al;"
    );
    DEBUG_draw_text(0, 6, keyt);
    keyt[0]++;
    __asm__ __volatile__
    (
        "popfd;"
        "popad;"
        "pop ds;"
        "pop es;"
        "pop fs;"
        "pop gs;"
        "leave;"
        "nop;"
        "iretd;"
    );
}
void idt_keyboard() {
    __asm__ __volatile__
    (
        "push gs;"
        "push fs;"
        "push es;"
        "push ds;"
        "pushad;"
        "pushfd;"
        "xor al, al;"
        "in al, 0x60;"
    );
    __asm__ __volatile__("mov %0, al":"=r"(keybuf));
    char textOne = KeyMapping(keybuf);
    DEBUG_draw_text(0, 7, &textOne);
    __asm__ __volatile__(
        "mov al, 0x20;"
        "out 0x20, al;"
    );
    __asm__ __volatile__
    (
        "popfd;"
        "popad;"
        "pop ds;"
        "pop es;"
        "pop fs;"
        "pop gs;"
        "leave;"
        "nop;"
        "iretd;"
    );
}
void idt_SystemCall() {
    __asm__ __volatile__
    (
        "push gs;"
        "push fs;"
        "push es;"
        "push ds;"
        "pushad;"
        "pushfd;"
        "mov al, 0x20;"
        "out 0x20, al;"
    );
    DEBUG_draw_text(0, 10, "System Call..");
    __asm__ __volatile__
    (
        "popfd;"
        "popad;"
        "pop ds;"
        "pop es;"
        "pop fs;"
        "pop gs;"
        "leave;"
        "nop;"
        "iretd;"
    );
}