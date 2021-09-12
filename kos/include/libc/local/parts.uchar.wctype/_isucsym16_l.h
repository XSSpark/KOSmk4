/* HASH CRC-32:0xa03d4459 */
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
#ifndef __local__isucsym16_l_defined
#define __local__isucsym16_l_defined 1
#include <__crt.h>
#include <hybrid/typecore.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep___isucsym16_defined
#define __local___localdep___isucsym16_defined 1
#if defined(__CRT_HAVE___iswcsym) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)
__CREDIRECT(__ATTR_CONST __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep___isucsym16,(__WINT16_TYPE__ __wc),__iswcsym,(__wc))
#elif defined(__CRT_HAVE_DOS$__iswcsym)
__CREDIRECT_DOS(__ATTR_CONST __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep___isucsym16,(__WINT16_TYPE__ __wc),__iswcsym,(__wc))
#elif __SIZEOF_WCHAR_T__ == 2
__NAMESPACE_LOCAL_END
#include <libc/local/wctype/__iswcsym.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep___isucsym16 (*(int(__LIBDCALL *)(__WINT16_TYPE__))&__LIBC_LOCAL_NAME(__iswcsym))
#else /* ... */
__NAMESPACE_LOCAL_END
#include <libc/local/parts.uchar.wctype/__isucsym16.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep___isucsym16 __LIBC_LOCAL_NAME(__isucsym16)
#endif /* !... */
#endif /* !__local___localdep___isucsym16_defined */
#ifndef __local___localdep_isualnum16_l_defined
#define __local___localdep_isualnum16_l_defined 1
__NAMESPACE_LOCAL_END
#include <bits/crt/wctype.h>
__NAMESPACE_LOCAL_BEGIN
#if defined(__crt_iswalnum_l) && defined(__CRT_HAVE_iswalnum_l) && __SIZEOF_WCHAR_T__ == 2
__COMPILER_EIREDIRECT(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__LIBDCALL,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),iswalnum_l,{ return __crt_iswalnum_l(__ch, __locale); })
#elif defined(__crt_iswalnum_l) && defined(__CRT_HAVE___iswalnum_l) && __SIZEOF_WCHAR_T__ == 2
__COMPILER_EIREDIRECT(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__LIBDCALL,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),__iswalnum_l,{ return __crt_iswalnum_l(__ch, __locale); })
#elif defined(__crt_iswalnum_l) && __SIZEOF_WCHAR_T__ == 2
__LOCAL __ATTR_PURE __ATTR_WUNUSED int __NOTHROW_NCX(__LIBDCALL __localdep_isualnum16_l)(__WINT16_TYPE__ __ch, __locale_t __locale) { return __crt_iswalnum_l(__ch, __locale); }
#elif defined(__CRT_HAVE_iswalnum_l) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),iswalnum_l,(__ch,__locale))
#elif defined(__CRT_HAVE_DOS$iswalnum_l)
__CREDIRECT_DOS(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),iswalnum_l,(__ch,__locale))
#elif defined(__CRT_HAVE__iswalnum_l) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),_iswalnum_l,(__ch,__locale))
#elif defined(__CRT_HAVE_DOS$_iswalnum_l)
__CREDIRECT_DOS(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),_iswalnum_l,(__ch,__locale))
#elif defined(__CRT_HAVE___iswalnum_l) && __SIZEOF_WCHAR_T__ == 2 && defined(__LIBCCALL_IS_LIBDCALL)
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),__iswalnum_l,(__ch,__locale))
#elif defined(__CRT_HAVE_DOS$__iswalnum_l)
__CREDIRECT_DOS(__ATTR_PURE __ATTR_WUNUSED,int,__NOTHROW_NCX,__localdep_isualnum16_l,(__WINT16_TYPE__ __ch, __locale_t __locale),__iswalnum_l,(__ch,__locale))
#elif __SIZEOF_WCHAR_T__ == 2
__NAMESPACE_LOCAL_END
#include <libc/local/wctype/iswalnum_l.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_isualnum16_l (*(int(__LIBDCALL *)(__WINT16_TYPE__, __locale_t))&__LIBC_LOCAL_NAME(iswalnum_l))
#else /* ... */
__NAMESPACE_LOCAL_END
#include <libc/local/parts.uchar.wctype/isualnum16_l.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_isualnum16_l __LIBC_LOCAL_NAME(isualnum16_l)
#endif /* !... */
#endif /* !__local___localdep_isualnum16_l_defined */
__LOCAL_LIBC(_isucsym16_l) __ATTR_PURE __ATTR_WUNUSED int
__NOTHROW_NCX(__LIBDCALL __LIBC_LOCAL_NAME(_isucsym16_l))(__WINT16_TYPE__ __wc, __locale_t __locale) {
#if defined(__CRT_KOS) && defined(__CRT_HAVE___unicode_descriptor)
	(void)__locale;
	__COMPILER_IMPURE();
	return __localdep___isucsym16(__wc);
#else /* __CRT_KOS && __CRT_HAVE___unicode_descriptor */
	return __localdep_isualnum16_l(__wc, __locale) || __wc == '_' || __wc == '$';
#endif /* !__CRT_KOS || !__CRT_HAVE___unicode_descriptor */
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep__isucsym16_l_defined
#define __local___localdep__isucsym16_l_defined 1
#define __localdep__isucsym16_l __LIBC_LOCAL_NAME(_isucsym16_l)
#endif /* !__local___localdep__isucsym16_l_defined */
#endif /* !__local__isucsym16_l_defined */
