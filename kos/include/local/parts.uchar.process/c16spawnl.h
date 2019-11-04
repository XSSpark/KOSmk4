/* HASH CRC-32:0x9511a5e7 */
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
#ifndef __local_c16spawnl_defined
#if (defined(__CRT_HAVE_wspawnv) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE__wspawnv) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_DOS$_wspawnv)
#define __local_c16spawnl_defined 1
#include <parts/redirect-exec.h>
/* Dependency: "c16spawnv" from "parts.uchar.process" */
#ifndef ____localdep_c16spawnv_defined
#define ____localdep_c16spawnv_defined 1
#if defined(__CRT_HAVE_wspawnv) && (__SIZEOF_WCHAR_T__ == 2)
__CREDIRECT(__ATTR_NONNULL((2, 3)) __ATTR_NONNULL((2, 3)),__pid_t,__NOTHROW_RPC,__localdep_c16spawnv,(int __mode, __CHAR16_TYPE__ const *__restrict __path, __T16ARGV),wspawnv,(__mode,__path, ___argv))
#elif defined(__CRT_HAVE__wspawnv) && (__SIZEOF_WCHAR_T__ == 2)
__CREDIRECT(__ATTR_NONNULL((2, 3)) __ATTR_NONNULL((2, 3)),__pid_t,__NOTHROW_RPC,__localdep_c16spawnv,(int __mode, __CHAR16_TYPE__ const *__restrict __path, __T16ARGV),_wspawnv,(__mode,__path, ___argv))
#elif defined(__CRT_HAVE_DOS$_wspawnv)
__CREDIRECT_DOS(__ATTR_NONNULL((2, 3)) __ATTR_NONNULL((2, 3)),__pid_t,__NOTHROW_RPC,__localdep_c16spawnv,(int __mode, __CHAR16_TYPE__ const *__restrict __path, __T16ARGV),_wspawnv,(__mode,__path, ___argv))
#elif defined(__CRT_HAVE_DOS$_wspawnv)
__CREDIRECT_DOS(__ATTR_NONNULL((2, 3)) __ATTR_NONNULL((2, 3)),__pid_t,__NOTHROW_RPC,__localdep_c16spawnv,(int __mode, __CHAR16_TYPE__ const *__restrict __path, __T16ARGV),_wspawnv,(__mode,__path, ___argv))
#else /* LIBC: c16spawnv */
#undef ____localdep_c16spawnv_defined
#endif /* c16spawnv... */
#endif /* !____localdep_c16spawnv_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(c16spawnl) __ATTR_SENTINEL __ATTR_NONNULL((2)) __pid_t
__NOTHROW_RPC(__VLIBCCALL __LIBC_LOCAL_NAME(c16spawnl))(int __mode,
                                                        __CHAR16_TYPE__ const *__restrict __path,
                                                        __CHAR16_TYPE__ const *__args,
                                                        ... /*, (char16_t *)NULL*/) {
#line 200 "kos/src/libc/magic/parts.uchar.process.c"
	__REDIRECT_SPAWNL(__CHAR16_TYPE__, __localdep_c16spawnv, __mode, __path, __args)
}
__NAMESPACE_LOCAL_END
#endif /* (defined(__CRT_HAVE_wspawnv) && __SIZEOF_WCHAR_T__ == 2) || (defined(__CRT_HAVE__wspawnv) && __SIZEOF_WCHAR_T__ == 2) || defined(__CRT_HAVE_DOS$_wspawnv) */
#endif /* !__local_c16spawnl_defined */
