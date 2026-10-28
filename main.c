#include <stdio.h>
#include <string.h>
#define COLOR_H_IMPL
#include "color.h"

#define TEST_STR "Hello, World!"

int main(void)
{
    size_t test_len = strlen(TEST_STR);
    c_printf(FORMAT(BOLD) TEST_STR FORMAT(RESET) "\n%zu\n", test_len);

    c_printf("{RED}Hello, {BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7)}World! %d %s{RESET}\n",
             42,
             "format arguments still work");
    c_printf("{BLUE UNDERLINE}This is blue and underlined.{RESET}\n");

    c_printf("%s%d%s{RESET}\n", FORMAT(RED) FORMAT(UNDERLINE_OFF)"I hate the recent " FORMAT(GREEN), 67, FORMAT(RED)" memes");

    char formatted[64];
    c_sprintf(formatted, "{GREEN}Value: %d{RESET}", 7);
    if (strcmp(formatted, "\x1b[32mValue: 7\x1b[0m") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{BOLD BLACK UNDERLINE BRIGHTNESS_BACKGROUND(7)}%d", 7);
    if (strcmp(formatted, "\x1b[1;30;4;107m7") != 0)
    {
        return 1;
    }

    c_sprintf(formatted, "{SET_FOREGROUND(5;196)}x");
    if (strcmp(formatted, "\x1b[38;5;196mx") != 0)
    {
        return 1;
    }

    return 0;
}
