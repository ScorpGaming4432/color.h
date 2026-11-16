# color.h

**A lightweight, single-header C/C++ library for printing formatted terminal text using ANSI escape sequences.**

`color.h` provides `printf`-style functions with inline color and formatting tags. Instead of manually writing ANSI escape sequences, you can use readable names, shorthand aliases, indexed colors, or RGB hexadecimal values directly inside your format strings.

```c
c_printf("{BOLD RED}ERROR:{/} Something went wrong.\n");
c_printf("{GREEN}Success!{/} Operation completed.\n");
c_printf("{#FF8800}Custom RGB color{/}\n");
c_printf("{BG(#202020) WHITE}Readable text{/}\n");
```

## Features

- **Single-header:** Just include `color.h` in your project.
- **No external dependencies:** Uses the standard C library.
- **printf compatibility:** Supports standard format specifiers and variadic arguments.
- **Readable syntax:** Use named colors and formatting tags.
- **Shorthand aliases:** Includes familiar symbols inspired by Markdown.
- **256-color support:** Specify colors using palette indices.
- **Truecolor support:** Specify foreground and background colors using hexadecimal RGB.
- **Direct ANSI macros:** Build escape sequences without using the custom parser.

## Getting started

### Installation

Copy `color.h` into your project and include it wherever needed.

This library uses a single-header implementation pattern. Define `COLOR_H_IMPL` in exactly one translation unit before including the header.

```c
// main.c
#define COLOR_H_IMPL
#include "color.h"
```

Other source files can include it normally:

```c
// other.c
#include "color.h"
```

### Basic usage

The library provides two functions:

| Function | Description |
|---|---|
| `c_printf()` | Prints formatted text to stdout. |
| `c_sprintf()` | Writes formatted text into a buffer. |

Both accept standard `printf`-style format strings with additional color tags.

```c
c_printf("{RED}Hello, world!{/}\n");

c_printf("{BOLD GREEN}Success!{/} %d items processed.\n", 42);
```

## Formatting syntax

Formatting tags are enclosed in curly braces `{}`. Multiple formatting options can be combined within a single tag, separated by spaces or tabs.

```c
c_printf("{BOLD UNDERLINE RED}Important text{/}\n");
```

### Text attributes

| Tag | Alias | Description |
|---|---|---|
| `BOLD` | `**` | Bold / increased intensity |
| `FAINT` | `DIM`, `FEINT` | Reduced intensity |
| `ITALIC` | `*` | Italic text |
| `UNDERLINE` | `_` | Underlined text |
| `DOUBLE_UNDERLINE` | `__` | Double underline |
| `BLINK` | | Blinking text |
| `INVERSE` | `REVERSE`, `@` | Inverted foreground and background |
| `CONCEAL` | `HIDE`, `!` | Concealed text |
| `STRIKETHROUGH` | `CROSS`, `~` | Strikethrough text |

### Reset and attribute controls

| Tag | Description |
|---|---|
| `RESET` | Reset all formatting attributes |
| `/` | Alias for `RESET` |
| `BOLD_OFF` | Disable bold and faint |
| `ITALIC_OFF` | Disable italic |
| `UNDERLINE_OFF` | Disable underline |
| `INVERSE_OFF` | Disable inverse |
| `STRIKETHROUGH_OFF` | Disable strikethrough |
| `RESET_FOREGROUND` | Restore default foreground color |
| `RESET_BACKGROUND` | Restore default background color |

Example:

```c
c_printf("{BOLD}Bold text{/} Normal text\n");
c_printf("{RED}Red{/} and {BLUE}blue{/}\n");
```

**Note:** `/` and `RESET` reset all SGR attributes, not just the most recently applied attribute. Formatting is not stack-based.

## Colors

### Standard foreground colors

| Tag | Color |
|---|---|
| `BLACK` | Black |
| `RED` | Red |
| `GREEN` | Green |
| `YELLOW` | Yellow |
| `BLUE` | Blue |
| `MAGENTA` | Magenta |
| `CYAN` | Cyan |
| `WHITE` | White |

### Standard background colors

Background colors use the `BG_` or `BACKGROUND_` prefix.

```c
c_printf("{BG_RED WHITE} White text on red background {/}\n");
```

Available tags:

`BG_BLACK`, `BG_RED`, `BG_GREEN`, `BG_YELLOW`, `BG_BLUE`, `BG_MAGENTA`, `BG_CYAN`, `BG_WHITE`.

The longer `BACKGROUND_` variants are also supported.

### Bright colors

Bright foreground colors are available through the `BRIGHT_` prefix.

```c
c_printf("{BRIGHT_RED}Bright red{/}\n");
c_printf("{BRIGHT_GREEN}Bright green{/}\n");
```

For dynamic bright colors, use:

- `BRIGHT_FOREGROUND(n)`
- `BRIGHT_BACKGROUND(n)`

Where `n` ranges from `0` to `7`.

### 256-color palette

Use `FG(n)` and `BG(n)` to specify an indexed color.

```c
c_printf("{FG(196)}Indexed foreground{/}\n");
c_printf("{BG(24) WHITE}Indexed background{/}\n");
```

The index ranges from `0` to `255`.

### Truecolor (RGB)

Use hexadecimal RGB notation for foreground colors:

```c
c_printf("{#FF0000}Red{/}\n");
c_printf("{#00FF00}Green{/}\n");
c_printf("{#0000FF}Blue{/}\n");
```

For background colors, wrap the hexadecimal value in `BG()`:

```c
c_printf("{BG(#202020) WHITE}Custom background{/}\n");
```

The supported format is `#RRGGBB`, with both uppercase and lowercase hexadecimal digits accepted.

## Direct ANSI formatting macros

If you prefer using regular `printf`, `FORMAT()` can construct ANSI escape sequences directly.

```c
printf(FORMAT(BOLD ";" RED) "Bold red text" FORMAT(RESET) "\n");
```

The library provides macros for:

- `FORMAT(x)` : Wrap SGR parameters in an ANSI escape sequence.
- `RESET`, `BOLD`, `FAINT`, `ITALIC`, `UNDERLINE`, etc.
- `BLACK`, `RED`, `GREEN`, `BLUE`, etc.
- `BG_RED`, `BG_GREEN`, `BG_BLUE`, etc.
- `BRIGHT_RED`, `BRIGHT_GREEN`, etc.
- `FG(x)` and `BG(x)` : Indexed foreground and background colors.
- `SET_FOREGROUND(x)` and `SET_BACKGROUND(x)` : Custom SGR color parameters.

These macros expand to string literals and can be concatenated directly.

For example:

```c
printf(
    FORMAT(BOLD ";" FG(196))
    "Custom bold color"
    FORMAT(RESET)
    "\n"
);
```

Unlike brace-style tags, these macros do not require `c_printf()` or `c_sprintf()`.

## Limitations

- **Terminal support:** ANSI escape sequences must be supported by the terminal to display correctly.
>Because of that, the library only handles popular ANSI SGR sequences. It does not support all possible ANSI escape codes. If you know your terminal supports additional sequences, you can use the direct ANSI macros to construct them manually, or if you want to add support for additional sequences, you can modify the library's source code, or even better - submit a pull request to the repository.
- **No automatic reset:** Formatting remains active until explicitly reset or changed.
- **No nested formatting:** Tags emit ANSI SGR sequences directly. They do not maintain a formatting stack.
- **Unknown tags:** Unknown or malformed tags are preserved as literal text.
- **`c_sprintf()` safety:** The function uses `vsprintf()` and does not accept a destination buffer size. Ensure the destination buffer is large enough.

As with standard `printf`, never use untrusted input as a format string. Pass external text through `%s` instead.

## License

See the repository's license file for licensing terms.