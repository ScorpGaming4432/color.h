#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdarg.h>

#define RESET 0
#define BOLD 1
#define FEINT 2
#define ITALIC 3
#define UNDERLINE 4
#define SLOW_BLINK 5
#define RAPID_BLINK 6
#define INVERSE 7
#define CONCEAL 8
#define CROSS 9
#define DEFAULT_FONT 10
#define ALTERNATE_FONT(x) DEFAULT_FONT + x 
#define FRAKTUR 20
#define BOLD_OFF 21
#define DOUBLE_UNDERLINE 21
#define BOLD_FAINT 22
#define ITALIC_FRAKTUR 23
#define UNDERLINE_OFF 24
#define BLINK_OFF 25
#define INVERSE_OFF 27
#define CONCEAL_OFF 28
#define CROSS_OFF 29
#define SET_FOREGROUND(x) // Next arguments are 5;<n> or 2;<r>;<g>;<b>, see below
#define RESET_FOREGROUND 39
#define SET_BACKGROUND(x) // Next arguments are 5;<n> or 2;<r>;<g>;<b>, see below
#define RESET_BACKGROUND 49
#define FRAMED 51
#define ENCIRCLED 52
#define OVERLINED 53
#define FRAMED_OVERLINED 54
#define NOT_OVERLINED
#define IDEOGRAM_UNDERLINE 60
#define IDEOGRAM_OVERLINE 62
#define IDEOGRAM_DOUBLE_OVERLINE 63
#define IDEOGRAM_STRESS 64
#define IDEOGRAM_OFF 65
#define BRIGHTNESS_FOREGROUND(x) 90+x
#define BRIGHTNESS_BACKGROUND(x) 100+x

#define BLACK 30
#define RED 31

/*Example usage:
    c_printf("{RED}Hello, {BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7)}World!{RESET}", *args);
maybe:
    c_printf(RED "Hello, " BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7) "World!" RESET, *args);
Maybe use the colors inside args??
    c_printf(RED "Hello, " BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7) "World!" RESET, *args);
do three different functions maybe
*/ 
inline int c_printf(const char *const _Color_Format);

/* Writes already formatted string ready to print into _Buffer
*/
inline int c_sprintf(char* _Buffer, const char *const _Color_Format);

#endif