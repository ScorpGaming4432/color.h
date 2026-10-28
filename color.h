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

#include <stdio.h>
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
#define FEINT "2"
/** Enable italic text. */
#define ITALIC "3"
/** Enable underlined text. */
#define UNDERLINE "4"
/** Enable slow blinking text. */
#define SLOW_BLINK "5"
/** Enable rapid blinking text. */
#define RAPID_BLINK "6"
/** Invert foreground and background colors. */
#define INVERSE "7"
/** Conceal text. */
#define CONCEAL "8"
/** Strike through text. */
#define CROSS "9"
/** Select the default font. */
#define DEFAULT_FONT "10"
/** Select alternate font @p x (0-9). */
#define ALTERNATE_FONT(x) "1" #x
/** Select the Fraktur font. */
#define FRAKTUR "20"
/** Disable bold/intense text. */
#define BOLD_OFF "21"
/** Enable double underline. */
#define DOUBLE_UNDERLINE "21"
/** Disable bold and faint text. */
#define BOLD_FAINT "22"
/** Disable italic and Fraktur text. */
#define ITALIC_FRAKTUR "23"
/** Disable underline. */
#define UNDERLINE_OFF "24"
/** Disable blinking text. */
#define BLINK_OFF "25"
/** Disable inverse video. */
#define INVERSE_OFF "27"
/** Disable concealment. */
#define CONCEAL_OFF "28"
/** Disable strikethrough. */
#define CROSS_OFF "29"
/** Set foreground color using parameters such as `5;196` or `2;255;0;0`. */
#define SET_FOREGROUND(x) "38;" #x
/** Restore the default foreground color. */
#define RESET_FOREGROUND "39"
/** Set background color using parameters such as `5;196` or `2;255;0;0`. */
#define SET_BACKGROUND(x) "48;" #x
/** Restore the default background color. */
#define RESET_BACKGROUND "49"

/** Enclose text. */
#define FRAMED "51"
/** Mark text as encircled. */
#define ENCIRCLED "52"
/** Enable overline. */
#define OVERLINED "53"
/** Enable frame and overline. */
#define FRAMED_OVERLINED "54"
/** Disable overline. */
#define NOT_OVERLINED "55"
/** Enable ideogram underline. */
#define IDEOGRAM_UNDERLINE "60"
/** Enable ideogram overline. */
#define IDEOGRAM_OVERLINE "62"
/** Enable ideogram double overline. */
#define IDEOGRAM_DOUBLE_OVERLINE "63"
/** Enable ideogram stress marking. */
#define IDEOGRAM_STRESS "64"
/** Disable ideogram attributes. */
#define IDEOGRAM_OFF "65"

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

/** Set bright foreground color @p x (0-7). */
#define BRIGHTNESS_FOREGROUND(x) "9" #x
/** Set bright background color @p x (0-7). */
#define BRIGHTNESS_BACKGROUND(x) "10" #x
/** @} */

/**
 * @brief Print formatted text with optional brace-style ANSI tags.
 *
 * Tags may contain one or more supported names separated by spaces or tabs.
 * For example, `{BOLD RED}` emits one combined SGR sequence. Use `{RESET}` to
 * restore terminal attributes when appropriate.
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
#include <stdlib.h>
#include <string.h>

static inline int color_token_matches(const char *token, size_t length, const char *name)
{
    return strlen(name) == length && memcmp(token, name, length) == 0;
}

static inline int color_read_small_number(const char *text, size_t length, int *number)
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

        value = value * 10 + (text[i] - '0');
        if (value > 9)
        {
            return 0;
        }
    }

    *number = value;
    return 1;
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
        {"BOLD", "1"},
        {"FEINT", "2"},
        {"ITALIC", "3"},
        {"UNDERLINE", "4"},
        {"SLOW_BLINK", "5"},
        {"RAPID_BLINK", "6"},
        {"INVERSE", "7"},
        {"CONCEAL", "8"},
        {"CROSS", "9"},
        {"DEFAULT_FONT", "10"},
        {"FRAKTUR", "20"},
        {"BOLD_OFF", "21"},
        {"DOUBLE_UNDERLINE", "21"},
        {"BOLD_FAINT", "22"},
        {"ITALIC_FRAKTUR", "23"},
        {"UNDERLINE_OFF", "24"},
        {"BLINK_OFF", "25"},
        {"INVERSE_OFF", "27"},
        {"CONCEAL_OFF", "28"},
        {"CROSS_OFF", "29"},
        {"RESET_FOREGROUND", "39"},
        {"RESET_BACKGROUND", "49"},
        {"FRAMED", "51"},
        {"ENCIRCLED", "52"},
        {"OVERLINED", "53"},
        {"FRAMED_OVERLINED", "54"},
        {"NOT_OVERLINED", "55"},
        {"IDEOGRAM_UNDERLINE", "60"},
        {"IDEOGRAM_OVERLINE", "62"},
        {"IDEOGRAM_DOUBLE_OVERLINE", "63"},
        {"IDEOGRAM_STRESS", "64"},
        {"IDEOGRAM_OFF", "65"},
        {"BLACK", "30"},
        {"RED", "31"},
        {"GREEN", "32"},
        {"YELLOW", "33"},
        {"BLUE", "34"},
        {"MAGENTA", "35"},
        {"CYAN", "36"},
        {"WHITE", "37"}};

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

    const char *dynamic_names[] = {
        "BRIGHTNESS_FOREGROUND(",
        "BRIGHTNESS_BACKGROUND(",
        "ALTERNATE_FONT("};
    int dynamic_base[] = {90, 100, 10};
    size_t prefix_lengths[] = {22, 22, 15};

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
        if (!color_read_small_number(token + prefix_length, number_length, &number))
        {
            return 0;
        }

        int value = dynamic_base[i] + number;
        if (value >= 100)
        {
            output[(*output_length)++] = (char)('0' + value / 100);
            value %= 100;
            output[(*output_length)++] = (char)('0' + value / 10);
            output[(*output_length)++] = (char)('0' + value % 10);
        }
        else
        {
            output[(*output_length)++] = (char)('0' + value / 10);
            output[(*output_length)++] = (char)('0' + value % 10);
        }
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
    char *output = (char *)malloc(format_length + 1);
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
