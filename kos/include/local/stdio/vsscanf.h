/* HASH CRC-32:0xc29cb85c */
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
#ifndef __local_vsscanf_defined
#define __local_vsscanf_defined 1
#include <kos/anno.h>
#include <bits/format-printer.h>
/* Dependency: "format_vscanf" from "format-printer" */
#ifndef ____localdep_format_vscanf_defined
#define ____localdep_format_vscanf_defined 1
#ifdef __CRT_HAVE_format_vscanf
/* Generic scanf implementation
 * Taking a regular scanf-style format string and argument, these
 * functions will call the given `SCANNER' function which in
 * return should successively yield a character at a time from
 * some kind of input source.
 *  - If `SCANNER' returns `< 0', scanning aborts and that value is returned.
 *    Otherwise, the function returns the amount of successfully parsed arguments.
 *  - The user may use `SCANNER' to track the last read character to get
 *    additional information about what character caused the scan to fail.
 *  - The given `SCANNER' should also indicate EOF by returning `NUL'
 *  - This implementation supports the following extensions:
 *    - `%[A-Z]'   -- Character ranges in scan patterns
 *    - `%[^abc]'  -- Inversion of a scan pattern
 *    - `"\n"'     -- Skip any kind of linefeed (`"\n"', `"\r"', `"\r\n"')
 *    - `%$s'      -- `$'-modifier, available for any format outputting a string.
 *                    This modifier reads a `size_t' from the argument list,
 *                    that specifies the size of the following string buffer:
 *                 >> char buffer[64];
 *                 >> sscanf(data, "My name is %.?s\n", sizeof(buffer), buffer);
 * format -> %[*|?][width][length]specifier
 * @return: 0 :  No data could be scanned.
 * @return: * :  The total number of successfully scanned arguments.
 * @return: EOF: `PGETC' returned EOF the first time an attempt at reading was made */
__CREDIRECT(__ATTR_NONNULL((1, 2, 4)) __ATTR_LIBC_SCANF(4, 0),__SSIZE_TYPE__,,__localdep_format_vscanf,(__pformatgetc __pgetc, __pformatungetc __pungetc, void *__arg, char const *__restrict __format, __builtin_va_list __args),format_vscanf,(__pgetc,__pungetc,__arg,__format,__args)) __THROWS(...)
#else /* LIBC: format_vscanf */
#include <local/format-printer/format_vscanf.h>
/* Generic scanf implementation
 * Taking a regular scanf-style format string and argument, these
 * functions will call the given `SCANNER' function which in
 * return should successively yield a character at a time from
 * some kind of input source.
 *  - If `SCANNER' returns `< 0', scanning aborts and that value is returned.
 *    Otherwise, the function returns the amount of successfully parsed arguments.
 *  - The user may use `SCANNER' to track the last read character to get
 *    additional information about what character caused the scan to fail.
 *  - The given `SCANNER' should also indicate EOF by returning `NUL'
 *  - This implementation supports the following extensions:
 *    - `%[A-Z]'   -- Character ranges in scan patterns
 *    - `%[^abc]'  -- Inversion of a scan pattern
 *    - `"\n"'     -- Skip any kind of linefeed (`"\n"', `"\r"', `"\r\n"')
 *    - `%$s'      -- `$'-modifier, available for any format outputting a string.
 *                    This modifier reads a `size_t' from the argument list,
 *                    that specifies the size of the following string buffer:
 *                 >> char buffer[64];
 *                 >> sscanf(data, "My name is %.?s\n", sizeof(buffer), buffer);
 * format -> %[*|?][width][length]specifier
 * @return: 0 :  No data could be scanned.
 * @return: * :  The total number of successfully scanned arguments.
 * @return: EOF: `PGETC' returned EOF the first time an attempt at reading was made */
#define __localdep_format_vscanf (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(format_vscanf))
#endif /* format_vscanf... */
#endif /* !____localdep_format_vscanf_defined */

/* Dependency: "unicode_readutf8" from "unicode" */
#ifndef ____localdep_unicode_readutf8_defined
#define ____localdep_unicode_readutf8_defined 1
#ifdef __CRT_HAVE_unicode_readutf8
/* Read a single Unicode character from a given UTF-8 string */
__CREDIRECT(__ATTR_NONNULL((1)),__CHAR32_TYPE__,__NOTHROW_NCX,__localdep_unicode_readutf8,(/*utf-8*/ char const **__restrict __ptext),unicode_readutf8,(__ptext))
#else /* LIBC: unicode_readutf8 */
#include <local/unicode/unicode_readutf8.h>
/* Read a single Unicode character from a given UTF-8 string */
#define __localdep_unicode_readutf8 (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(unicode_readutf8))
#endif /* unicode_readutf8... */
#endif /* !____localdep_unicode_readutf8_defined */

/* Dependency: "unicode_readutf8_rev" from "unicode" */
#ifndef ____localdep_unicode_readutf8_rev_defined
#define ____localdep_unicode_readutf8_rev_defined 1
#ifdef __CRT_HAVE_unicode_readutf8_rev
/* Same as `unicode_readutf8', but read backwards, with `*ptext'
 * starting out as a pointer after the character to be read, before
 * being updated to point to the start of the character that was read */
__CREDIRECT(__ATTR_NONNULL((1)),__CHAR32_TYPE__,__NOTHROW_NCX,__localdep_unicode_readutf8_rev,(/*utf-8*/ char const **__restrict __ptext),unicode_readutf8_rev,(__ptext))
#else /* LIBC: unicode_readutf8_rev */
#include <local/unicode/unicode_readutf8_rev.h>
/* Same as `unicode_readutf8', but read backwards, with `*ptext'
 * starting out as a pointer after the character to be read, before
 * being updated to point to the start of the character that was read */
#define __localdep_unicode_readutf8_rev (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(unicode_readutf8_rev))
#endif /* unicode_readutf8_rev... */
#endif /* !____localdep_unicode_readutf8_rev_defined */

__NAMESPACE_LOCAL_BEGIN
#include <hybrid/typecore.h>
#ifndef __EOF
#ifdef EOF
#define __EOF  EOF
#else
#define __EOF (-1)
#endif
#endif
__LOCAL_LIBC(vsscanf_getc) __SSIZE_TYPE__ (__LIBCCALL __vsscanf_getc)(void *__arg) {
	__CHAR32_TYPE__ __result = __localdep_unicode_readutf8((char const **)__arg);
	return __result ? __result : __EOF;
}
__LOCAL_LIBC(vsscanf_ungetc) __SSIZE_TYPE__ (__LIBCCALL __vsscanf_ungetc)(void *__arg, __CHAR32_TYPE__ __UNUSED(__ch)) {
	__localdep_unicode_readutf8_rev((char const **)__arg);
	return 0;
}
/* Scan data from a given `INPUT' string, following `FORMAT'
 * Return the number of successfully scanned data items */
__LOCAL_LIBC(vsscanf) __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) __ATTR_LIBC_SCANF(2, 0) __STDC_INT_AS_SIZE_T
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(vsscanf))(char const *__restrict __input,
                                                     char const *__restrict __format,
                                                     __builtin_va_list __args) {
#line 957 "kos/src/libc/magic/stdio.c"
	char const *__input_pointer = __input;
	return __localdep_format_vscanf(&__vsscanf_getc, &__vsscanf_ungetc,
	                     (void *)&__input_pointer, __format, __args);
}
__NAMESPACE_LOCAL_END
#endif /* !__local_vsscanf_defined */
