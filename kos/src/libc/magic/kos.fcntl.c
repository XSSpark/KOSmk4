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
%[default_impl_section(.text.crt.except.io.utility)]

%{
#include <features.h>
#include <fcntl.h>

__SYSDECL_BEGIN

#ifdef __CC__

}

[vartypes(void *)][guard][decl_include(<bits/types.h>)][throws]
Fcntl:($fd_t fd, int cmd, ...) -> __STDC_INT_AS_SSIZE_T;

%[default_impl_section(.text.crt.except.io.access)]

[cp][ATTR_WUNUSED][noexport][vartypes($mode_t)][throws][ignore][decl_include(<bits/types.h>)]
Open32:([nonnull] char const *filename, $oflag_t oflags, ...) -> $fd_t = Open?;

[cp][guard][ATTR_WUNUSED][noexport][vartypes($mode_t)][throws]
[if(defined(__USE_FILE_OFFSET64)), preferred_alias(Open64)]
[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(Open)][decl_include(<bits/types.h>)]
[requires($has_function(Open64) || (defined(__CRT_AT_FDCWD) && $has_function(OpenAt)))]
Open:([nonnull] char const *filename, $oflag_t oflags, ...) -> $fd_t {
	$fd_t result;
	va_list args;
	va_start(args, oflags);
@@if_has_function(Open64)@@
	result = Open64(filename, oflags, va_arg(args, mode_t));
@@else_has_function(Open64)@@
	result = OpenAt(__CRT_AT_FDCWD, filename, oflags, va_arg(args, mode_t));
@@endif_has_function(Open64)@@
	va_end(args);
	return result;
}

[cp][guard][ATTR_WUNUSED][throws]
[if(defined(__USE_FILE_OFFSET64)), preferred_alias(Creat64)]
[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(Creat)]
[requires($has_function(Open))]
[dependency_include(<bits/fcntl.h>)][noexport]
[decl_include(<bits/types.h>)]
Creat:([nonnull] char const *filename, $mode_t mode) -> $fd_t {
	return Open(filename, @O_CREAT@ | @O_WRONLY@ | @O_TRUNC@, mode);
}

%
%#ifdef __USE_LARGEFILE64
[cp][noexport][vartypes($mode_t)][throws]
[ATTR_WUNUSED][largefile64_variant_of(Open)]
[requires((defined(__CRT_AT_FDCWD) && $has_function(OpenAt64)) || $has_function(Open32))]
[dependency_include(<bits/fcntl.h>)]
[decl_include(<bits/types.h>)]
Open64:([nonnull] char const *filename, $oflag_t oflags, ...) -> $fd_t {
	$fd_t result;
	va_list args;
	va_start(args, oflags);
#if defined(__CRT_AT_FDCWD) && @@has_function(OpenAt64)@@
	result = OpenAt64(__CRT_AT_FDCWD, filename, oflags, va_arg(args, mode_t));
#else /* __CRT_AT_FDCWD && OpenAt64... */
#ifdef @__O_LARGEFILE@
	result = Open32(filename, oflags|@__O_LARGEFILE@, va_arg(args, mode_t));
#else /* __O_LARGEFILE */
	result = Open32(filename, oflags, va_arg(args, mode_t));
#endif /* !__O_LARGEFILE */
#endif /* !__CRT_AT_FDCWD || !OpenAt64... */
	va_end(args);
	return result;
}

[cp][noexport][guard]
[ATTR_WUNUSED][largefile64_variant_of(Creat)][throws]
[requires($has_function(Open64))]
[dependency_include(<bits/fcntl.h>)][decl_include(<bits/types.h>)]
Creat64:([nonnull] char const *filename, $mode_t mode) -> $fd_t {
	return Open64(filename, @O_CREAT@ | @O_WRONLY@ | @O_TRUNC@, mode);
}
%#endif /* __USE_LARGEFILE64 */
%
%#ifdef __USE_ATFILE

[cp][ignore][ATTR_WUNUSED][vartypes($mode_t)][decl_include(<bits/types.h>)]
OpenAt32:($fd_t dirfd, [nonnull] char const *filename, $oflag_t oflags, ...) -> $fd_t = OpenAt?;

[cp][guard][noexport][vartypes($mode_t)][ATTR_WUNUSED]
[if(defined(__USE_FILE_OFFSET64)), preferred_alias(Openat64)]
[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(OpenAt)]
[requires($has_function(OpenAt32) || $has_function(OpenAt64))][throws]
[dependency_include(<bits/fcntl.h>)]
[decl_include(<bits/types.h>)]
OpenAt:($fd_t dirfd, [nonnull] char const *filename, $oflag_t oflags, ...) -> $fd_t {
	$fd_t result;
	va_list args;
	va_start(args, oflags);
@@if_has_function(OpenAt64)@@
	result = OpenAt64(dirfd, filename, oflags, va_arg(args, mode_t));
@@else_has_function(OpenAt64)@@
	result = OpenAt32(dirfd, filename, oflags, va_arg(args, mode_t));
@@endif_has_function(OpenAt64)@@
	va_end(args);
	return result;
}
%#ifdef __USE_LARGEFILE64
[cp][guard][noexport][vartypes($mode_t)][ATTR_WUNUSED]
[largefile64_variant_of(OpenAt)][throws]
[requires($has_function(OpenAt32))]
[decl_include(<bits/types.h>)]
OpenAt64:($fd_t dirfd, [nonnull] char const *filename, $oflag_t oflags, ...) -> $fd_t {
	$fd_t result;
	va_list args;
	va_start(args, oflags);
#ifdef @__O_LARGEFILE@
	result = OpenAt32(dirfd, filename, oflags|@__O_LARGEFILE@, va_arg(args, mode_t));
#else /* __O_LARGEFILE */
	result = OpenAt32(dirfd, filename, oflags, va_arg(args, mode_t));
#endif /* !__O_LARGEFILE */
	va_end(args);
	return result;
}
%#endif /* __USE_LARGEFILE64 */
%#endif /* __USE_ATFILE */
%


%{

#endif /* __CC__ */

__SYSDECL_END

}