/* HASH CRC-32:0xf929a2dd */
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
#ifndef __local_wctomb_defined
#define __local_wctomb_defined 1
#include <__crt.h>
__NAMESPACE_LOCAL_BEGIN
/* Dependency: wcrtomb from wchar */
#ifndef __local___localdep_wcrtomb_defined
#define __local___localdep_wcrtomb_defined 1
#ifdef __CRT_HAVE_wcrtomb
__CREDIRECT(,__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcrtomb,(char *__restrict __str, __WCHAR_TYPE__ __wc, __mbstate_t *__mbs),wcrtomb,(__str,__wc,__mbs))
#elif defined(__CRT_HAVE_DOS$wcrtomb) && __SIZEOF_WCHAR_T__ == 4
__CREDIRECT_KOS(,__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcrtomb,(char *__restrict __str, __WCHAR_TYPE__ __wc, __mbstate_t *__mbs),wcrtomb,(__str,__wc,__mbs))
#elif defined(__CRT_HAVE_DOS$wcrtomb) && __SIZEOF_WCHAR_T__ == 2
__CREDIRECT_DOS(,__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcrtomb,(char *__restrict __str, __WCHAR_TYPE__ __wc, __mbstate_t *__mbs),wcrtomb,(__str,__wc,__mbs))
#elif defined(__CRT_HAVE_c16rtomb) && (__SIZEOF_WCHAR_T__ == 2)
__CREDIRECT(,__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcrtomb,(char *__restrict __str, __WCHAR_TYPE__ __wc, __mbstate_t *__mbs),c16rtomb,(__str,__wc,__mbs))
#elif defined(__CRT_HAVE_c32rtomb) && (__SIZEOF_WCHAR_T__ == 4)
__CREDIRECT(,__SIZE_TYPE__,__NOTHROW_NCX,__localdep_wcrtomb,(char *__restrict __str, __WCHAR_TYPE__ __wc, __mbstate_t *__mbs),c32rtomb,(__str,__wc,__mbs))
#else /* ... */
__NAMESPACE_LOCAL_END
#include <local/wchar/wcrtomb.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_wcrtomb __LIBC_LOCAL_NAME(wcrtomb)
#endif /* !... */
#endif /* !__local___localdep_wcrtomb_defined */
__LOCAL_LIBC(wctomb) int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(wctomb))(char *__str, __WCHAR_TYPE__ __wc) {
	return __localdep_wcrtomb(__str, __wc, __NULLPTR);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_wctomb_defined
#define __local___localdep_wctomb_defined 1
#define __localdep_wctomb __LIBC_LOCAL_NAME(wctomb)
#endif /* !__local___localdep_wctomb_defined */
#endif /* !__local_wctomb_defined */
