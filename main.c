#include <string.h>
#define COLOR_H_IMPL
#include "color.h"

#define TEST_STR "Hello, World!"

int main(void)
{
    size_t test_len = strlen(TEST_STR);
    c_printf("{BOLD CYAN}color_print feature demo{RESET}\n\n");

    c_printf("Plain printf path: %s (length %zu)\n", TEST_STR, test_len);
    c_printf("Brace tags with printf arguments: {RED}number=%d, text=%s{RESET}\n\n",
             42,
             "arguments still work");

    c_printf("Standard foreground colors: "
             "{BLACK}BLACK{RESET} {RED}RED{RESET} {GREEN}GREEN{RESET} "
             "{YELLOW}YELLOW{RESET} {BLUE}BLUE{RESET} {MAGENTA}MAGENTA{RESET} "
             "{CYAN}CYAN{RESET} {WHITE}WHITE{RESET}\n");
    c_printf("Standard backgrounds: "
             "{BACKGROUND_BLACK}BLACK{RESET} {BACKGROUND_RED}RED{RESET} "
             "{BACKGROUND_GREEN}GREEN{RESET} {BACKGROUND_YELLOW}YELLOW{RESET} "
             "{BACKGROUND_BLUE}BLUE{RESET} {BACKGROUND_MAGENTA}MAGENTA{RESET} "
             "{BACKGROUND_CYAN}CYAN{RESET} {BACKGROUND_WHITE}WHITE{RESET}\n");
    c_printf("Bright foregrounds: "
             "{BRIGHT_FOREGROUND(0)}0{RESET} {BRIGHT_FOREGROUND(1)}1{RESET} "
             "{BRIGHT_FOREGROUND(2)}2{RESET} {BRIGHT_FOREGROUND(3)}3{RESET} "
             "{BRIGHT_FOREGROUND(4)}4{RESET} {BRIGHT_FOREGROUND(5)}5{RESET} "
             "{BRIGHT_FOREGROUND(6)}6{RESET} {BRIGHT_FOREGROUND(7)}7{RESET}\n");
    c_printf("Named color aliases: {BRIGHT_RED}bright red{RESET} "
             "{BG_RED}red background{RESET}\n");
    c_printf("Extended colors: {SET_FOREGROUND(5;196)}indexed foreground{RESET} "
             "{SET_FOREGROUND(2;255;80;20)}RGB foreground{RESET} "
             "{SET_BACKGROUND(5;25)}indexed background{RESET} "
             "{SET_BACKGROUND(2;30;80;160)}RGB background{RESET}\n");
    c_printf("Bright backgrounds: "
             "{BRIGHT_BACKGROUND(0)} 0 {RESET} {BRIGHT_BACKGROUND(1)} 1 {RESET} "
             "{BRIGHT_BACKGROUND(2)} 2 {RESET} {BRIGHT_BACKGROUND(3)} 3 {RESET} "
             "{BRIGHT_BACKGROUND(4)} 4 {RESET} {BRIGHT_BACKGROUND(5)} 5 {RESET} "
             "{BRIGHT_BACKGROUND(6)} 6 {RESET} {BRIGHT_BACKGROUND(7)} 7 {RESET}\n");
    c_printf("{RESET_FOREGROUND}foreground reset{RESET_BACKGROUND} background reset\n\n");

    c_printf("Styles: {BOLD}bold{BOLD_OFF}, {FAINT}faint{NORMAL_INTENSITY}, "
             "{ITALIC}italic{ITALIC_OFF}, "
             "{UNDERLINE}underline{UNDERLINE_OFF}, "
             "{DOUBLE_UNDERLINE}double underline (terminal support varies)"
             "{UNDERLINE_OFF}{RESET}, {INVERSE}inverse{INVERSE_OFF}, "
             "{STRIKETHROUGH}strikethrough{STRIKETHROUGH_OFF}\n\n");

    c_printf("Direct FORMAT macros: " FORMAT(BOLD ";" BLUE) "bold blue"
             FORMAT(RESET) " and " FORMAT(BRIGHT_FOREGROUND(5)) "bright foreground"
             FORMAT(RESET) " and " FORMAT(FG(196)) "indexed foreground"
             FORMAT(RESET) "\n");
    c_printf("{BOLD RED}ERROR:{/} Something went wrong.\n"
             "{BOLD GREEN}SUCCESS:{/} Operation completed.\n"
             "{FG(196)}Custom colour{/}\n"
             "{BG(#202020) WHITE}Readable text{/}\n\n");
    c_printf("Unknown and incomplete tags stay literal: {NOT_A_STYLE}, {BOLD\n\n");

    char formatted[64];
    c_sprintf(formatted, "{GREEN}Value: %d{RESET}", 7);
    if (strcmp(formatted, "\x1b[32mValue: 7\x1b[0m") != 0)
    {
        return 1;
    }
    c_printf("c_sprintf result: %s\n", formatted);

    c_sprintf(formatted, "{BOLD BLACK UNDERLINE BRIGHT_BACKGROUND(7)}%d", 7);
    if (strcmp(formatted, "\x1b[1;30;4;107m7") != 0)
    {
        return 1;
    }
    c_printf("c_sprintf combined style: %s{RESET}\n", formatted);

    c_sprintf(formatted, "{BOLD_OFF}x");
    if (strcmp(formatted, "\x1b[22mx") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{DOUBLE_UNDERLINE}x");
    if (strcmp(formatted, "\x1b[21mx") != 0)
    {
        return 1;
    }
    if (strcmp(FORMAT(BOLD_OFF), "\x1b[22m") != 0 ||
        strcmp(FORMAT(DOUBLE_UNDERLINE), "\x1b[21m") != 0 ||
        strcmp(FORMAT(FG(196)), "\x1b[38;5;196m") != 0 ||
        strcmp(FORMAT(BG(42)), "\x1b[48;5;42m") != 0 ||
        strcmp(FORMAT(BG_RED), "\x1b[41m") != 0 ||
        strcmp(FORMAT(BRIGHT_RED), "\x1b[91m") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{SLOW_BLINK}");
    if (strcmp(formatted, "{SLOW_BLINK}") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{SET_FOREGROUND(5;196)}x");
    if (strcmp(formatted, "\x1b[38;5;196mx") != 0)
    {
        return 1;
    }
    c_printf("c_sprintf extended color: %s{RESET}\n", formatted);

    c_sprintf(formatted, "{FG(196)}x");
    if (strcmp(formatted, "\x1b[38;5;196mx") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{BG(42)}x");
    if (strcmp(formatted, "\x1b[48;5;42mx") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{#202020}x");
    if (strcmp(formatted, "\x1b[38;2;32;32;32mx") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{BG(#202020) WHITE}x");
    if (strcmp(formatted, "\x1b[48;2;32;32;32;37mx") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{/}x");
    if (strcmp(formatted, "\x1b[0mx") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{FG(256)}");
    if (strcmp(formatted, "{FG(256)}") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{#12G000}");
    if (strcmp(formatted, "{#12G000}") != 0)
    {
        return 1;
    }

    return 0;
}
