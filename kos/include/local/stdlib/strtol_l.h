/* HASH CRC-32:0xc7243c90 */
/* Copyright (c) 2019 Griefer@Work                                            *
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
#ifndef __local_strtol_l_defined
#define __local_strtol_l_defined 1
/* Dependency: "strtol" from "stdlib" */
#ifndef ____localdep_strtol_defined
#define ____localdep_strtol_defined 1
#ifdef __CRT_HAVE_strtol
__CREDIRECT(__ATTR_LEAF __ATTR_NONNULL((1)),long,__NOTHROW_NCX,__localdep_strtol,(char const *__restrict __nptr, char **__endptr, int __base),strtol,(__nptr,__endptr,__base))
#elif defined(__CRT_HAVE_strtoll) && (__SIZEOF_LONG__ == __SIZEOF_LONG_LONG__)
__CREDIRECT(__ATTR_LEAF __ATTR_NONNULL((1)),long,__NOTHROW_NCX,__localdep_strtol,(char const *__restrict __nptr, char **__endptr, int __base),strtoll,(__nptr,__endptr,__base))
#elif defined(__CRT_HAVE_strtoq) && (__SIZEOF_LONG__ == __SIZEOF_LONG_LONG__)
__CREDIRECT(__ATTR_LEAF __ATTR_NONNULL((1)),long,__NOTHROW_NCX,__localdep_strtol,(char const *__restrict __nptr, char **__endptr, int __base),strtoq,(__nptr,__endptr,__base))
#elif defined(__CRT_HAVE_strto32) && (__SIZEOF_LONG__ == 4)
__CREDIRECT(__ATTR_LEAF __ATTR_NONNULL((1)),long,__NOTHROW_NCX,__localdep_strtol,(char const *__restrict __nptr, char **__endptr, int __base),strto32,(__nptr,__endptr,__base))
#elif defined(__CRT_HAVE_strto64) && (__SIZEOF_LONG__ == 8)
__CREDIRECT(__ATTR_LEAF __ATTR_NONNULL((1)),long,__NOTHROW_NCX,__localdep_strtol,(char const *__restrict __nptr, char **__endptr, int __base),strto64,(__nptr,__endptr,__base))
#elif defined(__CRT_HAVE__strtoi64) && (__SIZEOF_LONG__ == 8)
__CREDIRECT(__ATTR_LEAF __ATTR_NONNULL((1)),long,__NOTHROW_NCX,__localdep_strtol,(char const *__restrict __nptr, char **__endptr, int __base),_strtoi64,(__nptr,__endptr,__base))
#elif defined(__CRT_HAVE_strtoimax) && (__SIZEOF_LONG__ == __SIZEOF_INTMAX_T__)
__CREDIRECT(__ATTR_LEAF __ATTR_NONNULL((1)),long,__NOTHROW_NCX,__localdep_strtol,(char const *__restrict __nptr, char **__endptr, int __base),strtoimax,(__nptr,__endptr,__base))
#else /* LIBC: strtol */
#include <local/stdlib/strtol.h>
#define __localdep_strtol (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(strtol))
#endif /* strtol... */
#endif /* !____localdep_strtol_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(strtol_l) __ATTR_NONNULL((1)) long
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(strtol_l))(char const *__restrict __nptr,
                                                      char **__endptr,
                                                      int __base,
                                                      __locale_t __locale) {
#line 1541 "kos/src/libc/magic/stdlib.c"
	(void)__locale;
	return __localdep_strtol(__nptr, __endptr, __base);
}
__NAMESPACE_LOCAL_END
#endif /* !__local_strtol_l_defined */
