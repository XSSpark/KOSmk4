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

%[define_replacement(char16_t = __CHAR16_TYPE__)]
%[define_replacement(char32_t = __CHAR32_TYPE__)]

%[define_replacement(pwformatprinter = __pwformatprinter)]
%[define_replacement(pc16formatprinter = __pc16formatprinter)]
%[define_replacement(pc32formatprinter = __pc32formatprinter)]

%[define_wchar_replacement(pwformatprinter = pc16formatprinter, pc32formatprinter)]
%[define_wchar_replacement(__pwformatprinter = __pc16formatprinter, __pc32formatprinter)]
%[define_wchar_replacement(format_wsnprintf_data = format_c16snprintf_data, format_c32snprintf_data)]
%[default_impl_section({.text.crt.wchar.string.format|.text.crt.dos.wchar.string.format})]


%(auto_header)#include <parts/uchar/format-printer.h>

%{
#include <features.h>
#ifndef _FORMAT_PRINTER_H
#include <format-printer.h>
#endif /* !_FORMAT_PRINTER_H */
#ifndef _WCHAR_H
#include <wchar.h>
#endif /* !_WCHAR_H */
#include <bits/wformat-printer.h>
#include <libc/malloc.h>
#include <hybrid/__assert.h>

__SYSDECL_BEGIN

#ifdef __CC__

#ifndef __pwformatprinter_defined
#define __pwformatprinter_defined 1
/* Callback functions prototypes provided to format functions.
 * NOTE: '__pformatprinter' usually returns the number of characters printed, but isn't required to.
 * @param: DATA:    The base address of a DATALEN bytes long character vector that should be printed.
 * @param: DATALEN: The amount of characters that should be printed, starting at `data'.
 *                  Note that this is an exact value, meaning that a NUL-character appearing
 *                  before then should not terminate printing prematurely, but be printed as well.
 * @param: CLOSURE: The user-defined closure parameter passed alongside this function pointer.
 * @return: < 0:    An error occurred and the calling function shall return with this same value.
 * @return: >= 0:   The print was successful.
 *                  Usually, the return value is added to a sum of values which is then
 *                  returned by the calling function upon success, also meaning that the
 *                  usual return value used to indicate success is 'DATALEN'. */
typedef __pwformatprinter pwformatprinter;
#endif /* !__pwformatprinter_defined */

}

[attribute(*)][wchar] format_wrepeat:(*) %{copy(format_repeat, str2wcs)}
[attribute(*)][wchar] format_wescape:(*) %{copy(format_escape, str2wcs)}
[attribute(*)][wchar] format_whexdump:(*) %{copy(format_hexdump, str2wcs)}

%
%
%
%

[attribute(*)][wchar] format_vwprintf:(*) %{copy(format_vprintf, str2wcs)}
[attribute(*)][wchar] format_wprintf:(*) %{copy(format_printf, str2wcs)}


%
%
%
@@Format-printer implementation for printing to a string buffer like `wsprintf' would
@@WARNING: No trailing NUL-character is implicitly appended
[wchar]
format_wsprintf_printer:([nonnull] /*wchar_t ***/ void *arg,
                         [nonnull] wchar_t const *__restrict data, $size_t datalen) -> $ssize_t
	%{copy(format_sprintf_printer, str2wcs)}

%{


#ifndef __format_wsnprintf_data_defined
#define __format_wsnprintf_data_defined 1
/* Data structure for implementing waprintf() */
struct format_wsnprintf_data {
	wchar_t      *sd_buffer; /* [0..sd_bufsiz] Pointer to the next memory location to which to write. */
	__SIZE_TYPE__ sd_bufsiz; /* Remaining buffer size. */
};
#endif /* !__format_wsnprintf_data_defined */
#define FORMAT_WSNPRINTF_INIT(buf, bufsize)       { buf, bufsize }
#define format_wsnprintf_init(self, buf, bufsize) ((self)->sd_buffer = (buf), (self)->sd_bufsiz = (bufsize))

}


@@Format-printer implementation for printing to a string buffer like `wsnprintf' would
@@WARNING: No trailing NUL-character is implicitly appended
@@NOTE: The number of written characters is `ORIG_BUFSIZE - ARG->sd_bufsiz'
@@NOTE: The number of required characters is `ARG->sd_buffer - ORIG_BUF', or alternatively the sum of return values of all calls to `format_snprintf_printer()'
[wchar][dependency_include(<hybrid/typecore.h>)]
format_wsnprintf_printer:([nonnull] /*struct format_wsnprintf_data**/ void *arg,
                          [nonnull] wchar_t const *__restrict data, $size_t datalen) -> $ssize_t
	%{copy(format_snprintf_printer, str2wcs)}


[doc_alias(format_width)][ATTR_PURE]
[if(__SIZEOF_WCHAR_T__ == 4), preferred_alias(format_length)][wchar]
format_wwidth:(void *arg, [nonnull] wchar_t const *__restrict data, $size_t datalen) -> $ssize_t {
#if __SIZEOF_WCHAR_T__ == 2
	size_t result = 0;
	wchar_t const *iter, *end;
	(void)arg;
	end = (iter = data) + datalen;
	while (iter < end) {
		wchar_t ch;
		ch = *iter++;
		if (ch >= 0xd800 && ch <= 0xd800 && iter < end) {
			ch = *iter;
			if (ch >= 0xdc00 && ch <= 0xdfff)
				++iter;
		}
		++result;
	}
	return (ssize_t)result;
#else
	(void)arg;
	(void)data;
	/* XXX: Not necessarily correct, as the 32-bit variant is actually ATTR_CONST.
	 *      However, magic headers don't support conditional attributes, so we can't just do
	 *      [if(__SIZEOF_WCHAR_T__ == 2), ATTR_PURE]
	 *      [if(__SIZEOF_WCHAR_T__ != 2), ATTR_CONST] */
	COMPILER_IMPURE();
	return (ssize_t)datalen;
#endif
}

[noexport][nocrt][nouser][ATTR_CONST]
format_wlength:(void *arg, wchar_t const *__restrict data, $size_t datalen) -> $ssize_t = format_length;



%{

#ifndef __format_waprintf_data_defined
#define __format_waprintf_data_defined 1
struct format_waprintf_data {
	wchar_t      *ap_base;  /* [0..ap_used|ALLOC(ap_used+ap_avail)][owned] Buffer */
	__SIZE_TYPE__ ap_avail; /* Unused buffer size */
	__SIZE_TYPE__ ap_used;  /* Used buffer size */
};
#endif /* !__format_waprintf_data_defined */

#define FORMAT_WAPRINTF_DATA_INIT        { __NULLPTR, 0, 0 }
#define format_waprintf_data_init(self)  ((self)->ap_base = __NULLPTR, (self)->ap_avail = (self)->ap_used = 0)
#define format_waprintf_data_cinit(self)            \
	(__hybrid_assert((self)->ap_base == __NULLPTR), \
	 __hybrid_assert((self)->ap_avail == 0),        \
	 __hybrid_assert((self)->ap_used == 0))
#ifdef NDEBUG
#define format_waprintf_data_fini(self)  (__libc_free((self)->ap_base))
#else /* NDEBUG */
#if __SIZEOF_POINTER__ == 4
#define format_waprintf_data_fini(self)                 \
	(__libc_free((self)->ap_base),                      \
	 (self)->ap_base  = (char *)__UINT32_C(0xcccccccc), \
	 (self)->ap_avail = __UINT32_C(0xcccccccc),         \
	 (self)->ap_used  = __UINT32_C(0xcccccccc))
#elif __SIZEOF_POINTER__ == 8
#define format_waprintf_data_fini(self)                         \
	(__libc_free((self)->ap_base),                              \
	 (self)->ap_base  = (char *)__UINT64_C(0xcccccccccccccccc), \
	 (self)->ap_avail = __UINT64_C(0xcccccccccccccccc),         \
	 (self)->ap_used  = __UINT64_C(0xcccccccccccccccc))
#else /* __SIZEOF_POINTER__ == ... */
#define format_waprintf_data_fini(self) (__libc_free((self)->ap_base))
#endif /* __SIZEOF_POINTER__ != ... */
#endif /* !NDEBUG */

}


@@Pack and finalize a given aprintf format printer
@@Together with `format_waprintf_printer()', the aprintf
@@format printer sub-system should be used as follows:
@@>> char *result; ssize_t error;
@@>> struct format_aprintf_data p = FORMAT_WAPRINTF_DATA_INIT;
@@>> error = format_wprintf(&format_waprintf_printer, &p, L"%s %s", "Hello", "World");
@@>> if unlikely(error < 0) {
@@>>     format_waprintf_data_fini(&p);
@@>>     return NULL;
@@>> }
@@>> result = format_waprintf_pack(&p, NULL);
@@>> return result;
@@WARNING: Note that `format_waprintf_pack()' is able to return `NULL' as well,
@@         but will finalize the given aprintf printer an all cases.
@@NOTE:    The caller must destroy the returned string by passing it to `free()'
@@@param: pstrlen: When non-NULL, store the length of the constructed string here
@@                 Note that this is the actual length if the constructed string,
@@                 but may differ from `wcslen(return)' when NUL characters were
@@                 printed to the waprintf-printer at one point.
@@                 (e.g. `format_waprintf_printer(&my_printer, L"\0", 1)')
[requires($has_function(realloc))][same_impl][user]
[dependency_include(<hybrid/__assert.h>)]
[dependency_include(<hybrid/typecore.h>)][wchar]
[ATTR_WUNUSED][ATTR_MALL_DEFAULT_ALIGNED][ATTR_MALLOC]
[dependency_prefix(
#ifndef __format_waprintf_data_defined
#define __format_waprintf_data_defined 1
struct format_waprintf_data {
	wchar_t      *ap_base;  /* [0..ap_used|ALLOC(ap_used+ap_avail)][owned] Buffer */
	__SIZE_TYPE__ ap_avail; /* Unused buffer size */
	__SIZE_TYPE__ ap_used;  /* Used buffer size */
};
#endif /* !__format_waprintf_data_defined */
)]
format_waprintf_pack:([nonnull] struct format_waprintf_data *__restrict self,
                      [nullable] $size_t *pstrlen) -> wchar_t * {
	/* Free unused buffer memory. */
	wchar_t *result;
	if (self->@ap_avail@ != 0) {
		wchar_t *newbuf;
		newbuf = (wchar_t *)realloc(self->@ap_base@,
		                         (self->@ap_used@ + 1) * sizeof(wchar_t));
		if likely(newbuf)
			self->@ap_base@ = newbuf;
	} else {
		if unlikely(!self->@ap_used@) {
			/* Special case: Nothing was printed. */
			__hybrid_assert(!self->@ap_base@);
#ifdef __CRT_HAVE_malloc
			self->@ap_base@ = (wchar_t *)malloc(1 * sizeof(wchar_t));
#else /* __CRT_HAVE_malloc */
			self->@ap_base@ = (wchar_t *)realloc(NULL, 1 * sizeof(wchar_t));
#endif /* !__CRT_HAVE_malloc */
			if unlikely(!self->@ap_base@)
				return NULL;
		}
	}
	result = self->@ap_base@;
	__hybrid_assert(result);
	result[self->@ap_used@] = '\0'; /* NUL-terminate */
	if (pstrlen)
		*pstrlen = self->@ap_used@;
#ifndef NDEBUG
#if __SIZEOF_POINTER__ == 4
	self->@ap_base@  = (wchar_t *)__UINT32_C(0xcccccccc);
	self->@ap_avail@ = __UINT32_C(0xcccccccc);
	self->@ap_used@  = __UINT32_C(0xcccccccc);
#elif __SIZEOF_POINTER__ == 8
	self->@ap_base@  = (wchar_t *)__UINT64_C(0xcccccccccccccccc);
	self->@ap_avail@ = __UINT64_C(0xcccccccccccccccc);
	self->@ap_used@  = __UINT64_C(0xcccccccccccccccc);
#endif /* __SIZEOF_POINTER__ == ... */
#endif /* !NDEBUG */
	return result;
}


@@Print data to a dynamically allocated heap buffer. On error, -1 is returned
[requires($has_function(realloc))][same_impl][user][wchar]
[dependency_include(<hybrid/__assert.h>)][ATTR_WUNUSED]
format_waprintf_printer:([nonnull] /*struct format_waprintf_data **/ void *arg,
                         [nonnull] wchar_t const *__restrict data, $size_t datalen) -> $ssize_t
	%{copy(format_aprintf_printer, str2wcs)}




%{

#endif /* __CC__ */

__SYSDECL_END

}

