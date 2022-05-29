/* HASH CRC-32:0x1e1defba */
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
#ifndef __local_getuid_defined
#define __local_getuid_defined
#include <__crt.h>
#ifdef __CRT_HAVE_getresuid
#include <bits/types.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_getresuid_defined
#define __local___localdep_getresuid_defined
__CREDIRECT(__ATTR_OUT_OPT(1) __ATTR_OUT_OPT(2) __ATTR_OUT_OPT(3),int,__NOTHROW_NCX,__localdep_getresuid,(__uid_t *__ruid, __uid_t *__euid, __uid_t *__suid),getresuid,(__ruid,__euid,__suid))
#endif /* !__local___localdep_getresuid_defined */
__LOCAL_LIBC(getuid) __ATTR_WUNUSED __uid_t
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(getuid))(void) {
	__uid_t __result;
	if ((__NAMESPACE_LOCAL_SYM __localdep_getresuid)(&__result, __NULLPTR, __NULLPTR))
		__result = (__uid_t)-1;
	return __result;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_getuid_defined
#define __local___localdep_getuid_defined
#define __localdep_getuid __LIBC_LOCAL_NAME(getuid)
#endif /* !__local___localdep_getuid_defined */
#else /* __CRT_HAVE_getresuid */
#undef __local_getuid_defined
#endif /* !__CRT_HAVE_getresuid */
#endif /* !__local_getuid_defined */