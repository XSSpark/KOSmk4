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

%[define_replacement(fd_t    = __fd_t)]
%[define_replacement(pid_t   = __pid_t)]
%[define_replacement(off_t   = __FS_TYPE(off))]
%[define_replacement(pos_t   = __FS_TYPE(pos))]
%[define_replacement(off32_t = __off32_t)]
%[define_replacement(off64_t = __off64_t)]
%[define_replacement(pos32_t = __pos32_t)]
%[define_replacement(pos64_t = __pos64_t)]

%{
#include <features.h>

#include <bits/types.h>
#include <bits/uio.h>
#include <sys/types.h>

__SYSDECL_BEGIN

#ifdef __CC__
}

%
%#ifdef __USE_GNU
[[cp]][[ATTR_WUNUSED]][decl_include(<bits/uio.h>)]
process_vm_readv:($pid_t pid,
                  [inp_opt(liovcnt)] struct iovec const *local_iov, unsigned long int liovcnt,
                  [inp_opt(riovcnt)] struct iovec const *remote_iov, unsigned long int riovcnt,
                  unsigned long int flags) -> ssize_t;

[[cp]][decl_include(<bits/uio.h>)]
process_vm_writev:($pid_t pid,
                   [inp_opt(liovcnt)] struct iovec const *local_iov, unsigned long int liovcnt,
                   [inp_opt(riovcnt)] struct iovec const *remote_iov, unsigned long int riovcnt,
                   unsigned long int flags) -> ssize_t;
%#endif /* __USE_GNU */

%
[[cp]][[ATTR_WUNUSED]][decl_include(<bits/uio.h>)]
readv:($fd_t fd, [inp(count)] struct iovec const *iov, __STDC_INT_AS_SIZE_T count) -> ssize_t;

[[cp]][decl_include(<bits/uio.h>)]
writev:($fd_t fd, [inp(count)] struct iovec const *iov, __STDC_INT_AS_SIZE_T count) -> ssize_t;


%
%#ifdef __USE_MISC

[[cp]][ignore][decl_include(<bits/uio.h>)]
preadv32:($fd_t fd, [inp(count)] struct iovec const *iov,
          __STDC_INT_AS_SIZE_T count, $off32_t offset) -> ssize_t = preadv?;
[[cp]][ignore][decl_include(<bits/uio.h>)]
pwritev32:($fd_t fd, [inp(count)] struct iovec const *iov,
           __STDC_INT_AS_SIZE_T count, $off32_t offset) -> ssize_t = pwritev?;

[[cp]][[ATTR_WUNUSED]][decl_include(<bits/uio.h>), no_crt_self_import]
[if(defined(__USE_FILE_OFFSET64)), preferred_alias(preadv64)]
[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(preadv)]
[requires($has_function(preadv32) || $has_function(preadv64))]
preadv:($fd_t fd, [inp(count)] struct iovec const *iov, __STDC_INT_AS_SIZE_T count, $off_t offset) -> ssize_t {
@@if_has_function(preadv64)@@
	return preadv64(fd, iov, count, (off64_t)offset);
@@else_has_function(preadv64)@@
	return preadv32(fd, iov, count, (off32_t)offset);
@@endif_has_function(preadv64)@@
}

[[cp]][decl_include(<bits/uio.h>), no_crt_self_import]
[if(defined(__USE_FILE_OFFSET64)), preferred_alias(pwritev64)]
[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(pwritev)]
[requires($has_function(pwritev32) || $has_function(pwritev64))]
[requires(defined(__CRT_HAVE_pwritev) || defined(__CRT_HAVE_pwritev64))]
pwritev:($fd_t fd, [inp(count)] struct iovec const *iov, __STDC_INT_AS_SIZE_T count, $off_t offset) -> ssize_t {
@@if_has_function(pwritev64)@@
	return pwritev64(fd, iov, count, (off64_t)offset);
@@else_has_function(pwritev64)@@
	return pwritev32(fd, iov, count, (off32_t)offset);
@@endif_has_function(pwritev64)@@
}

%
%#ifdef __USE_LARGEFILE64
[off64_variant_of(preadv)]
[[cp]][[ATTR_WUNUSED]][requires($has_function(preadv32))]
preadv64:($fd_t fd, [inp(count)] struct iovec const *iov,
          __STDC_INT_AS_SIZE_T count, $off64_t offset) -> ssize_t {
	return preadv32(fd, iov, count, (off32_t)offset);
}
[off64_variant_of(pwritev)]
[[cp]][requires($has_function(pwritev32))]
pwritev64:($fd_t fd, [inp(count)] struct iovec const *iov,
           __STDC_INT_AS_SIZE_T count, $off64_t offset) -> ssize_t {
	return pwritev32(fd, iov, count, (off32_t)offset);
}
%#endif /* __USE_LARGEFILE64 */
%#endif /* !__USE_MISC */

%{

#endif /* __CC__ */

__SYSDECL_END

}