/* HASH CRC-32:0xd0b072a5 */
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
#ifndef __local_fmtcheck_defined
#define __local_fmtcheck_defined
#include <__crt.h>
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(fmtcheck) __ATTR_RETNONNULL __ATTR_WUNUSED __ATTR_ACCESS_RO(2) __ATTR_ACCESS_RO_OPT(1) __ATTR_FORMAT_ARG(2) char const *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(fmtcheck))(char const *__user_format, char const *__good_format) {
	/* TODO: Implement properly */
	__COMPILER_IMPURE();
	(void)__good_format;
	return __user_format;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_fmtcheck_defined
#define __local___localdep_fmtcheck_defined
#define __localdep_fmtcheck __LIBC_LOCAL_NAME(fmtcheck)
#endif /* !__local___localdep_fmtcheck_defined */
#endif /* !__local_fmtcheck_defined */
