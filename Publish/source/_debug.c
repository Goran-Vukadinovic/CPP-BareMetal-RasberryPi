#include "_debug.h"
#include "uart.h"
#include "_sprintf.h"

extern volatile unsigned char _end;
void _print(char* str)
{
#ifdef DEVELOP
    uart_puts(str);
#endif
}
void _printf(char *fmt, ...) {
#ifdef DEVELOP

    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    // we don't have memory allocation yet, so we
    // simply place our string after our code
    char *s = (char*)&_end;
    // use sprintf to format our string
    vsprintf(s,fmt,args);
    // print out as usual
    while(*s) {
        /* convert newline to carrige return + newline */
        if(*s=='\n')
            uart_send('\r');
        uart_send(*s++);
    }
#endif
}