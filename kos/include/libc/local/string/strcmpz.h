/* HASH CRC-32:0x2595b2a */
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
#ifndef __local_strcmpz_defined
#define __local_strcmpz_defined
#include <__crt.h>
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(strcmpz) __ATTR_PURE __ATTR_WUNUSED __ATTR_NONNULL((1, 2)) int
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(strcmpz))(char const *__lhs, char const *__rhs, __SIZE_TYPE__ __rhs_len) {
	char __c1, __c2;
	do {
		__c1 = *__lhs++;
		if (!__rhs_len--) {
			/* Once  RHS  reaches  the end  of  the string,
			 * compare the last character of LHS with `NUL' */
			return (int)((unsigned char)__c1 - '\0');
		}
		__c2 = *__rhs++;
		if __unlikely(__c1 != __c2)
			return (int)((unsigned char)__c1 - (unsigned char)__c2);
	} while (__c1);
	return 0;
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_strcmpz_defined
#define __local___localdep_strcmpz_defined
#define __localdep_strcmpz __LIBC_LOCAL_NAME(strcmpz)
#endif /* !__local___localdep_strcmpz_defined */
#endif /* !__local_strcmpz_defined */
