/* HASH CRC-32:0xe5e6b513 */
/* Copyright (c) 2019-2020 Griefer@Work                                       *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement in the product documentation would be  *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_format_vprintf_defined
#define __local_format_vprintf_defined 1
#include <kos/anno.h>
#include <bits/format-printer.h>




#include <bits/format-printer.h>




#include <libc/string.h>

#include <hybrid/__assert.h>
#if !defined(CONFIG_USE_LIBDISASM) && !defined(CONFIG_NO_USE_LIBDISASM) && \
  ((defined(__KERNEL__) && defined(__KOS__)) || ((defined(CONFIG_HAVE_LIBDISASM_DISASSEMBLER_H) || \
    defined(HAVE_LIBDISASM_DISASSEMBLER_H) || __has_include(<libdisasm/disassembler.h>)) && \
    defined(__CRT_HAVE_dlopen) && defined(__CRT_HAVE_dlsym)))
#define CONFIG_USE_LIBDISASM 1
#endif
#if !defined(CONFIG_USE_LIBDEBUGINFO) && !defined(CONFIG_NO_USE_LIBDEBUGINFO) && \
  ((defined(__KERNEL__) && defined(__KOS__)) || ((defined(CONFIG_HAVE_LIBDEBUGINFO_ADDR2LINE_H) || \
    defined(HAVE_LIBDEBUGINFO_ADDR2LINE_H) || __has_include(<libdebuginfo/addr2line.h>)) && \
    defined(__CRT_HAVE_dlopen) && defined(__CRT_HAVE_dlsym) && defined(__CRT_HAVE_dlgetmodule) && defined(__CRT_HAVE_dlmodulebase)))
#define CONFIG_USE_LIBDEBUGINFO 1
#endif
#ifdef CONFIG_USE_LIBDISASM
#if !defined(__KERNEL__) || !defined(__KOS__)
#include <dlfcn.h>
#endif
#include <libdisasm/disassembler.h>
#endif
#ifdef CONFIG_USE_LIBDEBUGINFO
#if !defined(__KERNEL__) || !defined(__KOS__)
#include <dlfcn.h>
#include <libdebuginfo/addr2line.h>
#else
#include <kernel/addr2line.h>
#endif
#endif
/* Dependency: "format_repeat" from "format-printer" */
#ifndef ____localdep_format_repeat_defined
#define ____localdep_format_repeat_defined 1
#ifdef __CRT_HAVE_format_repeat
/* Repeat `CH' a number of `NUM_REPETITIONS' times
 * The usual format-printer rules apply, and this function
 * is allowed to call `PRINTER' as often as it chooses */
__CREDIRECT(__ATTR_NONNULL((1)),__SSIZE_TYPE__,,__localdep_format_repeat,(__pformatprinter __printer, void *__arg, char __ch, __SIZE_TYPE__ __num_repetitions),format_repeat,(__printer,__arg,__ch,__num_repetitions)) __THROWS(...)
#else /* LIBC: format_repeat */
#include <local/format-printer/format_repeat.h>
/* Repeat `CH' a number of `NUM_REPETITIONS' times
 * The usual format-printer rules apply, and this function
 * is allowed to call `PRINTER' as often as it chooses */
#define __localdep_format_repeat (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(format_repeat))
#endif /* format_repeat... */
#endif /* !____localdep_format_repeat_defined */

/* Dependency: "format_escape" from "format-printer" */
#ifndef ____localdep_format_escape_defined
#define ____localdep_format_escape_defined 1
#ifdef __CRT_HAVE_format_escape
/* Do C-style escape on the given text, printing it to the given printer.
 * Input:
 * >> Hello "World" W
 * >> hat a great day.
 * Output #1: >> \"Hello \"World\" W\nhat a great day.\"
 * Output #2: >> Hello \"World\" W\nhat a great day
 * NOTE: Output #2 is generated if the `FORMAT_ESCAPE_FPRINTRAW' is set
 * This function escapes all control and non-ascii characters,
 * preferring octal encoding for control characters and hex-encoding
 * for other non-ascii characters, a behavior that may be modified
 * with the `FORMAT_ESCAPE_FFORCE*' flags
 * @param: PRINTER: A function called for all quoted portions of the text
 * @param: TEXTLEN: The total number of bytes to escape, starting at `text' */
__CREDIRECT(__ATTR_NONNULL((1)),__SSIZE_TYPE__,,__localdep_format_escape,(__pformatprinter __printer, void *__arg, /*utf-8*/ char const *__restrict __text, __SIZE_TYPE__ __textlen, unsigned int __flags),format_escape,(__printer,__arg,__text,__textlen,__flags)) __THROWS(...)
#elif defined(__CRT_HAVE_format_quote)
/* Do C-style escape on the given text, printing it to the given printer.
 * Input:
 * >> Hello "World" W
 * >> hat a great day.
 * Output #1: >> \"Hello \"World\" W\nhat a great day.\"
 * Output #2: >> Hello \"World\" W\nhat a great day
 * NOTE: Output #2 is generated if the `FORMAT_ESCAPE_FPRINTRAW' is set
 * This function escapes all control and non-ascii characters,
 * preferring octal encoding for control characters and hex-encoding
 * for other non-ascii characters, a behavior that may be modified
 * with the `FORMAT_ESCAPE_FFORCE*' flags
 * @param: PRINTER: A function called for all quoted portions of the text
 * @param: TEXTLEN: The total number of bytes to escape, starting at `text' */
__CREDIRECT(__ATTR_NONNULL((1)),__SSIZE_TYPE__,,__localdep_format_escape,(__pformatprinter __printer, void *__arg, /*utf-8*/ char const *__restrict __text, __SIZE_TYPE__ __textlen, unsigned int __flags),format_quote,(__printer,__arg,__text,__textlen,__flags)) __THROWS(...)
#else /* LIBC: format_escape */
#include <local/format-printer/format_escape.h>
/* Do C-style escape on the given text, printing it to the given printer.
 * Input:
 * >> Hello "World" W
 * >> hat a great day.
 * Output #1: >> \"Hello \"World\" W\nhat a great day.\"
 * Output #2: >> Hello \"World\" W\nhat a great day
 * NOTE: Output #2 is generated if the `FORMAT_ESCAPE_FPRINTRAW' is set
 * This function escapes all control and non-ascii characters,
 * preferring octal encoding for control characters and hex-encoding
 * for other non-ascii characters, a behavior that may be modified
 * with the `FORMAT_ESCAPE_FFORCE*' flags
 * @param: PRINTER: A function called for all quoted portions of the text
 * @param: TEXTLEN: The total number of bytes to escape, starting at `text' */
#define __localdep_format_escape (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(format_escape))
#endif /* format_escape... */
#endif /* !____localdep_format_escape_defined */

/* Dependency: "format_hexdump" from "format-printer" */
#ifndef ____localdep_format_hexdump_defined
#define ____localdep_format_hexdump_defined 1
#ifdef __CRT_HAVE_format_hexdump
/* Print a hex dump of the given data using the provided format printer
 * @param: PRINTER:  A function called for all quoted portions of the text
 * @param: DATA:     A pointer to the data that should be dumped
 * @param: SIZE:     The amount of bytes read starting at DATA
 * @param: LINESIZE: The max amount of bytes to include per-line
 *                   HINT: Pass ZERO(0) to use a default size (16)
 * @param: FLAGS:    A set of `"FORMAT_HEXDUMP_FLAG_*"'
 * @return: 0: The given data was successfully hex-dumped
 * @return: *: The first non-ZERO(0) return value of PRINTER */
__CREDIRECT(__ATTR_NONNULL((1)),__SSIZE_TYPE__,,__localdep_format_hexdump,(__pformatprinter __printer, void *__arg, void const *__restrict __data, __SIZE_TYPE__ __size, __SIZE_TYPE__ __linesize, unsigned int __flags),format_hexdump,(__printer,__arg,__data,__size,__linesize,__flags)) __THROWS(...)
#else /* LIBC: format_hexdump */
#include <local/format-printer/format_hexdump.h>
/* Print a hex dump of the given data using the provided format printer
 * @param: PRINTER:  A function called for all quoted portions of the text
 * @param: DATA:     A pointer to the data that should be dumped
 * @param: SIZE:     The amount of bytes read starting at DATA
 * @param: LINESIZE: The max amount of bytes to include per-line
 *                   HINT: Pass ZERO(0) to use a default size (16)
 * @param: FLAGS:    A set of `"FORMAT_HEXDUMP_FLAG_*"'
 * @return: 0: The given data was successfully hex-dumped
 * @return: *: The first non-ZERO(0) return value of PRINTER */
#define __localdep_format_hexdump (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(format_hexdump))
#endif /* format_hexdump... */
#endif /* !____localdep_format_hexdump_defined */

/* Dependency: "unicode_writeutf8" from "unicode" */
#ifndef ____localdep_unicode_writeutf8_defined
#define ____localdep_unicode_writeutf8_defined 1
#ifdef __CRT_HAVE_unicode_writeutf8
/* Write a given Unicode character `ch' to `dst' and return a pointer to its end location.
 * This function will write at most `UNICODE_UTF8_CURLEN' bytes to `dst' */
__CREDIRECT(__ATTR_RETNONNULL __ATTR_NONNULL((1)),char *,__NOTHROW_NCX,__localdep_unicode_writeutf8,(/*utf-8*/ char *__restrict __dst, __CHAR32_TYPE__ __ch),unicode_writeutf8,(__dst,__ch))
#else /* LIBC: unicode_writeutf8 */
#include <local/unicode/unicode_writeutf8.h>
/* Write a given Unicode character `ch' to `dst' and return a pointer to its end location.
 * This function will write at most `UNICODE_UTF8_CURLEN' bytes to `dst' */
#define __localdep_unicode_writeutf8 (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(unicode_writeutf8))
#endif /* unicode_writeutf8... */
#endif /* !____localdep_unicode_writeutf8_defined */

/* Dependency: "format_width" from "format-printer" */
#ifndef ____localdep_format_width_defined
#define ____localdep_format_width_defined 1
#ifdef __CRT_HAVE_format_width
/* Returns the width (number of characters; not bytes) of the given unicode string */
__CREDIRECT(__ATTR_PURE __ATTR_NONNULL((2)),__SSIZE_TYPE__,__NOTHROW_NCX,__localdep_format_width,(void *__arg, /*utf-8*/ char const *__restrict __data, __SIZE_TYPE__ __datalen),format_width,(__arg,__data,__datalen))
#else /* LIBC: format_width */
#include <local/format-printer/format_width.h>
/* Returns the width (number of characters; not bytes) of the given unicode string */
#define __localdep_format_width (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(format_width))
#endif /* format_width... */
#endif /* !____localdep_format_width_defined */

__NAMESPACE_LOCAL_BEGIN
/* Generic printf implementation
 * Taking a regular printf-style format string and arguments, these
 * functions will call the given `PRINTER' callback with various strings
 * that, when put together, result in the desired formated text.
 *  - `PRINTER' obviously is called with the text parts in their correct order
 *  - If `PRINTER' returns '< 0', the function returns immediately,
 *    yielding that same value. Otherwise, format_printf() returns
 *    the sum of all return values from `PRINTER'.
 *  - The strings passed to `PRINTER' may not necessarily be zero-terminated, and
 *    a second argument is passed that indicates the absolute length in characters.
 * Supported extensions:
 *  - `%q'-format mode: Semantics equivalent to `%s', this modifier escapes the string using
 *                        `format_escape' with flags set of 'FORMAT_ESCAPE_FNONE', or
 *                        `PRINTF_FLAG_PREFIX' when the '#' flag was used (e.g.: `%#q').
 *  - `%.*s'   Instead of reading an `int' and dealing with undefined behavior when negative, an `unsigned int' is read.
 *  - `%.?s'   Similar to `%.*s', but takes a `size_t' from the argument list instead of an `unsigned int', as well as define
 *             a fixed-length buffer size for string/quote formats (thus allowing you to print '\0' characters after quoting)
 *  - `%$s'    Same as `%.?s'
 *  - `%q'     Print an escaped string. (format_printf(..., "a%qb", "foo\nbar")) --> "a\"foo\\nbar\"b"
 *  - `%#q'    Same as %q, without quotes. (format_printf(..., "a%#qb", "foo\nbar")) --> "afoo\\nbarb"
 *  - `%Q'     Print an escaped character. (format_printf(..., "a%Qb", '\n')) --> "a\'\\n\'b"
 *  - `%#Q'    Same as %Q, without quotes. (format_printf(..., "a%#Qb", '\n')) --> "a\\nb"
 *  - `%I'     length modifier: Integral length equivalent to sizeof(size_t)/sizeof(uintptr_t).
 *  - `%I8'    length modifier: Integral length equivalent to sizeof(int8_t).
 *  - `%I16'   length modifier: Integral length equivalent to sizeof(int16_t).
 *  - `%I32'   length modifier: Integral length equivalent to sizeof(int32_t).
 *  - `%I64'   length modifier: Integral length equivalent to sizeof(int64_t).
 *  - `%[...]' Extended formating options, allowing for additional formating options:
 *             - `%$[hex]' / `%$[hex:lLoOpPaAhH]'
 *                 - Invoke `format_hexdump(..., va_arg(args, void *))'
 *                 - Integer-length (`%$I32[hex]') flags affect `FORMAT_HEXDUMP_(BYTES|WORDS|DWORDS|QWORDS)'
 *                 - The argument string affect flags (defaults to `FORMAT_HEXDUMP_FNORMAL')
 *                    - `FORMAT_HEXDUMP_FHEXLOWER':  yes:l, no:L
 *                    - `FORMAT_HEXDUMP_FOFFSETS':   yes:o, no:O
 *                    - `FORMAT_HEXDUMP_FNOADDRESS': yes:P, no:p
 *                    - `FORMAT_HEXDUMP_FNOASCII':   yes:A, no:a
 *                    - `FORMAT_HEXDUMP_FNOHEX':     yes:H, no:h
 *             - `%[gen]'
 *                 - Invoke a custom format printer function pointer passed through args
 *                   >> typedef ssize_t (*PGEN)(pformatprinter printer, void *arg);
 *                   >> PGEN g = va_arg(args, PGEN);
 *                   >> DO((*g)(printer, arg));
 *             - `%[gen:c]'
 *                 - Same as `%[gen]', but insert an additional argument `T' that depends
 *                   on the integer size prefix (`%I32[gen:c]') and defaults to `void *'
 *                   >> typedef ssize_t (*PGEN)(T a, pformatprinter printer, void *arg);
 *                   >> PGEN g = va_arg(args, PGEN);
 *                   >> T    a = va_arg(args, T);
 *                   >> DO((*g)(a, printer, arg));
 *             - `%[disasm]' / `%$[disasm]'
 *                 - Print the mnemonic and operands of a single native assembly
 *                   instruction read from `va_arg(args, void *)' (using `libdisasm')
 *                   s.a. `disasm_single()'
 *                 - When the second form (with a fixed buffer size) is used, do a full
 *                   disassembly of that number of bytes, following `DISASSEMBLER_FNORMAL'
 *                   s.a. `disasm()'
 *             - `%[vinfo]' / `%[vinfo:<FORMAT=%f(%l,%c) : %n>]'
 *                 - Print addr2line information for a text address read from `va_arg(args, void *)'
 *                 - The given FORMAT string is a special printf-like format declaration
 *                   that accepts the following substitutions:
 *                   - `%%'   Print a single `%'-character (used for escaping `%')
 *                   - `%p'   Output the queried text address the same way `format_printf(..., "%q", addr)' would (as `sizeof(void *) * 2' uppercase hex characters)
 *                   - `%n'   Symbol name of the surrounding symbol (managed), or `???'   `di_debug_addr2line_t::al_name'
 *                   - `%N'   Raw name of the surrounding symbol, or `???'                `di_debug_addr2line_t::al_rawname'
 *                   - `%l'   Source line number (1-based; 0 if unknown)                  `di_debug_addr2line_t::al_srcline'
 *                   - `%c'   Source column number (1-based; 0 if unknown)                `di_debug_addr2line_t::al_srccol'
 *                   - `%f'   Source filename (with path prefix)                          `di_debug_addr2line_t::(al_cubase|al_srcpath|al_srcfile)'
 *                   - `%Rf'  Source filename (without path prefix)                       `di_debug_addr2line_t::al_srcfile'
 *                   - `%<'   Absolute starting address of the associated source location `di_debug_addr2line_t::al_linestart'
 *                   - `%>'   Absolute end address of the associated source location      `di_debug_addr2line_t::al_lineend'
 *                   - `%1>'  Absolute end address of the associated source location - 1  `di_debug_addr2line_t::al_lineend'
 *                   - `%S'   Absolute starting address of the surrounding symbol         `di_debug_addr2line_t::al_symstart'
 *                   - `%E'   Absolute end address of the surrounding symbol              `di_debug_addr2line_t::al_symend'
 *                   - `%1E'  Absolute end address of the surrounding symbol - 1          `di_debug_addr2line_t::al_symend'
 *                   - `%Dl'  Declaration line number (1-based; 0 if unknown)             `di_debug_addr2line_t::al_dclline'
 *                   - `%Dc'  Declaration column number (1-based; 0 if unknown)           `di_debug_addr2line_t::al_dclcol'
 *                   - `%Df'  Declaration filename (with path prefix)                     `di_debug_addr2line_t::(al_cubase|al_dclpath|al_dclfile)'
 *                   - `%RDf' Declaration filename (without path prefix)                  `di_debug_addr2line_t::al_dclfile'
 *                   - Any other character[-sequence] is forwarded as-is
 *             - `%[...]' Other sequences are resered for future usage
 * >>> Possible (and actual) uses:
 *  - printf:           Unbuffered output into any kind of stream/file.
 *  - sprintf/snprintf: Unsafe/Counted string formatting into a user-supplied buffer.
 *  - strdupf:          Output into dynamically allocated heap memory,
 *                      increasing the buffer when it gets filled completely.
 *  - syslog:           Unbuffered system-log output.
 *  - ...               There are a _lot_ more... */
__LOCAL_LIBC(format_vprintf) __ATTR_NONNULL((1, 3)) __ATTR_LIBC_PRINTF(3, 0) __SSIZE_TYPE__
(__LIBCCALL __LIBC_LOCAL_NAME(format_vprintf))(__pformatprinter __printer,
                                               void *__arg,
                                               char const *__restrict __format,
                                               __builtin_va_list __args) __THROWS(...) {
#line 840 "kos/src/libc/magic/format-printer.c"
#define __CHAR_TYPE                 char
#define __CHAR_SIZE                 __SIZEOF_CHAR__
#define __FORMAT_REPEAT             __localdep_format_repeat
#define __FORMAT_ESCAPE             __localdep_format_escape
#define __FORMAT_HEXDUMP            __localdep_format_hexdump








#define __FORMAT_UNICODE_WRITEUTF8  __localdep_unicode_writeutf8
#define __FORMAT_WIDTH              __localdep_format_width

#define __FORMAT_PRINTER            __printer
#define __FORMAT_ARG                __arg
#define __FORMAT_FORMAT             __format
#define __FORMAT_ARGS               __args
#include <local/format-printf.h>
}
__NAMESPACE_LOCAL_END
#endif /* !__local_format_vprintf_defined */
