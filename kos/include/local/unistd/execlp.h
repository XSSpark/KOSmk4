/* HASH CRC-32:0xa62360af */
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
#ifndef __local_execlp_defined
#if defined(__CRT_HAVE_execvp) || defined(__CRT_HAVE__execvp)
#define __local_execlp_defined 1
#include <parts/redirect-exec.h>
/* Dependency: "execvp" */
#ifndef ____localdep_execvp_defined
#define ____localdep_execvp_defined 1
#if __has_builtin(__builtin_execvp) && defined(__LIBC_BIND_CRTBUILTINS) && defined(__CRT_HAVE_execvp)
/* >> execvp(3)
 * Replace the calling process with the application image referred to by `PATH' / `FILE'
 * and execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__FORCELOCAL __ATTR_NONNULL((1, 2)) int __NOTHROW_RPC(__LIBCCALL __localdep_execvp)(char const *__restrict __file, __TARGV) { return __builtin_execvp(__file, (char *const *)___argv); }
#elif defined(__CRT_HAVE_execvp)
/* >> execvp(3)
 * Replace the calling process with the application image referred to by `PATH' / `FILE'
 * and execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__CREDIRECT(__ATTR_NONNULL((1, 2)),int,__NOTHROW_RPC,__localdep_execvp,(char const *__restrict __file, __TARGV),execvp,(__file,___argv))
#elif defined(__CRT_HAVE__execvp)
/* >> execvp(3)
 * Replace the calling process with the application image referred to by `PATH' / `FILE'
 * and execute it's `main()' method, passing the given `ARGV', and setting `environ' to `ENVP' */
__CREDIRECT(__ATTR_NONNULL((1, 2)),int,__NOTHROW_RPC,__localdep_execvp,(char const *__restrict __file, __TARGV),_execvp,(__file,___argv))
#else /* LIBC: execvp */
#undef ____localdep_execvp_defined
#endif /* execvp... */
#endif /* !____localdep_execvp_defined */

__NAMESPACE_LOCAL_BEGIN
/* >> execlp(3)
 * Replace the calling process with the application image referred to by `PATH' / `FILE'
 * and execute it's `main()' method, passing the list of NULL-terminated `ARGS'-list */
__LOCAL_LIBC(execlp) __ATTR_SENTINEL __ATTR_NONNULL((1)) int
__NOTHROW_RPC(__VLIBCCALL __LIBC_LOCAL_NAME(execlp))(char const *__restrict __file,
                                                     char const *__args,
                                                     ... /*, (char *)NULL*/) {
#line 284 "kos/src/libc/magic/unistd.c"
	__REDIRECT_EXECL(char, __localdep_execvp, __file, __args)
}
__NAMESPACE_LOCAL_END
#endif /* __CRT_HAVE_execvp || __CRT_HAVE__execvp */
#endif /* !__local_execlp_defined */
