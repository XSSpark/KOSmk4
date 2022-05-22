/* HASH CRC-32:0x6deb0c8e */
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
#ifndef __local_ether_aton_defined
#define __local_ether_aton_defined
#include <__crt.h>
struct ether_addr;
__NAMESPACE_LOCAL_BEGIN
#ifndef __local___localdep_ether_aton_r_defined
#define __local___localdep_ether_aton_r_defined
#ifdef __CRT_HAVE_ether_aton_r
__CREDIRECT(__ATTR_WUNUSED __ATTR_ACCESS_RO(1) __ATTR_ACCESS_WR(2),struct ether_addr *,__NOTHROW_NCX,__localdep_ether_aton_r,(char const *__restrict __asc, struct ether_addr *__restrict __addr),ether_aton_r,(__asc,__addr))
#else /* __CRT_HAVE_ether_aton_r */
__NAMESPACE_LOCAL_END
#include <libc/local/netinet.ether/ether_aton_r.h>
__NAMESPACE_LOCAL_BEGIN
#define __localdep_ether_aton_r __LIBC_LOCAL_NAME(ether_aton_r)
#endif /* !__CRT_HAVE_ether_aton_r */
#endif /* !__local___localdep_ether_aton_r_defined */
__NAMESPACE_LOCAL_END
#include <net/ethernet.h>
__NAMESPACE_LOCAL_BEGIN
__LOCAL_LIBC(ether_aton) __ATTR_RETNONNULL __ATTR_WUNUSED __ATTR_ACCESS_RO(1) struct ether_addr *
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(ether_aton))(char const *__restrict __asc) {
	static struct ether_addr __addr;
	return (__NAMESPACE_LOCAL_SYM __localdep_ether_aton_r)(__asc, &__addr);
}
__NAMESPACE_LOCAL_END
#ifndef __local___localdep_ether_aton_defined
#define __local___localdep_ether_aton_defined
#define __localdep_ether_aton __LIBC_LOCAL_NAME(ether_aton)
#endif /* !__local___localdep_ether_aton_defined */
#endif /* !__local_ether_aton_defined */
