/* HASH CRC-32:0x681b4315 */
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
#ifndef __local_j1l_defined
#define __local_j1l_defined
#include <__crt.h>
#if defined(__CRT_HAVE_j1) || defined(__CRT_HAVE___j1)
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_j1_defined
#define __local___localdep_j1_defined
#if __has_builtin(__builtin_j1) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_j1)
__CEIREDIRECT(__ATTR_WUNUSED,double,__NOTHROW,__localdep_j1,(double __x),j1,{ return __builtin_j1(__x); })
#elif defined(__CRT_HAVE_j1)
__CREDIRECT(__ATTR_WUNUSED,double,__NOTHROW,__localdep_j1,(double __x),j1,(__x))
#elif defined(__CRT_HAVE___j1)
__CREDIRECT(__ATTR_WUNUSED,double,__NOTHROW,__localdep_j1,(double __x),__j1,(__x))
#else /* ... */
#undef __local___localdep_j1_defined
#endif /* !... */
#endif /* !__local___localdep_j1_defined */
__LOCAL_LIBC(j1l) __ATTR_WUNUSED __LONGDOUBLE
__NOTHROW(__LIBCCALL __LIBC_LOCAL_NAME(j1l))(__LONGDOUBLE __x) {
	return (__LONGDOUBLE)(__NAMESPACE_LOCAL_SYM __localdep_j1)((double)__x);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_j1l_defined
#define __local___localdep_j1l_defined
#define __localdep_j1l __LIBC_LOCAL_NAME(j1l)
#endif /* !__local___localdep_j1l_defined */
#else /* __CRT_HAVE_j1 || __CRT_HAVE___j1 */
#undef __local_j1l_defined
#endif /* !__CRT_HAVE_j1 && !__CRT_HAVE___j1 */
#endif /* !__local_j1l_defined */
