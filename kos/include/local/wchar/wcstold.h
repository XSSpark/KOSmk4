/* HASH CRC-32:0xb3e6711e */
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
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2019-2020 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_wcstold_defined
#define __local_wcstold_defined 1
#include <__crt.h>
__NAMESPACE_LOCAL_BEGIN
/* Dependency: format_scanf from format-printer */
#ifndef __local___localdep_format_scanf_defined
#define __local___localdep_format_scanf_defined 1
#ifdef __CRT_HAVE_format_scanf
__NAMESPACE_LOCAL_END
#include <kos/anno.h>
#include <bits/format-printer.h>
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
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
__LIBC __ATTR_LIBC_SCANF(4, 5) __ATTR_NONNULL((1, 2, 4)) __SSIZE_TYPE__ (__VLIBCCALL __localdep_format_scanf)(__pformatgetc __pgetc, __pformatungetc __pungetc, void *__arg, char const *__restrict __format, ...) __THROWS(...) __CASMNAME("format_scanf");
#else /* __CRT_HAVE_format_scanf */
__NAMESPACE_LOCAL_END
#include <local/format-printer/format_scanf.h>
__NAMESPACE_LOCAL_BEGIN
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
#define __localdep_format_scanf __LIBC_LOCAL_NAME(format_scanf)
#endif /* !__CRT_HAVE_format_scanf */
#endif /* !__local___localdep_format_scanf_defined */
__NAMESPACE_LOCAL_END
#include <asm/crt/stdio.h>
#if __SIZEOF_WCHAR_T__ == 1
#ifndef ____vsscanf_getc_defined
#define ____vsscanf_getc_defined 1
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(vsscanf_getc) __SSIZE_TYPE__
(__FORMATPRINTER_CC __vsscanf_getc)(void *__arg) {
	char const *__reader = *(char const **)__arg;
	__CHAR32_TYPE__ __result = __unicode_readutf8(&__reader);
	if (!__result)
		return __EOF;
	*(char const **)__arg = __reader;
	return __result;
}
__LOCAL_LIBC(vsscanf_ungetc) __SSIZE_TYPE__
(__FORMATPRINTER_CC __vsscanf_ungetc)(void *__arg, __CHAR32_TYPE__ __UNUSED(__ch)) {
	__unicode_readutf8_rev((char const **)__arg);
	return 0;
}
__NAMESPACE_LOCAL_END
#endif /* !____vsscanf_getc_defined */

#elif __SIZEOF_WCHAR_T__ == 2
#ifndef ____vsc16scanf_getc_defined
#define ____vsc16scanf_getc_defined 1
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(vsc16scanf_getc) __SSIZE_TYPE__
(__FORMATPRINTER_CC __vsc16scanf_getc)(void *__arg) {
	__CHAR16_TYPE__ const *__reader = *(__CHAR16_TYPE__ const **)__arg;
	__CHAR32_TYPE__ __result = __unicode_readutf16(&__reader);
	if (!__result)
		return __EOF;
	*(__CHAR16_TYPE__ const **)__arg = __reader;
	return __result;
}
__LOCAL_LIBC(vsc16scanf_ungetc) __SSIZE_TYPE__
(__FORMATPRINTER_CC __vsc16scanf_ungetc)(void *__arg, __CHAR32_TYPE__ __UNUSED(__ch)) {
	__unicode_readutf16_rev((__CHAR16_TYPE__ const **)__arg);
	return 0;
}
__NAMESPACE_LOCAL_END
#endif /* !____vsc16scanf_getc_defined */

#else /* ... */
#ifndef ____vsc32scanf_getc_defined
#define ____vsc32scanf_getc_defined 1
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(vsc32scanf_getc) __SSIZE_TYPE__
(__FORMATPRINTER_CC __vsc32scanf_getc)(void *__arg) {
	__CHAR32_TYPE__ const *__reader = *(__CHAR32_TYPE__ const **)__arg;
	__CHAR32_TYPE__ __result = *__reader++;
	if (!__result)
		return __EOF;
	*(__CHAR32_TYPE__ const **)__arg = __reader;
	return __result;
}
__LOCAL_LIBC(vsc32scanf_ungetc) __SSIZE_TYPE__
(__FORMATPRINTER_CC __vsc32scanf_ungetc)(void *__arg, __CHAR32_TYPE__ __UNUSED(__ch)) {
	--*(__CHAR32_TYPE__ const **)__arg;
	return 0;
}
__NAMESPACE_LOCAL_END
#endif /* !____vsc32scanf_getc_defined */

#endif /* !... */
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(wcstold) __ATTR_LEAF __ATTR_NONNULL((1)) __LONGDOUBLE
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(wcstold))(__WCHAR_TYPE__ const *__restrict __nptr, __WCHAR_TYPE__ **__endptr) {
	__LONGDOUBLE __result;
	__WCHAR_TYPE__ const *__text_pointer = __nptr;
#if __SIZEOF_WCHAR_T__ == 1
	if (!__localdep_format_scanf(&__NAMESPACE_LOCAL_SYM __vsscanf_getc,
	                  &__NAMESPACE_LOCAL_SYM __vsscanf_ungetc,
	                  (void *)&__text_pointer, "%Lf", &__result))
		__result = 0.0L;
#elif __SIZEOF_WCHAR_T__ == 2
	if (!__localdep_format_scanf(&__NAMESPACE_LOCAL_SYM __vsc16scanf_getc,
	                  &__NAMESPACE_LOCAL_SYM __vsc16scanf_ungetc,
	                  (void *)&__text_pointer, "%Lf", &__result))
		__result = 0.0L;
#else /* ... */
	if (!__localdep_format_scanf(&__NAMESPACE_LOCAL_SYM __vsc32scanf_getc,
	                  &__NAMESPACE_LOCAL_SYM __vsc32scanf_ungetc,
	                  (void *)&__text_pointer, "%Lf", &__result))
		__result = 0.0L;
#endif /* !... */
	if (__endptr)
		*__endptr = (__WCHAR_TYPE__ *)__text_pointer;
	return __result;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_wcstold_defined
#define __local___localdep_wcstold_defined 1
#define __localdep_wcstold __LIBC_LOCAL_NAME(wcstold)
#endif /* !__local___localdep_wcstold_defined */
#endif /* !__local_wcstold_defined */
