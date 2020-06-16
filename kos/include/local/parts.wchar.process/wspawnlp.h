/* HASH CRC-32:0x1fe0236d */
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
#ifndef __local_wspawnlp_defined
#define __local_wspawnlp_defined 1
#include <__crt.h>
#ifdef __CRT_HAVE_wspawnvp
__NAMESPACE_LOCAL_BEGIN
/* Dependency: wspawnvp from parts.wchar.process */
#ifndef __local___localdep_wspawnvp_defined
#define __local___localdep_wspawnvp_defined 1
__CREDIRECT(__ATTR_NONNULL((2, 3)),__pid_t,__NOTHROW_RPC,__localdep_wspawnvp,(int __mode, __WCHAR_TYPE__ const *__restrict __file, __TWARGV),wspawnvp,(__mode,__file,___argv))
#endif /* !__local___localdep_wspawnvp_defined */
__NAMESPACE_LOCAL_END
#include <parts/redirect-exec.h>
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(wspawnlp) __ATTR_SENTINEL __ATTR_NONNULL((2)) __pid_t
__NOTHROW_RPC(__VLIBCCALL __LIBC_LOCAL_NAME(wspawnlp))(int __mode, __WCHAR_TYPE__ const *__restrict __file, __WCHAR_TYPE__ const *__args, ...) {
	__REDIRECT_SPAWNL(__WCHAR_TYPE__, __localdep_wspawnvp, __mode, __file, __args)
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_wspawnlp_defined
#define __local___localdep_wspawnlp_defined 1
#define __localdep_wspawnlp __LIBC_LOCAL_NAME(wspawnlp)
#endif /* !__local___localdep_wspawnlp_defined */
#else /* __CRT_HAVE_wspawnvp */
#undef __local_wspawnlp_defined
#endif /* !__CRT_HAVE_wspawnvp */
#endif /* !__local_wspawnlp_defined */
