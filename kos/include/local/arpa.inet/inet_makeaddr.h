/* HASH CRC-32:0x23bdc8c6 */
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
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2019-2020 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef __local_inet_makeaddr_defined
#define __local_inet_makeaddr_defined 1
#include <netinet/in.h>

#include <hybrid/__byteswap.h>
__NAMESPACE_LOCAL_BEGIN
/* Make Internet host address in network byte order by
 * combining the network number NET with the local address HOST */
__LOCAL_LIBC(inet_makeaddr) __ATTR_CONST __ATTR_WUNUSED struct in_addr
__NOTHROW_NCX(__LIBCCALL __LIBC_LOCAL_NAME(inet_makeaddr))(__UINT32_TYPE__ __net,
                                                           __UINT32_TYPE__ __host) {
#line 96 "kos/src/libc/magic/arpa.inet.c"
	struct in_addr __result;
	__UINT32_TYPE__ __result_addr;
	if (__net < IN_CLASSA_MAX)
		__result_addr = (__net << IN_CLASSA_NSHIFT) | (__host & IN_CLASSA_HOST);
	else if (__net < IN_CLASSB_MAX)
		__result_addr = (__net << IN_CLASSB_NSHIFT) | (__host & IN_CLASSB_HOST);
	else if (__net < IN_CLASSC_MAX)
		__result_addr = (__net << IN_CLASSC_NSHIFT) | (__host & IN_CLASSC_HOST);
	else {
		__result_addr = __net | __host;
	}
	__result.s_addr = __hybrid_htobe32(__result_addr);
	return __result;
}
__NAMESPACE_LOCAL_END
#endif /* !__local_inet_makeaddr_defined */
