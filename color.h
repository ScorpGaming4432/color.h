/**
 * @file color.h
 * @brief Print printf-style formatted text with inline ANSI color and style tags.
 * @version 0.1.0
 * @author Dorpiee (ScorpGaming4432)
 *
 * @details
 * `c_printf` and `c_sprintf` accept printf-style format strings with optional
 * color tags. A tag is enclosed in braces and contains one or more
 * space-separated style names:
 *
 * @code{.c}
 * c_printf("{RED}Hello, {BOLD BLACK UNDERLINE}world!{RESET}\n");
 * @endcode
 *
 * The tag is replaced by an ANSI SGR escape sequence. Unknown or malformed
 * tags are kept as literal text. Formatting conversions such as `%d` and `%s`
 * are processed by the standard printf family after tag conversion.
 * Named colors include `RED` for standard foreground, `BG_RED` for standard
 * background, and `BRIGHT_RED` for bright foreground. Shortcuts include
 * `{FG(196)}` for indexed foreground, `{#202020}` for RGB foreground,
 * `{BG(24)}` for indexed background, `{BG(#202020)}` for RGB background, and
 * `{/}` as a reset alias.
 *
 * @par Implementation
 * Define `COLOR_H_IMPL` in exactly one C or C++ translation unit immediately
 * before including this header. Include the header normally in other
 * translation units.
 *
 * @code{.c}
 * #define COLOR_H_IMPL
 * #include "color.h"
 * @endcode
 *
 * @warning `c_sprintf` has no destination-size parameter and uses
 * `vsprintf`. Ensure the destination buffer is large enough. As with `printf`,
 * do not use untrusted text as the format string.
 * @note ANSI escape sequences are interpreted only by terminals that support
 * them.
 */

#ifndef COLOR_H
#define COLOR_H

// #include <stdio.h>
#include <stdarg.h>

/**
 * @defgroup color_format_macros ANSI format macros
 * @brief Build ANSI SGR sequences directly into a printf-style format string.
 *
 * Use `FORMAT(code)` to wrap one or more semicolon-separated SGR codes.
 * The code macros below expand to string literals, so they can be combined
 * directly:
 *
 * @code{.c}
 * printf(FORMAT(BOLD ";" RED) "Bold red text" FORMAT(RESET) "\n");
 * @endcode
 *
 * These macros do not parse brace-style tags; use them when calling `printf`
 * directly or when constructing a format string for `c_printf`.
 * @{
 */
/** Wrap SGR parameter string @p x in an ANSI escape sequence. */
#define FORMAT(x) "\033[" x "m"

/** Reset all SGR attributes. */
#define RESET "0"
/** Enable bold/intense text. */
#define BOLD "1"
/** Enable faint text. */
#define FAINT "2"
#define FEINT FAINT
#define DIM FAINT
/** Enable italic text. */
#define ITALIC "3"
/** Enable underlined text. */
#define UNDERLINE "4"
/** Enable blinking text. */
#define BLINK "5"
/** Invert foreground and background colors. */
#define INVERSE "7"
#define REVERSE "7"
/** Enable concealed text. */
#define CONCEAL "8"
#define HIDE CONCEAL
/** Strike through text. */
#define STRIKETHROUGH "9"
#define CROSS STRIKETHROUGH
/** Enable double underline where supported (SGR 21). */
#define DOUBLE_UNDERLINE "21"
/** Disable bold and faint text (normal intensity, SGR 22). */
#define NORMAL_INTENSITY "22"
#define BOLD_OFF NORMAL_INTENSITY
#define BOLD_FAINT NORMAL_INTENSITY
/** Disable italic text. */
#define ITALIC_OFF "23"
/** Disable underline. */
#define UNDERLINE_OFF "24"
/** Disable inverse video. */
#define INVERSE_OFF "27"
/** Disable strikethrough. */
#define STRIKETHROUGH_OFF "29"
/** Compatibility alias for STRIKETHROUGH_OFF. */
#define CROSS_OFF STRIKETHROUGH_OFF
/** Set foreground color using parameters such as `5;196` or `2;255;0;0`. */
#define SET_FOREGROUND(x) "38;" #x
/** Set foreground to 256-color palette index @p x (0-255). */
#define FG(x) "38;5;" #x
/** Restore the default foreground color. */
#define RESET_FOREGROUND "39"
/** Set standard background color to black. */
#define BACKGROUND_BLACK "40"
/** Set standard background color to red. */
#define BACKGROUND_RED "41"
/** Set standard background color to green. */
#define BACKGROUND_GREEN "42"
/** Set standard background color to yellow. */
#define BACKGROUND_YELLOW "43"
/** Set standard background color to blue. */
#define BACKGROUND_BLUE "44"
/** Set standard background color to magenta. */
#define BACKGROUND_MAGENTA "45"
/** Set standard background color to cyan. */
#define BACKGROUND_CYAN "46"
/** Set standard background color to white. */
#define BACKGROUND_WHITE "47"
/** Short aliases for standard background colors. */
#define BG_BLACK BACKGROUND_BLACK
#define BG_RED BACKGROUND_RED
#define BG_GREEN BACKGROUND_GREEN
#define BG_YELLOW BACKGROUND_YELLOW
#define BG_BLUE BACKGROUND_BLUE
#define BG_MAGENTA BACKGROUND_MAGENTA
#define BG_CYAN BACKGROUND_CYAN
#define BG_WHITE BACKGROUND_WHITE
/** Set background color using parameters such as `5;196` or `2;255;0;0`. */
#define SET_BACKGROUND(x) "48;" #x
/** Set background to 256-color palette index @p x (0-255). */
#define BG(x) "48;5;" #x
/** Restore the default background color. */
#define RESET_BACKGROUND "49"

/** Set standard foreground color to black. */
#define BLACK "30"
/** Set standard foreground color to red. */
#define RED "31"
/** Set standard foreground color to green. */
#define GREEN "32"
/** Set standard foreground color to yellow. */
#define YELLOW "33"
/** Set standard foreground color to blue. */
#define BLUE "34"
/** Set standard foreground color to magenta. */
#define MAGENTA "35"
/** Set standard foreground color to cyan. */
#define CYAN "36"
/** Set standard foreground color to white. */
#define WHITE "37"

/** Set bright foreground color. */
#define BRIGHT_BLACK "90"
#define BRIGHT_RED "91"
#define BRIGHT_GREEN "92"
#define BRIGHT_YELLOW "93"
#define BRIGHT_BLUE "94"
#define BRIGHT_MAGENTA "95"
#define BRIGHT_CYAN "96"
#define BRIGHT_WHITE "97"
/** Set bright foreground color @p x (0-7). */
#define BRIGHT_FOREGROUND(x) "9" #x
/** Set bright background color @p x (0-7). */
#define BRIGHT_BACKGROUND(x) "10" #x
/** Compatibility aliases for BRIGHT_FOREGROUND and BRIGHT_BACKGROUND. */
#define BRIGHTNESS_FOREGROUND(x) BRIGHT_FOREGROUND(x)
#define BRIGHTNESS_BACKGROUND(x) BRIGHT_BACKGROUND(x)
/** @} */

/**
 * @brief Print formatted text with optional brace-style ANSI tags.
 *
 * Tags may contain one or more supported names separated by spaces or tabs.
 * For example, `{BOLD RED}` emits one combined SGR sequence. Use `{RESET}` to
 * restore terminal attributes when appropriate.
 * The supported tag set focuses on commonly implemented SGR attributes.
 * Double underline (SGR 21) is terminal-dependent; normal intensity (SGR 22)
 * disables both bold and faint because SGR has no portable bold-only reset.
 *
 * The function supports the same `%` conversions and variadic arguments as
 * `printf`. If the format contains no opening brace, it is sent directly to
 * `vprintf` without tag conversion.
 *
 * @param[in] _Color_Format printf-style format string with optional color tags.
 * @param[in] ... Arguments corresponding to conversions in @p _Color_Format.
 * @return Number of characters printed, or a negative value on failure.
 *
 * @par Example
 * @code{.c}
 * c_printf("{RED}Hello, {BOLD BLACK UNDERLINE}world!{RESET} %d\n", 42);
 * @endcode
 *
 * @warning The format string is interpreted by `vprintf`; pass untrusted text
 * as an argument with `%s`, not as the format string itself.
 */
static inline int c_printf(const char *const _Color_Format, ...);

/**
 * @brief Format text with optional brace-style ANSI tags into a buffer.
 *
 * This function behaves like `sprintf`, with the same tag handling as
 * c_printf(). The caller must provide enough space for the entire formatted
 * result, including the terminating null character.
 *
 * @param[out] _Buffer Destination buffer.
 * @param[in] _Color_Format printf-style format string with optional color tags.
 * @param[in] ... Arguments corresponding to conversions in @p _Color_Format.
 * @return Number of characters written, excluding the terminating null
 *         character, or a negative value on failure.
 *
 * @warning This function uses `vsprintf` and cannot check the size of
 * @p _Buffer. An undersized buffer causes undefined behavior.
 * @warning Do not use untrusted text as the format string.
 */
static inline int c_sprintf(char *_Buffer, const char *const _Color_Format, ...);

#ifdef COLOR_H_IMPL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int color_token_matches(const char *token, size_t length, const char *name)
{
    return strlen(name) == length && memcmp(token, name, length) == 0;
}

static inline int color_read_number(
    const char *text,
    size_t length,
    int maximum,
    int *number)
{
    int value = 0;

    if (length == 0)
    {
        return 0;
    }

    for (size_t i = 0; i < length; ++i)
    {
        if (text[i] < '0' || text[i] > '9')
        {
            return 0;
        }

        int digit = text[i] - '0';
        if (digit > maximum || value > (maximum - digit) / 10)
        {
            return 0;
        }
        value = value * 10 + digit;
    }

    *number = value;
    return 1;
}

static inline int color_read_hex_byte(const char *text, int *value)
{
    int result = 0;
    for (size_t i = 0; i < 2; ++i)
    {
        char character = text[i];
        int digit;
        if (character >= '0' && character <= '9')
        {
            digit = character - '0';
        }
        else if (character >= 'a' && character <= 'f')
        {
            digit = character - 'a' + 10;
        }
        else if (character >= 'A' && character <= 'F')
        {
            digit = character - 'A' + 10;
        }
        else
        {
            return 0;
        }

        result = result * 16 + digit;
    }

    *value = result;
    return 1;
}

static inline void color_append_decimal(char *output, size_t *output_length, int value)
{
    if (value >= 100)
    {
        output[(*output_length)++] = (char)('0' + value / 100);
        value %= 100;
        output[(*output_length)++] = (char)('0' + value / 10);
        output[(*output_length)++] = (char)('0' + value % 10);
    }
    else if (value >= 10)
    {
        output[(*output_length)++] = (char)('0' + value / 10);
        output[(*output_length)++] = (char)('0' + value % 10);
    }
    else
    {
        output[(*output_length)++] = (char)('0' + value);
    }
}

static inline void color_append_rgb(
    char *output,
    size_t *output_length,
    const char *prefix,
    int red,
    int green,
    int blue)
{
    size_t prefix_length = strlen(prefix);
    memcpy(output + *output_length, prefix, prefix_length);
    *output_length += prefix_length;

    const int channels[] = {red, green, blue};
    for (size_t i = 0; i < sizeof(channels) / sizeof(channels[0]); ++i)
    {
        if (i > 0)
        {
            output[(*output_length)++] = ';';
        }
        color_append_decimal(output, output_length, channels[i]);
    }
}

static inline int color_append_token(
    char *output,
    size_t *output_length,
    const char *token,
    size_t token_length)
{
    static const struct
    {
        const char *name;
        const char *code;
    } codes[] = {
        {"RESET", "0"},
        {"/", "0"},
        {"BOLD", "1"},
        {"**", "1"},
        {"FAINT", "2"},
        {"FEINT", "2"},
        {"DIM", "2"},
        {"ITALIC", "3"},
        {"*", "3"},
        {"UNDERLINE", "4"},
        {"_", "4"},
        {"BLINK", "5"},
        {"INVERSE", "7"},
        {"REVERSE", "7"},
        {"@", "7"},
        {"CONCEAL", "8"},
        {"HIDE", "8"},
        {"!", "8"},
        {"STRIKETHROUGH", "9"},
        {"CROSS", "9"},
        {"~", "9"},
        {"DOUBLE_UNDERLINE", "21"},
        {"__", "21"},
        {"BOLD_OFF", "22"},
        {"BOLD_FAINT", "22"},
        {"NORMAL_INTENSITY", "22"},
        {"ITALIC_OFF", "23"},
        {"UNDERLINE_OFF", "24"},
        {"INVERSE_OFF", "27"},
        {"STRIKETHROUGH_OFF", "29"},
        {"CROSS_OFF", "29"},
        {"RESET_FOREGROUND", "39"},
        {"RESET_BACKGROUND", "49"},
        {"BLACK", "30"},
        {"RED", "31"},
        {"GREEN", "32"},
        {"YELLOW", "33"},
        {"BLUE", "34"},
        {"MAGENTA", "35"},
        {"CYAN", "36"},
        {"WHITE", "37"},
        {"BRIGHT_BLACK", "90"},
        {"BRIGHT_RED", "91"},
        {"BRIGHT_GREEN", "92"},
        {"BRIGHT_YELLOW", "93"},
        {"BRIGHT_BLUE", "94"},
        {"BRIGHT_MAGENTA", "95"},
        {"BRIGHT_CYAN", "96"},
        {"BRIGHT_WHITE", "97"},
        {"BACKGROUND_BLACK", "40"},
        {"BACKGROUND_RED", "41"},
        {"BACKGROUND_GREEN", "42"},
        {"BACKGROUND_YELLOW", "43"},
        {"BACKGROUND_BLUE", "44"},
        {"BACKGROUND_MAGENTA", "45"},
        {"BACKGROUND_CYAN", "46"},
        {"BACKGROUND_WHITE", "47"},
        {"BG_BLACK", "40"},
        {"BG_RED", "41"},
        {"BG_GREEN", "42"},
        {"BG_YELLOW", "43"},
        {"BG_BLUE", "44"},
        {"BG_MAGENTA", "45"},
        {"BG_CYAN", "46"},
        {"BG_WHITE", "47"}
    };

    for (size_t i = 0; i < sizeof(codes) / sizeof(codes[0]); ++i)
    {
        if (color_token_matches(token, token_length, codes[i].name))
        {
            size_t code_length = strlen(codes[i].code);
            memcpy(output + *output_length, codes[i].code, code_length);
            *output_length += code_length;
            return 1;
        }
    }

    int red;
    int green;
    int blue;
    if (token_length == 7 && token[0] == '#' &&
        color_read_hex_byte(token + 1, &red) &&
        color_read_hex_byte(token + 3, &green) &&
        color_read_hex_byte(token + 5, &blue))
    {
        color_append_rgb(output, output_length, "38;2;", red, green, blue);
        return 1;
    }

    if (token_length == 11 && memcmp(token, "BG(#", 4) == 0 &&
        token[token_length - 1] == ')' &&
        color_read_hex_byte(token + 4, &red) &&
        color_read_hex_byte(token + 6, &green) &&
        color_read_hex_byte(token + 8, &blue))
    {
        color_append_rgb(output, output_length, "48;2;", red, green, blue);
        return 1;
    }

    const char *indexed_names[] = {"FG(", "BG("};
    const char *indexed_codes[] = {"38;5;", "48;5;"};
    for (size_t i = 0; i < sizeof(indexed_names) / sizeof(indexed_names[0]); ++i)
    {
        const size_t prefix_length = 3;
        if (token_length <= prefix_length ||
            memcmp(token, indexed_names[i], prefix_length) != 0 ||
            token[token_length - 1] != ')')
        {
            continue;
        }

        int index;
        if (!color_read_number(
                token + prefix_length,
                token_length - prefix_length - 1,
                255,
                &index))
        {
            return 0;
        }

        size_t code_length = strlen(indexed_codes[i]);
        memcpy(output + *output_length, indexed_codes[i], code_length);
        *output_length += code_length;
        color_append_decimal(output, output_length, index);
        return 1;
    }

    const char *dynamic_names[] = {
        "BRIGHT_FOREGROUND(",
        "BRIGHT_BACKGROUND(",
        "BRIGHTNESS_FOREGROUND(",
        "BRIGHTNESS_BACKGROUND(",
    };
    int dynamic_base[] = {90, 100, 90, 100};
    size_t prefix_lengths[] = {18, 18, 22, 22};

    for (size_t i = 0; i < sizeof(dynamic_names) / sizeof(dynamic_names[0]); ++i)
    {
        size_t prefix_length = prefix_lengths[i];
        if (token_length <= prefix_length ||
            memcmp(token, dynamic_names[i], prefix_length) != 0 ||
            token[token_length - 1] != ')')
        {
            continue;
        }

        int number;
        size_t number_length = token_length - prefix_length - 1;
        if (!color_read_number(token + prefix_length, number_length, 7, &number))
        {
            return 0;
        }

        int value = dynamic_base[i] + number;
        color_append_decimal(output, output_length, value);
        return 1;
    }

    const char *set_names[] = {"SET_FOREGROUND(", "SET_BACKGROUND("};
    size_t set_prefix_lengths[] = {15, 15};
    for (size_t i = 0; i < 2; ++i)
    {
        size_t prefix_length = set_prefix_lengths[i];
        if (token_length <= prefix_length ||
            memcmp(token, set_names[i], prefix_length) != 0 ||
            token[token_length - 1] != ')')
        {
            continue;
        }

        size_t value_length = token_length - prefix_length - 1;
        if (value_length == 0)
        {
            return 0;
        }

        for (size_t j = 0; j < value_length; ++j)
        {
            char character = token[prefix_length + j];
            if ((character < '0' || character > '9') && character != ';')
            {
                return 0;
            }
        }

        const char *prefix = (i == 0) ? "38;" : "48;";
        memcpy(output + *output_length, prefix, 3);
        *output_length += 3;
        memcpy(output + *output_length, token + prefix_length, value_length);
        *output_length += value_length;
        return 1;
    }

    return 0;
}

static inline char *color_convert_format(const char *format)
{
    size_t format_length = strlen(format);
    if (format_length > ((size_t)-1 - 1) / 3)
    {
        return NULL;
    }

    char *output = (char *)malloc(format_length * 3 + 1);
    if (output == NULL)
    {
        return NULL;
    }

    size_t input_index = 0;
    size_t output_length = 0;

    while (input_index < format_length)
    {
        if (format[input_index] != '{')
        {
            output[output_length++] = format[input_index++];
            continue;
        }

        size_t tag_end = input_index + 1;
        while (tag_end < format_length && format[tag_end] != '}')
        {
            ++tag_end;
        }

        if (tag_end == format_length)
        {
            output[output_length++] = format[input_index++];
            continue;
        }

        size_t tag_output_start = output_length;
        output[output_length++] = '\x1b';
        output[output_length++] = '[';

        size_t token_start = input_index + 1;
        int token_count = 0;
        int valid_tag = 1;

        while (token_start < tag_end)
        {
            while (token_start < tag_end &&
                   (format[token_start] == ' ' || format[token_start] == '\t'))
            {
                ++token_start;
            }

            if (token_start == tag_end)
            {
                break;
            }

            size_t token_end = token_start;
            while (token_end < tag_end &&
                   format[token_end] != ' ' && format[token_end] != '\t')
            {
                ++token_end;
            }

            if (token_count > 0)
            {
                output[output_length++] = ';';
            }

            if (!color_append_token(
                    output,
                    &output_length,
                    format + token_start,
                    token_end - token_start))
            {
                valid_tag = 0;
                break;
            }

            ++token_count;
            token_start = token_end;
        }

        if (!valid_tag || token_count == 0)
        {
            output_length = tag_output_start;
            size_t literal_length = tag_end - input_index + 1;
            memcpy(output + output_length, format + input_index, literal_length);
            output_length += literal_length;
        }
        else
        {
            output[output_length++] = 'm';
        }

        input_index = tag_end + 1;
    }

    output[output_length] = '\0';
    return output;
}

static inline int c_printf(const char *const _Color_Format, ...)
{
    if (_Color_Format == NULL)
    {
        return -1;
    }

    va_list arguments;
    va_start(arguments, _Color_Format);

    if (strchr(_Color_Format, '{') == NULL)
    {
        int result = vprintf(_Color_Format, arguments);
        va_end(arguments);
        return result;
    }

    char *converted_format = color_convert_format(_Color_Format);
    if (converted_format == NULL)
    {
        va_end(arguments);
        return -1;
    }

    int result = vprintf(converted_format, arguments);
    va_end(arguments);

    free(converted_format);
    return result;
}

static inline int c_sprintf(char *_Buffer, const char *const _Color_Format, ...)
{
    if (_Buffer == NULL || _Color_Format == NULL)
    {
        fprintf(stderr, "Argument found to be NULL");
        return -1;
    }

    va_list arguments;
    va_start(arguments, _Color_Format);

    if (strchr(_Color_Format, '{') == NULL)
    {
        int result = vsprintf(_Buffer, _Color_Format, arguments);
        va_end(arguments);
        return result;
    }

    char *converted_format = color_convert_format(_Color_Format);
    if (converted_format == NULL)
    {
        va_end(arguments);
        return -1;
    }

    int result = vsprintf(_Buffer, converted_format, arguments);
    va_end(arguments);

    free(converted_format);
    return result;
}
#endif
#endif
