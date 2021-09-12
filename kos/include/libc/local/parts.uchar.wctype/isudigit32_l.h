/* HASH CRC-32:0x1e896703 */
/* Copyright (c) 2019-2021 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2021 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_isudigit32_l_defined
#define __local_isudigit32_l_defined 1
#include <__crt.h>
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_isudigit32_defined
#define __local___localdep_isudigit32_defined 1
__NAMESPACE_LOCAL_END
#include <bits/crt/wctype.h>
__NAMESPACE_LOCAL_BEGIN
#if defined(__crt_iswdigit) && defined(__CRT_HAVE_iswdigit) && __SIZEOF_WCHAR_T__ == 4
__COMPILER_EIREDIRECT(__ATTR_CONST __ATTR_WUNUSED,int,__NOTHROW,__LIBKCALL,__localdep_isudigit32,(__WINT32_TYPE__ __wc),iswdigit,{ return __crt_iswdigit(__wc); })
#elif defined(__crt_iswdigit) && __SIZEOF_WCHAR_T__ == 4
__LOCAL __ATTR_CONST __ATTR_WUNUSED int __NOTHROW(__LIBKCALL __localdep_isudigit32)(__WINT32_TYPE__ __wc) { return __crt_iswdigit(__wc); }
#elif __has_builtin(__builtin_iswdigit) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_iswdigit) && __SIZEOF_WCHAR_T__ == 4
__COMPILER_EIREDIRECT(__ATTR_CONST __ATTR_WUNUSED,int,__NOTHROW,__LIBKCALL,__localdep_isudigit32,(__WINT32_TYPE__ __wc),iswdigit,{ return __builtin_iswdigit(__wc); })
#elif defined(__CRT_HAVE_iswdigit) && __SIZEOF_WCHAR_T__ == 4 && defined(__LIBCCALL_IS_LIBKCALL)
__CREDIRECT(__ATTR_CONST __ATTR_WUNUSED,int,__NOTHROW,__localdep_isudigit32,(__WINT32_TYPE__ __wc),iswdigit,(__wc))
#elif defined(__CRT_HAVE_KOS$iswdigit)
__CREDIRECT_KOS(__ATTR_CONST __ATTR_WUNUSED,int,__NOTHROW,__localdep_isudigit32,(__WINT32_TYPE__ __wc),iswdigit,(__wc))
#elif __SIZEOF_WCHAR_T__ == 4
__NAMESPACE_LOCAL_END
#include <libc/local/wctype/iswdigit.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_isudigit32 (*(int(__LIBKCALL *)(__WINT32_TYPE__))&__LIBC_LOCAL_NAME(iswdigit))
#else /* ... */
__NAMESPACE_LOCAL_END
#include <libc/local/parts.uchar.wctype/isudigit32.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_isudigit32 __LIBC_LOCAL_NAME(isudigit32)
#endif /* !... */
#endif /* !__local___localdep_isudigit32_defined */
__LOCAL_LIBC(isudigit32_l) __ATTR_PURE __ATTR_WUNUSED int
__NOTHROW_NCX(__LIBKCALL __LIBC_LOCAL_NAME(isudigit32_l))(__WINT32_TYPE__ __ch, __locale_t __locale) {
	__COMPILER_IMPURE();
	(void)__locale;
	return __localdep_isudigit32(__ch);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_isudigit32_l_defined
#define __local___localdep_isudigit32_l_defined 1
#define __localdep_isudigit32_l __LIBC_LOCAL_NAME(isudigit32_l)
#endif /* !__local___localdep_isudigit32_l_defined */
#endif /* !__local_isudigit32_l_defined */
