/* HASH CRC-32:0x7dac5ed4 */
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
#ifndef __local__wtof_defined
#define __local__wtof_defined 1
/* Dependency: "wcstod" from "wchar" */
#ifndef ____localdep_wcstod_defined
#define ____localdep_wcstod_defined 1
#ifdef __std___localdep_wcstod_defined
__NAMESPACE_STD_USING(__localdep_wcstod)
#elif defined(__CRT_HAVE_wcstod)
__CREDIRECT(__ATTR_NONNULL((1)),double,__NOTHROW_NCX,__localdep_wcstod,(__WCHAR_TYPE__ const *__restrict __nptr, __WCHAR_TYPE__ **__endptr),wcstod,(__nptr,__endptr))
#else /* LIBC: wcstod */
#include <local/wchar/wcstod.h>
#define __localdep_wcstod (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wcstod))
#endif /* wcstod... */
#endif /* !____localdep_wcstod_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(_wtof) __ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1)) double
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(_wtof))(__WCHAR_TYPE__ const *__nptr) {
#line 3171 "kos/src/libc/magic/stdlib.c"
	return __localdep_wcstod(__nptr, __NULLPTR);
}
__NAMESPACE_LOCAL_END
#endif /* !__local__wtof_defined */
