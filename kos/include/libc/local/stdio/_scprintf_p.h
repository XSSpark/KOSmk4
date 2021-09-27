/* HASH CRC-32:0xaf48e49d */
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
#ifndef __local__scprintf_p_defined
#define __local__scprintf_p_defined
#include <__crt.h>
#include <features.h>
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep__vscprintf_p_defined
#define __local___localdep__vscprintf_p_defined
#ifdef __CRT_HAVE__vscprintf_p
__CREDIRECT(__ATTR_WUNUSED __ATTR_LIBC_PRINTF_P(1, 0) __ATTR_NONNULL((1)),__STDC_INT_AS_SIZE_T,__NOTHROW_NCX,__localdep__vscprintf_p,(char const *__restrict __format, __builtin_va_list __args),_vscprintf_p,(__format,__args))
#else /* __CRT_HAVE__vscprintf_p */
__NAMESPACE_LOCAL_END
#include <libc/local/stdio/_vscprintf_p.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep__vscprintf_p __LIBC_LOCAL_NAME(_vscprintf_p)
#endif /* !__CRT_HAVE__vscprintf_p */
#endif /* !__local___localdep__vscprintf_p_defined */
__LOCAL_LIBC(_scprintf_p) __ATTR_WUNUSED __ATTR_LIBC_PRINTF_P(1, 2) __ATTR_NONNULL((1)) __STDC_INT_AS_SIZE_T
__NOTHROW_NCX(__VLIBCCALL __LIBC_LOCAL_NAME(_scprintf_p))(char const *__restrict __format, ...) {
	__STDC_INT_AS_SIZE_T __result;
	__builtin_va_list __args;
	__builtin_va_start(__args, __format);
	__result = (__NAMESPACE_LOCAL_SYM __localdep__vscprintf_p)(__format, __args);
	__builtin_va_end(__args);
	return __result;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep__scprintf_p_defined
#define __local___localdep__scprintf_p_defined
#define __localdep__scprintf_p __LIBC_LOCAL_NAME(_scprintf_p)
#endif /* !__local___localdep__scprintf_p_defined */
#endif /* !__local__scprintf_p_defined */
