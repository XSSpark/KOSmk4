/* HASH CRC-32:0x73b19cba */
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
#ifndef __local_waccess_defined
#if defined(__CRT_AT_FDCWD) && defined(__CRT_HAVE_wfaccessat)
#define __local_waccess_defined 1
/* Dependency: "wfaccessat" from "parts.wchar.unistd" */
#ifndef ____localdep_wfaccessat_defined
#define ____localdep_wfaccessat_defined 1
#ifdef __CRT_HAVE_wfaccessat
/* >> wfaccessat(2)
 * @param: TYPE: Set of `X_OK|W_OK|R_OK'
 * Test for access to the specified file `DFD:FILE', testing for `TYPE' */
__CREDIRECT(__ATTR_NONNULL((2)),int,__NOTHROW_RPC,__localdep_wfaccessat,(__fd_t __dfd, __WCHAR_TYPE__ const *__file, int __type, __atflag_t __flags),wfaccessat,(__dfd,__file,__type,__flags))
#else /* LIBC: wfaccessat */
#undef ____localdep_wfaccessat_defined
#endif /* wfaccessat... */
#endif /* !____localdep_wfaccessat_defined */

__NAMESPACE_LOCAL_BEGIN
/* >> waccess(2)
 * @param: TYPE: Set of `X_OK|W_OK|R_OK'
 * Test for access to the specified file `FILE', testing for `TYPE' */
__LOCAL_LIBC(waccess) __ATTR_WUNUSED __ATTR_NONNULL((1)) int
__NOTHROW_RPC(__LIBCCALL __LIBC_LOCAL_NAME(waccess))(__WCHAR_TYPE__ const *__file,
                                                     int __type) {
#line 684 "kos/src/libc/magic/unistd.c"
	return __localdep_wfaccessat(__CRT_AT_FDCWD, __file, __type, 0);
}
__NAMESPACE_LOCAL_END
#endif /* __CRT_AT_FDCWD && __CRT_HAVE_wfaccessat */
#endif /* !__local_waccess_defined */
