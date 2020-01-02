/* HASH CRC-32:0xa4e3d4b4 */
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
#ifndef __local_wcssep_defined
#define __local_wcssep_defined 1
/* Dependency: "wcschr" from "wchar" */
#ifndef ____localdep_wcschr_defined
#define ____localdep_wcschr_defined 1
#ifdef __std___localdep_wcschr_defined
__NAMESPACE_STD_USING(__localdep_wcschr)
#elif defined(__CRT_HAVE_wcschr)
/* Return the pointer of the first instance of `NEEDLE', or `NULL' if `NEEDLE' wasn't found. */
__CREDIRECT(__ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1)),__WCHAR_TYPE__ *,__NOTHROW_NCX,__localdep_wcschr,(__WCHAR_TYPE__ const *__restrict __haystack, __WCHAR_TYPE__ __needle),wcschr,(__haystack,__needle))
#else /* LIBC: wcschr */
#include <local/wchar/wcschr.h>
/* Return the pointer of the first instance of `NEEDLE', or `NULL' if `NEEDLE' wasn't found. */
#define __localdep_wcschr (__NAMESPACE_LOCAL_SYM __LIBC_LOCAL_NAME(wcschr))
#endif /* wcschr... */
#endif /* !____localdep_wcschr_defined */

__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(wcssep) __ATTR_LEAF __ATTR_NONNULL((1, 2)) __WCHAR_TYPE__ *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(wcssep))(__WCHAR_TYPE__ **__restrict __stringp,
                                                    __WCHAR_TYPE__ const *__restrict __delim) {
#line 832 "kos/src/libc/magic/string.c"
	__WCHAR_TYPE__ *__result, *__iter;
	if (!__stringp || (__result = *__stringp) == __NULLPTR || !*__result)
		return __NULLPTR;
	for (__iter = __result; *__iter && !__localdep_wcschr(__delim, *__iter); ++__iter)
		;
	if (*__iter)
		*__iter++ = '\0';
	*__stringp = __iter;
	return __result;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_wcssep_defined */
