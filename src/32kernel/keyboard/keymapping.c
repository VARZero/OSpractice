#include "keyboard.h"
#include "../types.h"

unsigned char KeyMapping(char keyboardKey){
    BOOL shiftOn = FALSE;
    unsigned char string;
    switch (keyboardKey)
    {
        // 숫자
        case 0x02:  string = '1';  break;
        case 0x03:  string = '2';  break;
        case 0x04:  string = '3';  break;
        case 0x05:  string = '4';  break;
        case 0x06:  string = '5';  break;
        case 0x07:  string = '6';  break;
        case 0x08:  string = '7';  break;
        case 0x09:  string = '8';  break;
        case 0x0A:  string = '9';  break;
        case 0x0B:  string = '0';  break;
        // 특수문자 일부
        case 0x0C:  string = '-';  break;
        case 0x0D:  string = '=';  break;
        case 0x0E:  string = 0x08;  break;
        case 0x0F:  string = 0x09;  break;
        // 쿼티 상단
        case 0x10:  string = 'q';  break;
        case 0x11:  string = 'w';  break;
        case 0x12:  string = 'e';  break;
        case 0x13:  string = 'r';  break;
        case 0x14:  string = 't';  break;
        case 0x15:  string = 'y';  break;
        case 0x16:  string = 'u';  break;
        case 0x17:  string = 'i';  break;
        case 0x18:  string = 'o';  break;
        case 0x19:  string = 'p';  break;
        // 대괄호, 특수문자
        case 0x1A:  string = '[';  break;
        case 0x1B:  string = ']';  break;
        case 0x1C:  string = 0x13;  break;
        //case 0x1D:  string = ;  break; 왼쪽 컨트롤
        // 쿼티 중간
        case 0x1E:  string = 'a';  break;
        case 0x1F:  string = 's';  break;
        case 0x20:  string = 'd';  break;
        case 0x21:  string = 'f';  break;
        case 0x22:  string = 'g';  break;
        case 0x23:  string = 'h';  break;
        case 0x24:  string = 'j';  break;
        case 0x25:  string = 'k';  break;
        case 0x26:  string = 'l';  break;
        // 세미클론, ' 옆에 이거 하고 이거 역방향하고 왼쪽 쉬프트하고 
        case 0x27:  string = ';';  break;
        case 0x28:  string = 0x27;  break;
        case 0x29:  string = '`';  break;
        case 0x2A:  shiftOn = TRUE;  break; // 왼 쉬프트
        case 0x2B:  string = 0x5C;  break;
        // 쿼티하단
        case 0x2C:  string = 'z';  break;
        case 0x2D:  string = 'x';  break;
        case 0x2E:  string = 'c';  break;
        case 0x2F:  string = 'v';  break;
        case 0x30:  string = 'b';  break;
        case 0x31:  string = 'n';  break;
        case 0x32:  string = 'm';  break;
        // .,특수문자
        case 0x33:  string = ',';  break;
        case 0x34:  string = '.';  break;
        case 0x35:  string = '/';  break;
        case 0x36:  shiftOn = TRUE; break; // 오른 쉬프트

        case 0x39:  string = 0x20;  break; // 스페이스바

        // 떼질때 필요한거
        case 0xAA:  shiftOn = FALSE; break; // 왼 쉬프트 뗌 
        case 0xB6:  shiftOn = FALSE; break; // 오른 쉬프트 뗌 
    }
    if (shiftOn == TRUE){
        if (string >= 'a' && string <= 'z'){
            string = string - 32;
        }
        DEBUG_draw_text(0,8, "Shift On");
    }
    return string;
}