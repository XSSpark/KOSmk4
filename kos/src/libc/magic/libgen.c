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

%[define_replacement(fd_t = __fd_t)]

%{
#include <features.h>

__SYSDECL_BEGIN

#ifdef __CC__

}

@@Return directory part of PATH or "." if none is available
dirname:(char *path) -> [[nonnull]] char * {
	/* NOTE: This implementation is taken from GLibc */
	/* dirname - return directory part of PATH.
	   Copyright (C) 1996-2017 Free Software Foundation, Inc.
	   This file is part of the GNU C Library.
	   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1996.
	
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
	char *last_slash;
	last_slash = path ? strrchr(path, '/') : NULL;
	if (last_slash && last_slash != path && last_slash[1] == '\0') {
		char *runp;
		for (runp = last_slash; runp != path; --runp) {
			if (runp[-1] != '/')
				break;
		}
		if (runp != path)
			last_slash = (char *)memrchr(path, '/', (size_t)(runp - path));
	}
	if (last_slash) {
		char *runp;
		for (runp = last_slash; runp != path; --runp) {
			if (runp[-1] != '/')
				break;
		}
		if (runp == path) {
			if (last_slash == path + 1)
				++last_slash;
			else {
				last_slash = path + 1;
			}
		} else {
			last_slash = runp;
		}
		last_slash[0] = '\0';
	} else {
		path = (char *)".";
	}
	return path;
}

@@Return final component of PATH.
@@This is the weird XPG version of this function. It sometimes will
@@modify its argument. Therefore we normally use the GNU version (in
@@<string.h>) and only if this header is included make the XPG
@@version available under the real name
__xpg_basename:(char *filename) -> [[nonnull]] char * {
	/* NOTE: This implementation is taken from GLibc */
	/* Return basename of given pathname according to the weird XPG specification.
	   Copyright (C) 1997-2017 Free Software Foundation, Inc.
	   This file is part of the GNU C Library.
	   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.
	
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
	char *result;
	if (!filename || !filename[0])
		result = (char *)".";
	else {
		result = strrchr(filename, '/');
		if (!result)
			result = filename;
		else if (!result[1]) {
			while (result > filename && result[-1] == '/')
				--result;
			if (result > filename) {
				*result-- = '\0';
				while (result > filename && result[-1] != '/')
					--result;
			} else {
				while (result[1] != '\0')
					++result;
			}
		} else {
			++result;
		}
	}
	return result;
}

%#define basename(path) __xpg_basename(path)


%{

#endif /* __CC__ */

__SYSDECL_END

}