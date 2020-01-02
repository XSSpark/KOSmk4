/* HASH CRC-32:0x12afc23d */
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
#ifndef __local_iswupper_l_defined
#define __local_iswupper_l_defined 1
/* Dependency: "iswupper" from "wctype" */
#ifndef ____localdep_iswupper_defined
#define ____localdep_iswupper_defined 1
#if __has_builtin(__builtin_iswupper) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_iswupper)
__FORCELOCAL __ATTR_CONST __ATTR_WUNUSED int __NOTHROW(__LIBCCALL __localdep_iswupper)(__WINT_TYPE__ __wc) { return __builtin_iswupper(__wc); }
#elif defined(__CRT_HAVE_iswupper)
__CREDIRECT(__ATTR_CONST __ATTR_WUNUSED,int,__NOTHROW,__localdep_iswupper,(__WINT_TYPE__ __wc),iswupper,(__wc))
#else /* LIBC: iswupper */
#include <local/wctype/iswupper.h>
#define __localdep_iswupper (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(iswupper))
#endif /* iswupper... */
#endif /* !____localdep_iswupper_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(iswupper_l) __ATTR_PURE __ATTR_WUNUSED int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(iswupper_l))(__WINT_TYPE__ __wc,
                                                        __locale_t __locale) {
#line 293 "kos/src/libc/magic/wctype.c"
	(void)__locale;
	__COMPILER_IMPURE();
	return __localdep_iswupper(__wc);
}
__NAMESPACE_LOCAL_END
#endif /* !__local_iswupper_l_defined */
