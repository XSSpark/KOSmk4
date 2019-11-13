/* HASH CRC-32:0x9afc968d */
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
#ifndef __local_strtof_defined
#define __local_strtof_defined 1
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(strtof) __ATTR_LEAF __ATTR_NONNULL((1)) float
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(strtof))(char const *__restrict __nptr,
                                                    char **__endptr) {
#line 901 "kos/src/libc/magic/stdlib.c"
	/* TODO */
	__COMPILER_IMPURE();
	if (__endptr)
		*__endptr = (char *)__nptr;
	return 0;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_strtof_defined */
