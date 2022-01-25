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
%(c_prefix){
/* (#) Portability: Windows Kits (/ucrt/corecrt_wio.h) */
}

%[define_replacement(fd_t = __fd_t)]
%[define_replacement(oflag_t = __oflag_t)]

%[insert:prefix(
#include <features.h>
)]%[insert:prefix(
#include <hybrid/typecore.h>
)]%[insert:prefix(
#include <bits/types.h>
)]%[insert:prefix(
#include <asm/os/fcntl.h>
)]%[insert:prefix(
#include <asm/os/oflags.h>
)]%{

#ifdef __CC__
__SYSDECL_BEGIN

#ifndef __errno_t_defined
#define __errno_t_defined
typedef __errno_t errno_t;
#endif /* !__errno_t_defined */

#ifndef __size_t_defined
#define __size_t_defined
typedef __size_t size_t;
#endif /* !__size_t_defined */

#ifndef __ssize_t_defined
#define __ssize_t_defined
typedef __ssize_t ssize_t;
#endif /* !__ssize_t_defined */

#ifndef __intptr_t_defined
#define __intptr_t_defined
typedef __intptr_t intptr_t;
#endif /* !__intptr_t_defined */

#ifndef _FSIZE_T_DEFINED
#define _FSIZE_T_DEFINED
typedef __UINT32_TYPE__ _fsize_t;
#endif /* _FSIZE_T_DEFINED */

struct _wfinddata32_t;
struct _wfinddata64_t;
struct _wfinddata32i64_t;
struct _wfinddata64i32_t;

}


%[insert:function(_waccess = waccess)]

[[wchar, cp, wunused, decl_include("<features.h>")]]
[[section(".text.crt.dos.wchar.fs.property")]]
[[requires_function(waccess), impl_include("<libc/errno.h>")]]
errno_t _waccess_s([[nonnull]] wchar_t const *file, __STDC_INT_AS_UINT_T type) {
	if (waccess(file, type) != 0)
		return $__libc_geterrno_or(1);
	return 0;
}


%[insert:function(_wchmod = wchmod)]
%[insert:function(_wcreat = wcreat)]
%[insert:function(_wunlink = wunlink)]
%[insert:function(_wrename = wrename)]


%[default:section(".text.crt.dos.wchar.fs.utility")]

[[wchar]]
errno_t _wmktemp_s([[nonnull]] wchar_t *template_, size_t bufsize);

[[wchar, nonnull]]
wchar_t *_wmktemp([[nonnull]] wchar_t *template_);


%[default:section(".text.crt.dos.fs.io")]
%
[[wchar, cp]]
[[decl_include("<bits/types.h>")]]
[[impl_include("<libc/errno.h>")]]
[[requires_function(_wsopen)]]
errno_t _wsopen_s([[nonnull]] $fd_t *fd,
                  [[nonnull]] wchar_t const *filename,
                  $oflag_t oflags, int sflags,
                  $mode_t mode) {
	fd_t result;
	if (!fd) {
@@pp_ifdef EINVAL@@
		return $EINVAL;
@@pp_else@@
		return 1;
@@pp_endif@@
	}
	result = _wsopen(filename, oflags, sflags, mode);
	if (result < 0)
		return $__libc_geterrno_or(1);
	*fd = result;
	return 0;
}

[[wchar, cp, wunused, decl_include("<bits/types.h>")]]
[[requires_function(_wsopen_s), section(".text.crt.dos.wchar.fs.io")]]
errno_t _wsopen_dispatch([[nonnull]] wchar_t const *filename,
                         $oflag_t oflags, int sflags, $mode_t mode,
                         [[nonnull]] $fd_t *fd, int bsecure) {
	(void)bsecure;
	return _wsopen_s(fd, filename, oflags, sflags, mode);
}


%[insert:function(_wopen = wopen)]

[[wchar, cp, vartypes($mode_t), wunused, decl_include("<bits/types.h>")]]
[[requires_function(wopen), section(".text.crt.dos.wchar.fs.io")]]
$fd_t _wsopen([[nonnull]] wchar_t const *filename, $oflag_t oflags, int sflags, ...) {
	fd_t result;
	va_list args;
	va_start(args, sflags);
	(void)sflags;
	result = wopen(filename, oflags, va_arg(args, mode_t));
	va_end(args);
	return result;
}


[[wchar, cp, wunused, export_alias("_wfindfirst"), decl_include("<hybrid/typecore.h>")]]
intptr_t _wfindfirst32([[nonnull]] wchar_t const *__restrict filename,
                       [[nonnull]] struct _wfinddata32_t *__restrict finddata);

[[wchar, cp, wunused, export_alias("_wfindfirsti64"), decl_include("<hybrid/typecore.h>")]]
intptr_t _wfindfirst32i64([[nonnull]] wchar_t const *__restrict filename,
                          [[nonnull]] struct _wfinddata32i64_t *__restrict finddata);

[[wchar, cp, wunused, export_alias("_wfindfirst64i32"), decl_include("<hybrid/typecore.h>")]]
intptr_t _wfindfirst64([[nonnull]] wchar_t const *__restrict filename,
                       [[nonnull]] struct _wfinddata64_t *__restrict finddata);

[[/*wchar,*/ cp, wunused, decl_include("<hybrid/typecore.h>")]]
intptr_t _wfindfirst64i32([[nonnull]] wchar_t const *__restrict filename,
                          [[nonnull]] struct _wfinddata64i32_t *__restrict finddata)
	= _wfindfirst64;

[[wchar, cp, export_alias("_wfindnext"), decl_include("<hybrid/typecore.h>")]]
int _wfindnext32(intptr_t findfd,
                 [[nonnull]] struct _wfinddata32_t *__restrict finddata);

[[wchar, cp, export_alias("_wfindnexti64"), decl_include("<hybrid/typecore.h>")]]
int _wfindnext32i64(intptr_t findfd,
                    [[nonnull]] struct _wfinddata32i64_t *__restrict finddata);

[[wchar, cp, export_alias("_wfindnext64i32"), decl_include("<hybrid/typecore.h>")]]
int _wfindnext64(intptr_t findfd,
                 [[nonnull]] struct _wfinddata64_t *__restrict finddata);

[[/*wchar,*/ cp, decl_include("<hybrid/typecore.h>")]]
int _wfindnext64i32(intptr_t findfd,
                    [[nonnull]] struct _wfinddata64i32_t *__restrict finddata)
	= _wfindnext64;


%{

#ifndef _WFINDDATA_T_DEFINED
#define _WFINDDATA_T_DEFINED 1
/* Safely first! */
}%[push_macro @undef { attrib time_create time_access time_write size name }]%{
struct _wfinddata32_t {
	__UINT32_TYPE__      attrib;
	__INT32_TYPE__       time_create;
	__INT32_TYPE__       time_access;
	__INT32_TYPE__       time_write;
	_fsize_t             size;
	wchar_t              name[260];
};
struct _wfinddata32i64_t {
	__UINT32_TYPE__      attrib;
	__INT32_TYPE__       time_create;
	__INT32_TYPE__       time_access;
	__INT32_TYPE__       time_write;
	__INT64_TYPE__       size;
	wchar_t              name[260];
};
struct _wfinddata64i32_t {
	__UINT32_TYPE__      attrib;
	__INT64_TYPE__       time_create;
	__INT64_TYPE__       time_access;
	__INT64_TYPE__       time_write;
	union {
		_fsize_t         size;
		__INT64_TYPE__ __pad; /* I think this is correct? */
	};
	wchar_t              name[260];
};
struct _wfinddata64_t {
	__UINT32_TYPE__      attrib;
	__INT64_TYPE__       time_create;
	__INT64_TYPE__       time_access;
	__INT64_TYPE__       time_write;
	__INT64_TYPE__       size;
	wchar_t              name[260];
};
}%[pop_macro]%{

#ifdef __USE_TIME_BITS64
#define _wfinddata_t                       _wfinddata64i32_t
#define _wfinddatai64_t                    _wfinddata64_t
#define _wfindfirst(filename, finddata)    _wfindfirst64i32(filename, finddata)
#define _wfindnext(findfd, finddata)       _wfindnext64i32(findfd, finddata)
#define _wfindfirsti64(filename, finddata) _wfindfirst64(filename, finddata)
#define _wfindnexti64(findfd, finddata)    _wfindnext64(findfd, finddata)
#else /* __USE_TIME_BITS64 */
#define _wfinddata_t                       _wfinddata32_t
#define _wfinddatai64_t                    _wfinddata32i64_t
#define _wfindfirst(filename, finddata)    _wfindfirst32(filename, finddata)
#define _wfindnext(findfd, finddata)       _wfindnext32(findfd, finddata)
#define _wfindfirsti64(filename, finddata) _wfindfirst32i64(filename, finddata)
#define _wfindnexti64(findfd, finddata)    _wfindnext32i64(findfd, finddata)
#endif /* !__USE_TIME_BITS64 */
#endif /* !_WFINDDATA_T_DEFINED */

__SYSDECL_END
#endif /* __CC__ */

#ifdef __USE_UTF
#if defined(_UCHAR_H) && !defined(_PARTS_UCHAR_IO_H)
#include <parts/uchar/io.h>
#endif /* _UCHAR_H && !_PARTS_UCHAR_IO_H */
#endif /* __USE_UTF */

}