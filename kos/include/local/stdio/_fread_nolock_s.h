/* HASH CRC-32:0x94c0e9f */
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
#ifndef __local__fread_nolock_s_defined
#if defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE_fread_s) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && defined(__CRT_HAVE__filbuf)) || defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc)
#define __local__fread_nolock_s_defined 1
#include <kos/anno.h>
/* Dependency: "fread_unlocked" from "stdio" */
#ifndef ____localdep_fread_unlocked_defined
#define ____localdep_fread_unlocked_defined 1
#if defined(__CRT_HAVE_fread_unlocked)
/* Same as `fread()', but performs I/O without acquiring a lock to `STREAM' */
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 4)),__SIZE_TYPE__,,__localdep_fread_unlocked,(void *__restrict __buf, __SIZE_TYPE__ __elemsize, __SIZE_TYPE__ __elemcount, __FILE *__restrict __stream),fread_unlocked,(__buf,__elemsize,__elemcount,__stream)) __THROWS(...)
#elif defined(__CRT_HAVE__fread_nolock)
/* Same as `fread()', but performs I/O without acquiring a lock to `STREAM' */
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 4)),__SIZE_TYPE__,,__localdep_fread_unlocked,(void *__restrict __buf, __SIZE_TYPE__ __elemsize, __SIZE_TYPE__ __elemcount, __FILE *__restrict __stream),_fread_nolock,(__buf,__elemsize,__elemcount,__stream)) __THROWS(...)
#elif defined(__CRT_HAVE_fread)
/* Same as `fread()', but performs I/O without acquiring a lock to `STREAM' */
__CREDIRECT(__ATTR_WUNUSED __ATTR_NONNULL((1, 4)),__SIZE_TYPE__,,__localdep_fread_unlocked,(void *__restrict __buf, __SIZE_TYPE__ __elemsize, __SIZE_TYPE__ __elemcount, __FILE *__restrict __stream),fread,(__buf,__elemsize,__elemcount,__stream)) __THROWS(...)
#elif defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) || defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE_fread_s)
#include <local/stdio/fread_unlocked.h>
/* Same as `fread()', but performs I/O without acquiring a lock to `STREAM' */
#define __localdep_fread_unlocked (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(fread_unlocked))
#else /* CUSTOM: fread_unlocked */
#undef ____localdep_fread_unlocked_defined
#endif /* fread_unlocked... */
#endif /* !____localdep_fread_unlocked_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(_fread_nolock_s) __ATTR_WUNUSED __ATTR_NONNULL((1, 5)) __SIZE_TYPE__
(__LIBCCALL __LIBC_LOCAL_NAME(_fread_nolock_s))(void *__restrict __buf,
                                                __SIZE_TYPE__ __bufsize,
                                                __SIZE_TYPE__ __elemsize,
                                                __SIZE_TYPE__ __elemcount,
                                                __FILE *__restrict __stream) __THROWS(...) {
#line 2674 "kos/src/libc/magic/stdio.c"
	__bufsize = __elemsize ? __bufsize / __elemsize : 0;
	return __localdep_fread_unlocked(__buf, __elemsize, __bufsize < __elemcount ? __bufsize : __elemcount, __stream);
}
__NAMESPACE_LOCAL_END
#endif /* defined(__CRT_HAVE_fread_unlocked) || defined(__CRT_HAVE__fread_nolock) || defined(__CRT_HAVE_fread) || defined(__CRT_HAVE_fread_s) || defined(__CRT_HAVE_fgetc_unlocked) || defined(__CRT_HAVE_getc_unlocked) || (defined(__CRT_DOS) && defined(__CRT_HAVE__filbuf)) || defined(__CRT_HAVE_fgetc) || defined(__CRT_HAVE_getc) */
#endif /* !__local__fread_nolock_s_defined */
