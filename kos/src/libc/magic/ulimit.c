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

%[define_replacement(fd_t = __fd_t)]

%{
#include <features.h>

__SYSDECL_BEGIN

/* Some documentation comments are taken from /usr/include/ulimit.h on a linux machine.
 * The following copyright notice can be found at the top of that file: */
/* Copyright (C) 1997-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */



/* Constants used as the first parameter for `ulimit'.
 * They denote limits which can be set or retrieved using this function. */
}%[enum @macro @undef {
	UL_GETFSIZE     = 1, /* Return limit on the size of a file, in units of 512 bytes. */
	UL_SETFSIZE     = 2, /* Set limit on the size of a file to second argument. */
	__UL_GETMAXBRK  = 3, /* Return the maximum possible address of the data segment. */
	__UL_GETOPENMAX = 4  /* Return the maximum number of files that the calling process can open. */
}]%{

#ifdef __CC__
}

@@Control process limits according to CMD
[vartypes(long)] ulimit:(int cmd, ...) -> long int;

%{
#endif /* __CC__ */

__SYSDECL_END

}