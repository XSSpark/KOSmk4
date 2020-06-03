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

%[crt_feature(environ, _environ, __environ)]
%[crt_feature(__peb, __argc, __argv, _pgmptr)]
%[crt_feature(program_invocation_name, __progname_full)]
%[crt_feature(program_invocation_short_name, __progname)]


%{
#include <features.h>
#include <unistd.h>

__SYSDECL_BEGIN

#ifdef __CC__

#ifndef __pos_t_defined
#define __pos_t_defined 1
typedef __FS_TYPE(pos) pos_t; /* File/device position */
#endif /* !__pos_t_defined */

#ifdef __USE_LARGEFILE64
#ifndef __pos64_t_defined
#define __pos64_t_defined 1
typedef __pos64_t pos64_t; /* File/device position */
#endif /* !__pos64_t_defined */
#endif /* __USE_LARGEFILE64 */

}

%[default_impl_section(.text.crt.except.fs.exec.exec)]

[throws][cp][ATTR_NORETURN][doc_alias(execv)]   Execv:([[nonnull]] char const *__restrict path, [[nonnull]] @__TARGV@);
[throws][cp][ATTR_NORETURN][doc_alias(execve)]  Execve:([[nonnull]] char const *__restrict path, [[nonnull]] @__TARGV@, [[nonnull]] @__TENVP@);
[throws][cp][ATTR_NORETURN][doc_alias(execvp)]  Execvp:([[nonnull]] char const *__restrict file, [[nonnull]] @__TARGV@);

[throws][cp][dependency_include(<parts/redirect-exec.h>)][doc_alias(execl)]
[requires_dependency(Execv)][ATTR_SENTINEL][ATTR_NORETURN][allow_macros]
Execl:([[nonnull]] char const *__restrict path, char const *args, ... /*, (char *)NULL*/) {
	__REDIRECT_XEXECL(char, Execv, path, args)
}

[throws][cp][dependency_include(<parts/redirect-exec.h>)][doc_alias(execle)]
[requires_dependency(Execve)][ATTR_SENTINEL_O(1)][ATTR_NORETURN][allow_macros]
Execle:([[nonnull]] char const *__restrict path, char const *args, ... /*, (char *)NULL, (char **)environ*/) {
	__REDIRECT_XEXECLE(char, Execve, path, args)
}

[throws][cp][dependency_include(<parts/redirect-exec.h>)][doc_alias(execlp)]
[requires_dependency(Execvp)][ATTR_SENTINEL][ATTR_NORETURN][allow_macros]
Execpl:([[nonnull]] char const *__restrict file, char const *args, ... /*, (char *)NULL*/) {
	__REDIRECT_XEXECL(char, Execvp, file, args)
}

[throws][cp][dependency_include(<parts/redirect-exec.h>)][doc_alias(execle)]
[requires_dependency(Execvpe)][ATTR_SENTINEL_O(1)][ATTR_NORETURN][allow_macros]
Execlpe:([[nonnull]] char const *__restrict file, char const *args, ... /*, (char *)NULL, (char **)environ*/) {
	__REDIRECT_XEXECLE(char, Execvpe, file, args)
}


%
[throws][doc_alias(pipe)]
[section(.text.crt.except.io.access)]
Pipe:([[nonnull]] $fd_t pipedes[2]);


%
[doc_alias(fsync)][throws][cp][userimpl]
[alias(FDataSync)][section(.text.crt.except.io.sync)]
FSync:($fd_t fd) {
	(void)fd;
	/* No-Op */
}


%[default_impl_section(.text.crt.except.sched.process)]

%
[doc_alias(setpgid)][throws]
SetPGid:($pid_t pid, $pid_t pgid);


%
[doc_alias(setsid)][throws]
SetSid:() -> $pid_t;

%
[doc_alias(setuid)][throws]
SetUid:($uid_t uid);

%
[doc_alias(setgid)][throws]
SetGid:($gid_t gid);

%
[doc_alias(fork)][ATTR_WUNUSED][throws]
[section(.text.crt.except.sched.access)]
Fork:() -> $pid_t;

%
[section(.text.crt.except.fs.property)]
[throws][ATTR_WUNUSED][doc_alias(fpathconf)]
FPathConf:($fd_t fd, int name) -> long int;


%[default_impl_section(.text.crt.except.fs.modify)]


%
[throws][cp][noexport][doc_alias(chown)]
[requires(defined(__CRT_AT_FDCWD) && $has_function(FChownAt))]
Chown:([[nonnull]] char const *file, $uid_t owner, $gid_t group) {
	FChownAt(__CRT_AT_FDCWD, file, owner, group, 0);
}

%
[throws][cp][noexport][doc_alias(pathconf)]
[section(.text.crt.except.fs.property)]
PathConf:([[nonnull]] char const *path, int name) -> long int;

%
[throws][cp][noexport][doc_alias(link)]
[requires(defined(__CRT_AT_FDCWD) && $has_function(LinkAt))]
Link:([[nonnull]] char const *from, [[nonnull]] char const *to) {
	LinkAt(__CRT_AT_FDCWD, from, __CRT_AT_FDCWD, to, 0);
}

%
[throws][doc_alias(read)][cp][section(.text.crt.except.io.read)]
Read:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize) -> size_t;

%
[throws][doc_alias(write)][cp][section(.text.crt.except.io.write)]
Write:($fd_t fd, [inp(bufsize)] void const *buf, size_t bufsize) -> size_t;

%
%#ifdef __USE_KOS
[throws][doc_alias(readall)][cp][section(.text.crt.except.io.read)]
[requires($has_function(Read) && $has_function(lseek))]
[dependency_include(<parts/errno.h>)]
[dependency_include(<kos/except.h>)]
ReadAll:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize) -> size_t {
	size_t result, temp;
	result = Read(fd, buf, bufsize);
	if (result != 0 && result < bufsize) {
		/* Keep on reading */
		for (;;) {
			@TRY@ {
				temp = Read(fd,
				           (byte_t *)buf + (size_t)result,
				            bufsize - (size_t)result);
			} @EXCEPT@ {
#ifdef @__errno@
				int old_error = @__errno@;
#endif
				/* Try to un-read data that had already been loaded. */
				lseek(fd, -(off_t)(pos_t)result, SEEK_CUR);
#ifdef @__errno@
				@__errno@ = old_error;
#endif
				@RETHROW@();
			}
			if (!temp) {
				result = 0;
				break;
			}
			result += temp;
			if (result >= bufsize)
				break;
		}
	}
	return result;
}
%#endif /* __USE_KOS */


[throws][ignore][section(.text.crt.except.io.seek)][doc_alias(lseek32)]
LSeek32:($fd_t fd, $off32_t offset, int whence) -> $pos32_t = LSeek?;

%
[[throws, doc_alias(lseek), no_crt_self_import]]
[[if(defined(__USE_FILE_OFFSET64)), preferred_alias(LSeek64)]]
[[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(LSeek)]]
[[userimpl, requires($has_function(LSeek32) || $has_function(LSeek64))]]
[[section(.text.crt.except.io.seek)]]
LSeek:($fd_t fd, $off_t offset, int whence) -> $pos_t {
@@if_has_function(LSeek32)@@
	return LSeek32(fd, ($off32_t)offset, whence);
@@else_has_function(LSeek32)@@
	return LSeek64(fd, ($off64_t)offset, whence);
@@endif_has_function(LSeek32)@@
}

%
[throws][section(.text.crt.except.io.access)][doc_alias(dup2)]
Dup2:($fd_t oldfd, $fd_t newfd) -> $fd_t;

%
[section(.text.crt.except.io.access)][doc_alias(dup)]
[ATTR_WUNUSED] Dup:($fd_t fd) -> $fd_t;

%
[throws][cp][doc_alias(chdir)]
[section(.text.crt.except.fs.basic_property)]
Chdir:([[nonnull]] char const *path);

%
[throws][cp][doc_alias(getcwd)]
[section(.text.crt.except.fs.basic_property)]
GetCwd:([outp_opt(bufsize)] char *buf, size_t bufsize) -> char *;

%[default_impl_section(.text.crt.fs.modify)]

%
[throws][cp][doc_alias(unlink)][noexport]
[requires(defined(__CRT_AT_FDCWD) && $has_function(UnlinkAt))]
Unlink:([[nonnull]] char const *file) {
	UnlinkAt(__CRT_AT_FDCWD, file, 0);
}

%
[throws][cp][doc_alias(rmdir)][noexport]
[requires(defined(__CRT_AT_FDCWD) && $has_function(UnlinkAt))]
Rmdir:([[nonnull]] char const *path) {
	UnlinkAt(__CRT_AT_FDCWD, path, 0x0200); /* AT_REMOVEDIR */
}

%
%#ifdef __USE_ATFILE
%[default_impl_section(.text.crt.except.fs.modify)]

%
[throws][cp][doc_alias(fchownat)]
FChownAt:($fd_t dfd, [[nonnull]] char const *file, $uid_t owner, $gid_t group, $atflag_t flags);

%
[throws][cp][doc_alias(linkat)]
LinkAt:($fd_t fromfd, [[nonnull]] char const *from, $fd_t tofd, [[nonnull]] char const *to, $atflag_t flags);

%
[throws][cp][doc_alias(symlinkat)]
SymlinkAt:([[nonnull]] char const *link_text, $fd_t tofd, [[nonnull]] char const *target_path);

%[default_impl_section(.text.crt.except.fs.property)]

%
[throws][cp][doc_alias(readlinkat)]
ReadlinkAt:($fd_t dfd, [[nonnull]] char const *__restrict path,
            [outp(buflen)] char *__restrict buf, size_t buflen) -> size_t;

%
%#ifdef __USE_KOS
[throws][cp][doc_alias(freadlinkat)]
FReadlinkAt:($fd_t dfd, [[nonnull]] char const *__restrict path,
             [outp(buflen)] char *__restrict buf, size_t buflen, $atflag_t flags) -> size_t;
%#endif /* __USE_KOS */

%[default_impl_section(.text.crt.except.fs.modify)]

%
[throws][cp][doc_alias(unlinkat)]
UnlinkAt:($fd_t dfd, [[nonnull]] char const *name, $atflag_t flags);
%#endif /* __USE_ATFILE */


%
%
%#ifdef __USE_LARGEFILE64
[throws][doc_alias(lseek64)]
[off64_variant_of(LSeek)][noexport][requires($has_function(LSeek32))]
[section(.text.crt.except.io.large.seek)]
LSeek64:($fd_t fd, $off64_t offset, int whence) -> $pos64_t {
	return LSeek32(fd, (__off32_t)offset, whence);
}
%#endif /* __USE_LARGEFILE64 */


%
%
%#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K8)

%
[[throws, cp, doc_alias(pread), no_crt_self_import]]
[[if(defined(__USE_FILE_OFFSET64)), preferred_alias(PRead64)]]
[[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(PRead)]]
[[section(.text.crt.except.io.read)]]
[[userimpl, requires($has_function(PRead32) || $has_function(PRead64))]]
PRead:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize, pos_t offset) -> size_t {
@@if_has_function(PRead32)@@
	return PRead32(fd, buf, bufsize, (pos32_t)offset);
@@else_has_function(PRead32)@@
	return PRead64(fd, buf, bufsize, (pos64_t)offset);
@@endif_has_function(PRead32)@@
}

[[throws, cp, doc_alias(pwrite), no_crt_self_import]]
[[if(defined(__USE_FILE_OFFSET64)), preferred_alias(PWrite64)]]
[[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(PWrite)]]
[[userimpl, requires($has_function(PWrite32) || $has_function(PWrite64))]]
[[section(.text.crt.except.io.write)]]
PWrite:($fd_t fd, [inp(bufsize)] void const *buf, size_t bufsize, pos_t offset) -> size_t {
@@if_has_function(PWrite32)@@
	return PWrite32(fd, buf, bufsize, (pos32_t)offset);
@@else_has_function(PWrite32)@@
	return PWrite64(fd, buf, bufsize, (pos64_t)offset);
@@endif_has_function(PWrite32)@@
}


%
%#ifdef __USE_KOS
[[throws, cp, doc_alias(preadall), no_crt_self_import]]
[[if(defined(__USE_FILE_OFFSET64)), preferred_alias(PReadAll64)]]
[[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(PReadAll)]]
[[userimpl, requires($has_function(PReadAll32) || $has_function(PReadAll64))]]
[[section(.text.crt.except.io.read)]]
PReadAll:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize, pos_t offset) -> size_t {
@@if_has_function(PReadAll32)@@
	return PReadAll32(fd, buf, bufsize, (pos32_t)offset);
@@else_has_function(PReadAll32)@@
	return PReadAll64(fd, buf, bufsize, (pos64_t)offset);
@@endif_has_function(PReadAll32)@@
}
%#endif /* __USE_KOS */

%
%#ifdef __USE_LARGEFILE64

[throws][section(.text.crt.except.io.read)][cp][ignore] PRead32:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize, $pos32_t offset) -> size_t = PRead?;
[throws][section(.text.crt.except.io.write)][cp][ignore] PWrite32:($fd_t fd, [inp(bufsize)] void const *buf, size_t bufsize, $pos32_t offset) -> size_t = PWrite?;

[throws][doc_alias(pread64)]
[off64_variant_of(PRead)][cp][noexport][section(.text.crt.except.io.large.read)]
[requires($has_function(PRead32))]
PRead64:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize, pos64_t offset) -> size_t {
	return PRead32(fd, buf, bufsize, (pos32_t)offset);
}

[throws][doc_alias(pwrite64)]
[off64_variant_of(PWrite)][cp][noexport][section(.text.crt.except.io.large.write)]
[requires($has_function(PWrite32))]
PWrite64:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize, pos64_t offset) -> size_t {
	return PWrite32(fd, buf, bufsize, (pos32_t)offset);
}

%
%#ifdef __USE_KOS
[throws][section(.text.crt.except.io.read)][cp][ignore]
PReadAll32:($fd_t fd, [outp(bufsize)] void *buf, size_t bufsize, $pos32_t offset) -> size_t = PReadAll?;

[throws][doc_alias(preadall64)]
[off64_variant_of(PReadAll)][cp][requires($has_function(PRead64))]
[section(.text.crt.except.io.large.read)]
PReadAll64:($fd_t fd, [inp(bufsize)] void *buf, size_t bufsize, pos64_t offset) -> size_t {
	size_t result, temp;
	result = PRead64(fd, buf, bufsize, offset);
	if (result != 0 && (size_t)result < bufsize) {
		/* Keep on reading */
		for (;;) {
			temp = PRead64(fd,
			              (byte_t *)buf + result,
			               bufsize - result,
			               offset + result);
			if (!temp) {
				result = 0;
				break;
			}
			result += temp;
			if (result >= bufsize)
				break;
		}
	}
	return result;
}
%#endif /* __USE_KOS */

%#endif /* __USE_LARGEFILE64 */
%#endif /* __USE_UNIX98 || __USE_XOPEN2K8 */

%
%
%#ifdef __USE_GNU

[doc_alias(pipe2)][throws][noexport][section(.text.crt.except.io.access)]
Pipe2:([[nonnull]] $fd_t pipedes[2], $oflag_t flags);

[doc_alias(dup3)][throws][noexport][section(.text.crt.except.io.access)]
Dup3:($fd_t oldfd, $fd_t newfd, $oflag_t flags) -> $fd_t;

[doc_alias(get_current_dir_name)]
[throws][cp][noexport][requires($has_function(GetCwd))]
[section(.text.crt.except.fs.basic_property)]
GetCurrentDirName:() -> [nonnull, malloc(?)] char * {
	return GetCwd(NULL, 0);
}

[throws][cp][user][noexport][section(.text.crt.except.fs.modify)]
SyncFs:($fd_t fd) {
	(void)fd;
	/* NO-OP */
}

%[default_impl_section(.text.crt.except.sched.user)]

GetResUid:($uid_t *ruid, $uid_t *euid, $uid_t *suid);
GetResGid:($gid_t *rgid, $gid_t *egid, $gid_t *sgid);
SetResUid:($uid_t ruid, $uid_t euid, $uid_t suid);
SetResGid:($gid_t rgid, $gid_t egid, $gid_t sgid);
%#endif /* __USE_GNU */

%#if (defined(__USE_XOPEN_EXTENDED) && !defined(__USE_XOPEN2K8)) || \
%     defined(__USE_MISC)
[throws][doc_alias(vfork)][section(.text.crt.sched.access)]
[ATTR_RETURNS_TWICE][ATTR_WUNUSED] VFork:() -> $pid_t;
%#endif


%
[throws][cp][section(.text.crt.except.fs.modify)][doc_alias(fchown)]
FChown:($fd_t fd, $uid_t owner, $gid_t group);

%
[throws][cp][section(.text.crt.except.fs.basic_property)][doc_alias(fchdir)]
FChdir:($fd_t fd);

%
[throws][section(.text.crt.except.sched.user)][doc_alias(getpgid)]
[ATTR_WUNUSED] GetPGid:($pid_t pid) -> $pid_t;

%
[throws][section(.text.crt.except.sched.process)][doc_alias(getsid)]
[ATTR_WUNUSED] GetSid:($pid_t pid) -> $pid_t;

%
[throws][cp][noexport][doc_alias(lchown)]
[requires(defined(__CRT_AT_FDCWD) && $has_function(FChownAt))]
[section(.text.crt.except.fs.modify)]
LChown:([[nonnull]] char const *file, $uid_t owner, $gid_t group) {
	FChownAt(__CRT_AT_FDCWD, file, owner, group, 0x0100); /* AT_SYMLINK_NOFOLLOW */
}


%
%#if defined(__USE_XOPEN_EXTENDED) || defined(__USE_XOPEN2K8)

%
[[throws, doc_alias(truncate), no_crt_self_import]]
[[if(defined(__USE_FILE_OFFSET64)), preferred_alias(Truncate64)]]
[[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(Truncate)]]
[[section(.text.crt.except.fs.modify)]]
[[userimpl, requires($has_function(Truncate32) || $has_function(Truncate64))]]
Truncate:([[nonnull]] char const *file, pos_t length) {
@@if_has_function(Truncate32)@@
	Truncate64(file, (__pos64_t)length);
@@else_has_function(Truncate32)@@
	Truncate32(file, (__pos32_t)length);
@@endif_has_function(Truncate32)@@
}

[throws][doc_alias(Truncate)][section(.text.crt.except.fs.modify)]
[ignore] Truncate32:([[nonnull]] char const *file, $pos32_t length) = Truncate?;

%
%#ifdef __USE_LARGEFILE64
[throws][doc_alias(truncate64)]
[off64_variant_of(Truncate)][noexport]
[section(.text.crt.except.fs.modify)]
[requires($has_function(Truncate32))]
Truncate64:([[nonnull]] char const *file, pos64_t length) {
	Truncate32(file, (__pos32_t)length);
}
%#endif /* __USE_LARGEFILE64 */
%#endif /* __USE_XOPEN_EXTENDED || __USE_XOPEN2K8 */
%

%#ifdef __USE_XOPEN2K8

%
[throws][cp][guard][doc_alias(fexecve)][ATTR_NORETURN]
[argument_names(fd, ___argv, ___envp)][section(.text.crt.except.fs.exec.exec)]
FExecve:($fd_t fd, [[nonnull]] @__TARGV@, [[nonnull]] @__TENVP@);

%#endif /* __USE_XOPEN2K8 */

%#ifdef __USE_GNU
[throws][cp][doc_alias(execvpe)][ATTR_NORETURN]
[section(.text.crt.except.fs.exec.exec)]
Execvpe:([[nonnull]] char const *__restrict file, [[nonnull]] @__TARGV@, [[nonnull]] @__TENVP@);
%#endif /* __USE_GNU */

%
%#if defined(__USE_MISC) || defined(__USE_XOPEN)
[throws][section(.text.crt.except.sched.param)][noexport][user]
Nice:(int inc) -> int {
	(void)inc;
	/* It should be sufficient to emulate this is a no-op. */
	return 0;
}
%#endif /* __USE_MISC || __USE_XOPEN */


%
%#if defined(__USE_MISC) || defined(__USE_XOPEN_EXTENDED)
[throws][doc_alias(setpgrp)][section(.text.crt.except.sched.process)] SetPGrp:();
[throws][doc_alias(setreuid)][section(.text.crt.except.sched.user)] SetReUid:($uid_t ruid, $uid_t euid);
[throws][doc_alias(setregid)][section(.text.crt.except.sched.user)] SetReGid:($gid_t rgid, $gid_t egid);
%#endif /* __USE_MISC || __USE_XOPEN_EXTENDED */

%
%#ifdef __USE_XOPEN2K
[throws][doc_alias(seteuid)][section(.text.crt.except.sched.user)] SetEUid:($uid_t euid);
[throws][doc_alias(setegid)][section(.text.crt.except.sched.user)] SetEGid:($gid_t egid);
%#endif /* __USE_XOPEN2K */

%
%#if defined(__USE_XOPEN_EXTENDED) || defined(__USE_XOPEN2K)
[throws][doc_alias(symlink)][cp][noexport][section(.text.crt.except.fs.modify)]
[requires(defined(__CRT_AT_FDCWD) && $has_function(SymlinkAt))]
Symlink:([[nonnull]] char const *link_text, [[nonnull]] char const *target_path) {
	SymlinkAt(link_text, __CRT_AT_FDCWD, target_path);
}

%
[throws][cp][noexport][doc_alias(readlink)]
[requires(defined(__CRT_AT_FDCWD) && $has_function(ReadlinkAt))]
[section(.text.crt.except.fs.property)]
Readlink:([[nonnull]] char const *__restrict path,
          [outp(buflen)] char *__restrict buf, size_t buflen) -> size_t {
	ReadlinkAt(__CRT_AT_FDCWD, path, buf, buflen);
}

%#endif /* __USE_XOPEN_EXTENDED || __USE_XOPEN2K */

%
%#if defined(__USE_UNIX98) || defined(__USE_XOPEN2K)
[throws][doc_alias(gethostname)][section(.text.crt.except.system.configuration)]
GetHostName:([outp(buflen)] char *name, size_t buflen);
%#endif /* __USE_UNIX98 || __USE_XOPEN2K */


%
%#ifdef __USE_MISC
%
[throws][doc_alias(sethostname)][section(.text.crt.except.system.configuration)]
SetHostName:([inp(len)] char const *name, size_t len);

%
[throws][doc_alias(getdomainname)][section(.text.crt.except.system.configuration)]
GetDomainName:([outp(buflen)] char *name, size_t buflen);

%
[[throws, doc_alias(setdomainname), section(.text.crt.except.system.configuration)]]
SetDomainName:([inp(len)] char const *name, size_t len);

[[throws, section(.text.crt.except.system.utility), cp]]
Syscall:(__syscall_ulong_t sysno, ...) -> __syscall_slong_t;

%#ifdef __USE_KOS
[[throws, section(.text.crt.except.system.utility), cp, preferred_alias(Syscall)]]
Syscall64:(__syscall_ulong_t sysno, ...) -> __LONG64_TYPE__;
%#endif /* __USE_KOS */

%#endif /* __USE_MISC */

%
%#if defined(__USE_MISC) || \
%   (defined(__USE_XOPEN) && !defined(__USE_XOPEN2K))
[throws][cp][doc_alias(chroot)][section(.text.crt.except.fs.utility)]
ChRoot:([[nonnull]] char const *__restrict path);
%#endif /* ... */


%
%#if defined(__USE_POSIX199309) || defined(__USE_XOPEN_EXTENDED) || defined(__USE_XOPEN2K)
[throws][doc_alias(ftruncate32)][section(.text.crt.except.io.write)][ignore]
FTruncate32:($fd_t fd, __pos32_t length) -> int = FTruncate?;

[[throws, doc_alias(ftruncate), no_crt_self_import]]
[[if(defined(__USE_FILE_OFFSET64)), preferred_alias(FTruncate64)]]
[[if(!defined(__USE_FILE_OFFSET64)), preferred_alias(FTruncate)]]
[[userimpl, requires($has_function(FTruncate32) || $has_function(FTruncate64))]]
[[section(.text.crt.except.io.write)]]
FTruncate:($fd_t fd, pos_t length) {
@@if_has_function(FTruncate32)@@
	FTruncate32(fd, (__pos32_t)length);
@@else_has_function(FTruncate32)@@
	FTruncate64(fd, (__pos64_t)length);
@@endif_has_function(FTruncate32)@@
}

%
%#ifdef __USE_LARGEFILE64
[throws][doc_alias(ftruncate64)]
[off64_variant_of(FTruncate)][noexport]
[requires($has_function(FTruncate32))]
[section(.text.crt.except.io.large.write)]
FTruncate64:($fd_t fd, pos64_t length) {
	FTruncate32(fd, (pos32_t)length);
}
%#endif /* __USE_LARGEFILE64 */
%#endif /* __USE_POSIX199309 || __USE_XOPEN_EXTENDED || __USE_XOPEN2K */

%
%#if defined(__USE_POSIX199309) || defined(__USE_UNIX98)
[throws][doc_alias(fdatasync)][section(.text.crt.except.io.sync)][cp][user]
FDataSync:($fd_t fd) = FSync;
%#endif /* __USE_POSIX199309 || __USE_UNIX98 */


%{

#endif /* __CC__ */

__SYSDECL_END

}