/* HASH CRC-32:0xdeeb29c5 */
/* Copyright (c) 2019-2022 Griefer@Work                                       *
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
 *    Portions Copyright (c) 2019-2022 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_regfree_defined
#define __local_regfree_defined
#include <__crt.h>
#if (defined(__CRT_HAVE_free) || defined(__CRT_HAVE_cfree) || defined(__CRT_HAVE___libc_free)) && defined(__CRT_KOS)
#include <bits/crt/regex.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_free_defined
#define __local___localdep_free_defined
#if __has_builtin(__builtin_free) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_free)
__CEIREDIRECT(,void,__NOTHROW_NCX,__localdep_free,(void *__mallptr),free,{ __builtin_free(__mallptr); })
#elif defined(__CRT_HAVE_free)
__CREDIRECT_VOID(,__NOTHROW_NCX,__localdep_free,(void *__mallptr),free,(__mallptr))
#elif defined(__CRT_HAVE_cfree)
__CREDIRECT_VOID(,__NOTHROW_NCX,__localdep_free,(void *__mallptr),cfree,(__mallptr))
#elif defined(__CRT_HAVE___libc_free)
__CREDIRECT_VOID(,__NOTHROW_NCX,__localdep_free,(void *__mallptr),__libc_free,(__mallptr))
#else /* ... */
#undef __local___localdep_free_defined
#endif /* !... */
#endif /* !__local___localdep_free_defined */
__LOCAL_LIBC(regfree) void
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(regfree))(struct re_pattern_buffer *__self) {
	(__NAMESPACE_LOCAL_SYM __localdep_free)(__self->__REPB_PREFIX(buffer));
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_regfree_defined
#define __local___localdep_regfree_defined
#define __localdep_regfree __LIBC_LOCAL_NAME(regfree)
#endif /* !__local___localdep_regfree_defined */
#else /* (__CRT_HAVE_free || __CRT_HAVE_cfree || __CRT_HAVE___libc_free) && __CRT_KOS */
#undef __local_regfree_defined
#endif /* (!__CRT_HAVE_free && !__CRT_HAVE_cfree && !__CRT_HAVE___libc_free) || !__CRT_KOS */
#endif /* !__local_regfree_defined */