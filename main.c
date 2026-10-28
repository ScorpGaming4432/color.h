#include <stdio.h>
#include <string.h>
#define COLOR_H_IMPL
#include "color.h"

#define TEST_STR "Hello, World!"

int main(int argc, char const *argv[])
{
    size_t test_len = strlen(TEST_STR);
    printf(FORMAT(BOLD) TEST_STR FORMAT(RESET) "\n%zu\n", test_len);

    c_printf("{RED}Hello, {BOLD}World!{RESET}\n");
    c_printf("{BLUE UNDERLINE}This is blue and underlined.{RESET}\n");

    return 0;
}
