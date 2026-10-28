#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdarg.h>

#define FORMAT(x) "\033[" x "m"
#define RESET             "0"
#define BOLD              "1"
#define FEINT             "2"
#define ITALIC            "3"
#define UNDERLINE         "4"
#define SLOW_BLINK        "5"
#define RAPID_BLINK       "6"
#define INVERSE           "7"
#define CONCEAL           "8"
#define CROSS             "9"
#define DEFAULT_FONT      "10"
#define ALTERNATE_FONT(x) DEFAULT_FONT + x
#define FRAKTUR           "20"
#define BOLD_OFF          "21"
#define DOUBLE_UNDERLINE  "21"
#define BOLD_FAINT        "22"
#define ITALIC_FRAKTUR    "23"
#define UNDERLINE_OFF     "24"
#define BLINK_OFF         "25"
#define INVERSE_OFF       "27"
#define CONCEAL_OFF       "28"
#define CROSS_OFF         "29"
#define SET_FOREGROUND(x) // Next arguments are 5;<n> or 2;<r>;<g>;<b>, see below
#define RESET_FOREGROUND  "39"
#define SET_BACKGROUND(x) // Next arguments are 5;<n> or 2;<r>;<g>;<b>, see below
#define RESET_BACKGROUND  "49"
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
#define BRIGHTNESS_FOREGROUND(x) 90 + x
#define BRIGHTNESS_BACKGROUND(x) 100 + x

#define BLACK 30
#define RED "31"

/*Example usage:
    c_printf("{RED}Hello, {BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7)}World!{RESET}", *args); <- we choose this as c_printf
maybe:
    c_printf(RED "Hello, " BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7) "World!" RESET, *args);
Maybe use the colors inside args??
    c_printf(RED "Hello, " BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7) "World!" RESET, *args);
do three different functions maybe
*/
inline int c_printf(const char *const _Color_Format);

/* Writes already formatted string ready to print into _Buffer
 */
inline int c_sprintf(char *_Buffer, const char *const _Color_Format);

#ifdef COLOR_H_IMPL
#include <string.h>
#include <stdlib.h>

/*
    This version is intentionally simple and easy to follow.
    It converts tags like {RED} or {BOLD RED} into ANSI escape codes.
    Example:
        c_printf("{RED}Hello{RESET}\n");
*/
inline int c_sprintf(char *_Buffer, const char *const _Color_Format)
{
    if (_Buffer == NULL || _Color_Format == NULL)
    {
        return -1;
    }

    size_t out_index = 0;
    size_t i = 0;

    while (_Color_Format[i] != '\0')
    {
        if (_Color_Format[i] == '{')
        {
            size_t end = i + 1;
            while (_Color_Format[end] != '\0' && _Color_Format[end] != '}')
            {
                ++end;
            }

            if (_Color_Format[end] == '}')
            {
                char tag[128];
                size_t tag_len = end - i - 1;

                if (tag_len >= sizeof(tag))
                {
                    tag_len = sizeof(tag) - 1;
                }

                memcpy(tag, _Color_Format + i + 1, tag_len);
                tag[tag_len] = '\0';

                char working_copy[128];
                char ansi_codes[128] = "";
                strcpy(working_copy, tag);
                char *token = strtok(working_copy, " ");

                while (token != NULL)
                {
                    char value[32] = "";
                    int number = 0;

                    if (strcmp(token, "RESET") == 0)
                    {
                        strcpy(value, "0");
                    }
                    else if (strcmp(token, "BOLD") == 0)
                    {
                        strcpy(value, "1");
                    }
                    else if (strcmp(token, "FEINT") == 0)
                    {
                        strcpy(value, "2");
                    }
                    else if (strcmp(token, "ITALIC") == 0)
                    {
                        strcpy(value, "3");
                    }
                    else if (strcmp(token, "UNDERLINE") == 0)
                    {
                        strcpy(value, "4");
                    }
                    else if (strcmp(token, "BLACK") == 0)
                    {
                        strcpy(value, "30");
                    }
                    else if (strcmp(token, "RED") == 0)
                    {
                        strcpy(value, "31");
                    }
                    else if (strcmp(token, "GREEN") == 0)
                    {
                        strcpy(value, "32");
                    }
                    else if (strcmp(token, "YELLOW") == 0)
                    {
                        strcpy(value, "33");
                    }
                    else if (strcmp(token, "BLUE") == 0)
                    {
                        strcpy(value, "34");
                    }
                    else if (strcmp(token, "MAGENTA") == 0)
                    {
                        strcpy(value, "35");
                    }
                    else if (strcmp(token, "CYAN") == 0)
                    {
                        strcpy(value, "36");
                    }
                    else if (strcmp(token, "WHITE") == 0)
                    {
                        strcpy(value, "37");
                    }
                    else if (sscanf(token, "BRIGHTNESS_FOREGROUND(%d)", &number) == 1)
                    {
                        snprintf(value, sizeof(value), "%d", 90 + number);
                    }
                    else if (sscanf(token, "BRIGHTNESS_BACKGROUND(%d)", &number) == 1)
                    {
                        snprintf(value, sizeof(value), "%d", 100 + number);
                    }
                    else if (strcmp(token, "RESET_FOREGROUND") == 0)
                    {
                        strcpy(value, "39");
                    }
                    else if (strcmp(token, "RESET_BACKGROUND") == 0)
                    {
                        strcpy(value, "49");
                    }

                    if (value[0] != '\0')
                    {
                        if (ansi_codes[0] != '\0')
                        {
                            strcat(ansi_codes, ";");
                        }
                        strcat(ansi_codes, value);
                    }

                    token = strtok(NULL, " ");
                }

                if (ansi_codes[0] != '\0')
                {
                    char escape[128];
                    snprintf(escape, sizeof(escape), "\033[%sm", ansi_codes);
                    size_t escape_len = strlen(escape);
                    memcpy(_Buffer + out_index, escape, escape_len + 1);
                    out_index += escape_len;
                }
                else
                {
                    /* Unknown tag: keep the original text so the program does not crash. */
                    memcpy(_Buffer + out_index, _Color_Format + i, end - i + 1);
                    out_index += end - i + 1;
                }

                i = end + 1;
                continue;
            }
        }

        _Buffer[out_index++] = _Color_Format[i++];
    }

    _Buffer[out_index] = '\0';
    return (int)out_index;
}

inline int c_printf(const char *const _Color_Format)
{
    char output[4096];
    int written = c_sprintf(output, _Color_Format);

    if (written < 0)
    {
        return written;
    }

    return printf("%s", output);
}
#endif
#endif