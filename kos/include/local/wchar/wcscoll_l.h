/* HASH CRC-32:0x97765da3 */
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
#ifndef __local_wcscoll_l_defined
#define __local_wcscoll_l_defined 1
/* Dependency: "wcscoll" from "wchar" */
#ifndef ____localdep_wcscoll_defined
#define ____localdep_wcscoll_defined 1
#ifdef __std___localdep_wcscoll_defined
__NAMESPACE_STD_USING(__localdep_wcscoll)
#elif defined(__CRT_HAVE_wcscoll)
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1, 2)),int,__NOTHROW_NCX,__localdep_wcscoll,(__WCHAR_TYPE__ const *__s1, __WCHAR_TYPE__ const *__s2),wcscoll,(__s1,__s2))
#else /* LIBC: wcscoll */
#include <local/wchar/wcscoll.h>
#define __localdep_wcscoll (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wcscoll))
#endif /* wcscoll... */
#endif /* !____localdep_wcscoll_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(wcscoll_l) __ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(wcscoll_l))(__WCHAR_TYPE__ const *__s1,
                                                       __WCHAR_TYPE__ const *__s2,
                                                       __locale_t __locale) {
#line 485 "kos/src/libc/magic/string.c"
	(void)__locale;
	return __localdep_wcscoll(__s1, __s2);
}
__NAMESPACE_LOCAL_END
#endif /* !__local_wcscoll_l_defined */
