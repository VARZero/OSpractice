#pragma once

void init_intdesc();
void idt_ignore();
void idt_timer();
void idt_keyboard();
void idt_SystemCall();

unsigned char keytext(unsigned char);

struct IDT{
    unsigned short offsetl;
    unsigned short selector;
    unsigned short type;
    unsigned short offseth;
} __attribute__((packed));

struct IDTR
{
    unsigned short size;
    unsigned int addr;
} __attribute__((packed));
