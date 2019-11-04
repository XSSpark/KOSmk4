/* HASH CRC-32:0x7bf4f9e9 */
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
#ifndef __local_getumask_defined
#if defined(__CRT_HAVE_umask) || defined(__CRT_HAVE__umask)
#define __local_getumask_defined 1
/* Dependency: "umask" */
#ifndef ____localdep_umask_defined
#define ____localdep_umask_defined 1
#if defined(__CRT_HAVE_umask)
__CREDIRECT(,__mode_t,__NOTHROW_NCX,__localdep_umask,(__mode_t __mode),umask,(__mode))
#elif defined(__CRT_HAVE__umask)
__CREDIRECT(,__mode_t,__NOTHROW_NCX,__localdep_umask,(__mode_t __mode),_umask,(__mode))
#else /* LIBC: umask */
#undef ____localdep_umask_defined
#endif /* umask... */
#endif /* !____localdep_umask_defined */

__NAMESPACE_LOCAL_BEGIN
/* Return the current umask.
 * WARNING: This function isn't thread-safe */
__LOCAL_LIBC(getumask) __mode_t
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(getumask))(void) {
#line 494 "kos/src/libc/magic/sys.stat.c"
	__mode_t __result;
	__result = __localdep_umask(0);
	__localdep_umask(__result);
	return __result;
}
__NAMESPACE_LOCAL_END
#endif /* defined(__CRT_HAVE_umask) || defined(__CRT_HAVE__umask) */
#endif /* !__local_getumask_defined */
