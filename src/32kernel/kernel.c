/*
##########################################################################
KingKernel의 메인 부분
타이머, 스케줄러, 멀티스레딩, 멀티프로세싱, 메모리 보호 실행
##########################################################################
*/
#include "types.h"
#include "interrupt.h"

BOOL DEBUG = 1;
int memorysizeCheck();
DWORD cpuid_Func(DWORD EAXmode);
void PortIn();
void PortOut();

void main(){
    //메인 함수
    DEBUG_draw_text(0,0,"KING_KERNEL v0.0.0 by VAR Zero");
    int ramsize = memorysizeCheck();
    if (ramsize < 64){
        DEBUG_draw_text(0,1,"ramsize is low");
        while (1);
    }
    DEBUG_draw_text(0,4,"====interrupts..====");
    init_intdesc();
    __asm__ __volatile__(
        "int 0x80;"
    );
}

void DEBUG_draw_text(char xx, char yy, char* text){
    //디버깅용 텍스트 함수
    if(DEBUG){}
    char *videoMem = (char*) 0xB8000+(xx*2)+(80*yy*2);
    int i;
    for (i=0;text[i]!=0;i++){
        *videoMem++ = text[i];
        *videoMem++ = 0x07;
    }
}
int memorysizeCheck(){
    DWORD* CurrRAM;
    CurrRAM = ( DWORD* ) 0x100000;
    int RamKB = 0;
    while(1){
        *CurrRAM = 0x12345678;
        if( *CurrRAM != 0x12345678 ){
            return RamKB;
        }
        CurrRAM += (0x100000/4);
        RamKB++;
    }
    return RamKB;
}
