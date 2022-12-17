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
#ifndef _LIBREGEX_REGCOMP_H
#define _LIBREGEX_REGCOMP_H 1

#include "api.h"

#include <bits/types.h>

#include <libc/string.h>
#include <libc/malloc.h>

__DECL_BEGIN

/* RE error codes */
typedef int re_errno_t;
#define RE_NOERROR  0  /* Success. */
#define RE_NOMATCH  1  /* Didn't find a match (for `regexec(3)'). */
#define RE_BADPAT   2  /* General pattern syntax error. */
#define RE_ECOLLATE 3  /* ??? */
#define RE_ECTYPE   4  /* Invalid/unknown character class name. */
#define RE_EESCAPE  5  /* Trailing '\'. */
#define RE_ESUBREG  6  /* Back reference to group that was never initialized. */
#define RE_EBRACK   7  /* Unmatched '['. */
#define RE_EPAREN   8  /* Unmatched '('. */
#define RE_EBRACE   9  /* Unmatched '{'. */
#define RE_BADBR    10 /* Invalid contents of '{...}'. */
#define RE_ERANGE   11 /* Invalid range end (e.g. '[z-a]'). */
#define RE_ESPACE   12 /* Out of memory. */
#define RE_BADRPT   13 /* Nothing is preceding '+', '*', '?' or '{'. */
#define RE_EEND     14 /* Unexpected end of pattern. */
#define RE_ESIZE    15 /* Too large (pattern violates some hard limit that isn't the currently available ram) */
#define RE_ERPAREN  16 /* Unmatched ')' (only when `RE_SYNTAX_UNMATCHED_RIGHT_PAREN_ORD' was set) */



/* RE instruction stream opcodes
 *
 * ================================================ Compilation ================================================
 *
 * Simple matches:
 *     >> "x"                   REOP_CHAR 'x'
 *     >> "abc"                 REOP_EXACT "abc"
 *     >> "."                   REOP_ANY
 *     >> "[<x>]"               REOP_CHAR 'x'            (can be used to escape other characters)
 *     >> "\\"                  REOP_CHAR '\'
 *     >> "\<x>"                REOP_CHAR 'x'            (caution: many characters here have special meaning!)
 *     >> "[a-z]"               REOP_BITSET "[a-z]"      (NOTE: multi-byte utf-8 characters are encoded using `REOP_CONTAINS_UTF8')
 *     >> "[^a-z]"              REOP_BITSET_NOT "[a-z]"  (NOTE: multi-byte utf-8 characters are encoded using `REOP_CONTAINS_UTF8_NOT')
 *     >> "\<1-9>"              REOP_GROUP_MATCH <1-9>   // Replaced by `REOP_GROUP_MATCH_Jn' if followed by a repeat-suffix
 *     >> "\w"                  <[:symcont:]>            (HINT: Also allowed in []-sets)
 *     >> "\W"                  <[^:symcont:]>           (HINT: Also allowed in []-sets)
 *     >> "\n"                  <[:lf:]>                 (HINT: Also allowed in []-sets)     [kos-extension]
 *     >> "[:<foo>:]"           REOP_UTF8_IS<foo>                                            [some classes are kos extensions]
 *     >> "[^:<foo>:]"          REOP_UTF8_IS<foo>_NOT                                        [kos-extension]
 *     >> "[:digit<OP><N>:]"    REOP_UTF8_ISDIGIT_<OP> <N>     [1]                           [kos-extension]
 *     >> "[:numeric<OP><N>:]"  REOP_UTF8_ISNUMERIC_<OP> <N>   [1]                           [kos-extension]
 *     >> "\s"                  <[:space:]>              (HINT: Also allowed in []-sets)
 *     >> "\S"                  <[^:space:]>             (HINT: Also allowed in []-sets)
 *     >> "\d"                  <[:digit:]>              (HINT: Also allowed in []-sets)     [python-extension]
 *     >> "\D"                  <[^:digit:]>             (HINT: Also allowed in []-sets)     [python-extension]
 *     >> "\0123"               REOP_CHAR '\0123'        (octal-byte)                        [kos-extension]
 *     >> "\xAB"                REOP_CHAR '\xAB'         (hex-byte)                          [kos-extension]
 *     >> "\uABCD"              REOP_CHAR '\uABCD'       (utf-8 encoded)                     [kos-extension]
 *     >> "\UABCDABCD"          REOP_CHAR '\UABCDABCD'   (utf-8 encoded)                     [kos-extension]
 *
 * [1]: <OP> is one of "=" (or "=="), "!=", "<", "<=", ">", ">="
 *
 * Location assertion (note: it is a syntax error to use these before repetition expressions):
 *     >> "^"            REOP_AT_SOL
 *     >> "$"            REOP_AT_EOL
 *     >> "\`"           REOP_AT_SOI
 *     >> "\'"           REOP_AT_EOI
 *     >> "\b"           REOP_AT_WOB
 *     >> "\B"           REOP_AT_WOB_NOT
 *     >> "\<"           REOP_AT_SOW
 *     >> "\>"           REOP_AT_EOW
 *     >> "\_<"          REOP_AT_SOS
 *     >> "\_>"          REOP_AT_EOS
 *     >> "\A"           REOP_AT_SOI  [python-extension]
 *     >> "\Z"           REOP_AT_EOI  [python-extension]
 *
 * Expressions:
 *     >> "(X)"          REOP_GROUP_START {GID}
 *     >>                <X>
 *     >>                REOP_GROUP_END   {GID}  // Replaced by `REOP_GROUP_END_Jn' if followed by a repeat-suffix
 *
 *     >> "X|Y"          REOP_JMP_ONFAIL  1f
 *     >>                <X>
 *     >>                REOP_JMP         2f
 *     >>                # HINT: Another `REOP_JMP_ONFAIL' to <Z> would go here if it existed
 *     >>             1: <Y>
 *     >>             2:
 *
 *     >> "X|Y|Z"        REOP_JMP_ONFAIL  1f
 *     >>                <X>
 *     >>                REOP_JMP         3f
 *     >>             1: REOP_JMP_ONFAIL  2f
 *     >>                <Y>
 *     >>                REOP_JMP         3f
 *     >>             2: <Z>
 *     >>             3:
 *
 *     >> "X?"           REOP_JMP_ONFAIL 1f
 *     >>                <X>
 *     >>             1:
 *
 *     >> "X*"           REOP_JMP_ONFAIL 2f
 *     >>             1: <X>     // Last instruction is `REOP_*_Jn(N)' transformed to jump to `2f'
 *     >>                REOP_JMP_AND_RETURN_ONFAIL 1b
 *     >>             2:
 *
 *     >> "X+"        1: <X>     // Last instruction is `REOP_*_Jn(N)' transformed to jump to `2f'
 *     >>                REOP_JMP_AND_RETURN_ONFAIL 1b
 *     >>             2:
 *
 *     >> "X{0}"         REOP_NOP        // Or just no instruction at all
 *
 *     >> "X{1}"         <X>
 *
 *     >> "X{0,1}"       <X?>
 *
 *     >> "X{0,}"        <X*>
 *
 *     >> "X{1,}"        <X+>
 *
 *     >> "X{n}"         REOP_SETVAR  {VAR = (n - 1)}
 *     >>             1: <X>
 *     >>                REOP_DEC_JMP {VAR}, 1b
 *
 *     >> "X{n,}"        REOP_SETVAR  {VAR = (n - 1)}
 *     >>             1: <X>     // Last instruction is `REOP_*_Jn(N)' transformed to jump to `2f'
 *     >>                REOP_DEC_JMP {VAR}, 1b
 *     >>                REOP_JMP_AND_RETURN_ONFAIL 1b
 *     >>             2:
 *
 *     >> "X{1,m}"       REOP_SETVAR  {VAR = (m - 1)}
 *     >>            1:  <X>     // Last instruction is `REOP_*_Jn(N)' transformed to jump to `2f'
 *     >>                REOP_DEC_JMP_AND_RETURN_ONFAIL {VAR}, 1b
 *     >>            2:
 *
 *     >> "X{n,m}"       REOP_SETVAR  {VAR1 = n - 1}
 *     >>                REOP_SETVAR  {VAR2 = (m - n)}
 *     >>            1:  <X>     // Last instruction is `REOP_*_Jn(N)' transformed to jump to `2f'
 *     >>                REOP_DEC_JMP {VAR1}, 1b
 *     >>                REOP_DEC_JMP_AND_RETURN_ONFAIL {VAR2}, 1b
 *     >>            2:
 *
 * HINT: Using the above  compilation patterns, inserting  code in front  of
 *       already-compiled code can simply be done whilst ignoring the chance
 *       of  relocations. This is because jumps are relative, and will never
 *       jump out of- or into some block "<X>".
 *       As  such, in order  to compile (e.g.)  "X{n,m}", simply compile "X",
 *       then memmoveup() its bytecode to insert the 2 `REOP_SETVAR' opcodes,
 *       with the guaranty that no jump offsets ever need to be adjusted.
 *
 * NOTES:
 * - "on-failure stack" elements only consist of "{ RESUME_PC, INPUT_POINTER }"
 * - Whenever a mismatch happens, the top-most element of the  "on-failure stack"
 *   is popped, which in turn contains the address of the next REOP_* instruction
 *   with which to continue, as well as the input pointer that should be restored
 * - `REOP_*_Jn(N)'-transformation works on the bytecode of an expression <X>, by
 *   replacing the last opcode:
 *    - REOP_GROUP_MATCH -> REOP_GROUP_MATCH_Jn
 *    - REOP_GROUP_END -> REOP_GROUP_END_Jn
 *   This opcode replacement is not performed when <X> doesn't match epsilon.
 *   -> This transformation has to be done to prevent infinite loops in "()+",
 *      which would otherwise be encoded as:
 *      >> 1:  REOP_GROUP_START {GID}
 *      >>     REOP_GROUP_END   {GID}
 *      >>     REOP_JMP_AND_RETURN_ONFAIL 1b
 *      >>     REOP_MATCHED
 *      Since the body can never fail, `REOP_JMP_AND_RETURN_ONFAIL' will  just
 *      keep on  jumping back  to the  start of  the expression  indefinitely.
 *      However, by performing `REOP_*_Jn(N)'-transformation, the new code is:
 *      >> 1:  REOP_GROUP_START  {GID}
 *      >>     REOP_GROUP_END_J3 {GID}
 *      >>     REOP_JMP_AND_RETURN_ONFAIL 1b
 *      >>     REOP_MATCHED
 *      Which behaves differently. When executed, `regexec(3)' will notice  that
 *      the group-expression matched epsilon, so  rather than continue with  the
 *      `REOP_JMP_AND_RETURN_ONFAIL' instruction, it  will skip  3 bytes,  which
 *      just to happens to land us at the `REOP_MATCHED' opcode, thus preventing
 *      any sort of looping.
 *   -> This  also works in  something like "(|foo|bar)+".  Here, the first time
 *      around, the regex will push an  on-fail address for "foo" when  matching
 *      epsilon before proceeding to skip over the "+" suffix. With the  initial
 *      match  (of an  empty string) set,  the previously pushed  on-fail is now
 *      run, and the other branches are allowed to (try) to match more than just
 *      an empty word.
 *
 *
 * TODO: "(?<=X)"   -- Current input position must be preceded by "X"
 * TODO: "(?<!X)"   -- Current input position must not be preceded by "X"
 * TODO: "(?=X)"    -- Current input position must be succeeded by "X"
 * TODO: "(?!X)"    -- Current input position must not be succeeded by "X"
 * TODO: Support for other (?)-extensions
 */


/* Regex opcodes (always encoded as a single byte) */
enum {

	/* Opcodes for matching (and thus: consuming) input */
	REOP_EXACT,                /* [+1+n] Followed by 1 byte N = *PC++, followed by N more bytes that must be matched exactly
	                            * NOTE: N must be >= 2 */
	REOP_EXACT_ASCII_ICASE,    /* [+1+n] Like 'REOP_EXACT', but strings are both ascii, and casing should be ignored (used instead of
	                            * `REOP_EXACT_UTF8_ICASE' when the pattern string is ascii-only, or when `RE_SYNTAX_NO_UTF8' was set)
	                            * NOTE: N must be >= 2 */
	REOP_EXACT_UTF8_ICASE,     /* [+1+n] Followed  by  a  COUNT-byte, followed  by  a `COUNT'-character
	                            * long utf-8 string (matches utf-8 character contained in said string).
	                            * NOTE: COUNT must be >= 1 */
	REOP_ANY,                  /* [+0] Match any character */
	REOP_ANY_NOTLF,            /* [+0] Match any character (except ASCII line-feeds) */
#define REOP_ANY_NOTLF_UTF8 REOP_UTF8_ISLF_NOT
	REOP_ANY_NOTNUL,           /* [+0] Match any character (except '\0') */
	REOP_ANY_NOTNUL_NOTLF,     /* [+0] Match any character (except '\0' or ASCII line-feeds) */
	REOP_ANY_NOTNUL_NOTLF_UTF8, /* [+0] Match any character (except '\0' or ASCII line-feeds) */
	REOP_CHAR,                 /* [+1] Followed by 1 byte that must be matched exactly */
	REOP_CHAR2,                /* [+2] Followed by 2 bytes, one of which must be matched exactly (for "[ab]" or "a" -> "[aA]" in ICASE-mode) */
	REOP_CONTAINS_UTF8,        /* [+1+n] Followed by a COUNT-byte, followed by a `COUNT'-character long utf-8 string (matches utf-8 character contained in said string). */
	REOP_CONTAINS_UTF8_NOT,    /* [+1+n] Followed by a COUNT-byte, followed by a `COUNT'-character long utf-8 string (matches utf-8 character not contained in said string). */
	REOP_BITSET,               /* [+1+n] Followed by a LAYOUT-byte, followed by `REOP_BITSET_LAYOUT_GETBYTES(LAYOUT)' bitset bytes.
	                            * >> bool is_char_in_set(byte_t const *layout_ptr, uint8_t ch) {
	                            * >>     uint8_t layout      = *layout_ptr++;
	                            * >>     uint8_t minch       = REOP_BITSET_LAYOUT_GETBASE(layout);
	                            * >>     uint8_t bitset_size = REOP_BITSET_LAYOUT_GETBYTES(layout);
	                            * >>     uint8_t bitset_bits = bitset_size * 8;
	                            * >>     if (OVERFLOW_USUB(ch, minch, &ch))
	                            * >>         return false;
	                            * >>     if (ch >= bitset_bits)
	                            * >>         return false;
	                            * >>     return (layout_ptr[ch / 8] & (1 << (ch % 8))) != 0;
	                            * >> }
	                            * NOTE: If the bitset would need to contain non-ascii characters, `REOP_CONTAINS_UTF8' is used instead. */
	REOP_BITSET_NOT,           /* [+1+n] Same as REOP_BITSET, but the return value of `is_char_in_set' is inverted */
	REOP_BITSET_UTF8_NOT,      /* [+1+n] Same as `REOP_BITSET_NOT', but non-matching character must be utf-8 */
	REOP_GROUP_MATCH,          /* [+1] Re-match the contents of the (N = *PC++)'th already-matched group; If group start/end aren't, triggers a match-failure. */
	REOP_GROUP_MATCH_J3,       /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 3 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_MATCH_J4,       /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 4 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_MATCH_J5,       /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 5 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_MATCH_J6,       /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 6 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_MATCH_J7,       /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 7 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_MATCH_J8,       /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 8 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_MATCH_J9,       /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 9 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_MATCH_J10,      /* [+1] Same as `REOP_GROUP_MATCH', but skip the next 10 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
#define REOP_GROUP_MATCH_JMIN  REOP_GROUP_MATCH_J3
#define REOP_GROUP_MATCH_JMAX  REOP_GROUP_MATCH_J10
#define REOP_GROUP_MATCH_Jn(n) (REOP_GROUP_MATCH_J3 + (n) - 3)
#define REOP_GROUP_MATCH_Joff(opcode) (3 + (opcode) - REOP_GROUP_MATCH_J3)

	/* Numerical attribute classes */
#define REOP_ASCII_ISDIGIT_cmp_MIN REOP_ASCII_ISDIGIT_EQ
	REOP_ASCII_ISDIGIT_EQ,     /* [+1] Match `isdigit(ch) && (ch - 0x30) == *PC++' */
	REOP_ASCII_ISDIGIT_NE,     /* [+1] Match `isdigit(ch) && (ch - 0x30) != *PC++' */
	REOP_ASCII_ISDIGIT_LO,     /* [+1] Match `isdigit(ch) && (ch - 0x30) <  *PC++' */
	REOP_ASCII_ISDIGIT_LE,     /* [+1] Match `isdigit(ch) && (ch - 0x30) <= *PC++' */
	REOP_ASCII_ISDIGIT_GR,     /* [+1] Match `isdigit(ch) && (ch - 0x30) >  *PC++' */
	REOP_ASCII_ISDIGIT_GE,     /* [+1] Match `isdigit(ch) && (ch - 0x30) >= *PC++' */
#define REOP_ASCII_ISDIGIT_cmp_MAX REOP_ASCII_ISDIGIT_GE
#define REOP_UTF8_ISDIGIT_cmp_MIN REOP_UTF8_ISDIGIT_EQ
	REOP_UTF8_ISDIGIT_EQ,      /* [+1] Match `unicode_isdigit(ch) && unicode_getnumeric(ch) == *PC++' */
	REOP_UTF8_ISDIGIT_NE,      /* [+1] Match `unicode_isdigit(ch) && unicode_getnumeric(ch) != *PC++' */
	REOP_UTF8_ISDIGIT_LO,      /* [+1] Match `unicode_isdigit(ch) && unicode_getnumeric(ch) <  *PC++' */
	REOP_UTF8_ISDIGIT_LE,      /* [+1] Match `unicode_isdigit(ch) && unicode_getnumeric(ch) <= *PC++' */
	REOP_UTF8_ISDIGIT_GR,      /* [+1] Match `unicode_isdigit(ch) && unicode_getnumeric(ch) >  *PC++' */
	REOP_UTF8_ISDIGIT_GE,      /* [+1] Match `unicode_isdigit(ch) && unicode_getnumeric(ch) >= *PC++' */
#define REOP_UTF8_ISDIGIT_cmp_MAX REOP_UTF8_ISDIGIT_GE
#define REOP_UTF8_ISNUMERIC_cmp_MIN REOP_UTF8_ISNUMERIC_EQ
	REOP_UTF8_ISNUMERIC_EQ,    /* [+1] Match `unicode_isnumeric(ch) && unicode_getnumeric(ch) == *PC++' */
	REOP_UTF8_ISNUMERIC_NE,    /* [+1] Match `unicode_isnumeric(ch) && unicode_getnumeric(ch) != *PC++' */
	REOP_UTF8_ISNUMERIC_LO,    /* [+1] Match `unicode_isnumeric(ch) && unicode_getnumeric(ch) <  *PC++' */
	REOP_UTF8_ISNUMERIC_LE,    /* [+1] Match `unicode_isnumeric(ch) && unicode_getnumeric(ch) <= *PC++' */
	REOP_UTF8_ISNUMERIC_GR,    /* [+1] Match `unicode_isnumeric(ch) && unicode_getnumeric(ch) >  *PC++' */
	REOP_UTF8_ISNUMERIC_GE,    /* [+1] Match `unicode_isnumeric(ch) && unicode_getnumeric(ch) >= *PC++' */
#define REOP_UTF8_ISNUMERIC_cmp_MAX REOP_UTF8_ISNUMERIC_GE

	/* Special trait classes */
	/* TODO: Deal with `RE_SYNTAX_HAT_LISTS_NOT_NEWLINE' */
#define REOP_TRAIT_ASCII_MIN REOP_ASCII_ISCNTRL
#define REOP_TRAIT_ASCII_ISNOT(x) ((((x) - REOP_ASCII_ISCNTRL_NOT) & 1) == 0)
	REOP_ASCII_ISCNTRL,         /* [+0] consume trait `iscntrl(ch) == true' */
	REOP_ASCII_ISCNTRL_NOT,     /* [+0] consume trait `iscntrl(ch) == false' */
	REOP_ASCII_ISSPACE,         /* [+0] consume trait `isspace(ch) == true' */
	REOP_ASCII_ISSPACE_NOT,     /* [+0] consume trait `isspace(ch) == false' */
	REOP_ASCII_ISUPPER,         /* [+0] consume trait `isupper(ch) == true' */
	REOP_ASCII_ISUPPER_NOT,     /* [+0] consume trait `isupper(ch) == false' */
	REOP_ASCII_ISLOWER,         /* [+0] consume trait `islower(ch) == true' */
	REOP_ASCII_ISLOWER_NOT,     /* [+0] consume trait `islower(ch) == false' */
	REOP_ASCII_ISALPHA,         /* [+0] consume trait `isalpha(ch) == true' */
	REOP_ASCII_ISALPHA_NOT,     /* [+0] consume trait `isalpha(ch) == false' */
	REOP_ASCII_ISDIGIT,         /* [+0] consume trait `isdigit(ch) == true' */
	REOP_ASCII_ISDIGIT_NOT,     /* [+0] consume trait `isdigit(ch) == false' */
	REOP_ASCII_ISXDIGIT,        /* [+0] consume trait `isxdigit(ch) == true' */
	REOP_ASCII_ISXDIGIT_NOT,    /* [+0] consume trait `isxdigit(ch) == false' */
	REOP_ASCII_ISALNUM,         /* [+0] consume trait `isalnum(ch) == true' */
	REOP_ASCII_ISALNUM_NOT,     /* [+0] consume trait `isalnum(ch) == false' */
	REOP_ASCII_ISPUNCT,         /* [+0] consume trait `ispunct(ch) == true' */
	REOP_ASCII_ISPUNCT_NOT,     /* [+0] consume trait `ispunct(ch) == false' */
	REOP_ASCII_ISGRAPH,         /* [+0] consume trait `isgraph(ch) == true' */
	REOP_ASCII_ISGRAPH_NOT,     /* [+0] consume trait `isgraph(ch) == false' */
	REOP_ASCII_ISPRINT,         /* [+0] consume trait `isprint(ch) == true' */
	REOP_ASCII_ISPRINT_NOT,     /* [+0] consume trait `isprint(ch) == false' */
	REOP_ASCII_ISBLANK,         /* [+0] consume trait `isblank(ch) == true' */
	REOP_ASCII_ISBLANK_NOT,     /* [+0] consume trait `isblank(ch) == false' */
	REOP_ASCII_ISSYMSTRT,       /* [+0] consume trait `issymstrt(ch) == true' */
	REOP_ASCII_ISSYMSTRT_NOT,   /* [+0] consume trait `issymstrt(ch) == false' */
	REOP_ASCII_ISSYMCONT,       /* [+0] consume trait `issymcont(ch) == true' */
	REOP_ASCII_ISSYMCONT_NOT,   /* [+0] consume trait `issymcont(ch) == false' */
#define REOP_TRAIT_ASCII_MAX REOP_ASCII_ISSYMCONT_NOT

#define REOP_TRAIT_UTF8_MIN REOP_UTF8_ISCNTRL
#define REOP_TRAIT_UTF8_ISNOT(x) ((((x) - REOP_UTF8_ISCNTRL_NOT) & 1) == 0)
	REOP_UTF8_ISCNTRL,         /* [+0] consume trait `unicode_iscntrl(ch) == true' */
	REOP_UTF8_ISCNTRL_NOT,     /* [+0] consume trait `unicode_iscntrl(ch) == false' */
	REOP_UTF8_ISSPACE,         /* [+0] consume trait `unicode_isspace(ch) == true' */
	REOP_UTF8_ISSPACE_NOT,     /* [+0] consume trait `unicode_isspace(ch) == false' */
	REOP_UTF8_ISUPPER,         /* [+0] consume trait `unicode_isupper(ch) == true' */
	REOP_UTF8_ISUPPER_NOT,     /* [+0] consume trait `unicode_isupper(ch) == false' */
	REOP_UTF8_ISLOWER,         /* [+0] consume trait `unicode_islower(ch) == true' */
	REOP_UTF8_ISLOWER_NOT,     /* [+0] consume trait `unicode_islower(ch) == false' */
	REOP_UTF8_ISALPHA,         /* [+0] consume trait `unicode_isalpha(ch) == true' */
	REOP_UTF8_ISALPHA_NOT,     /* [+0] consume trait `unicode_isalpha(ch) == false' */
	REOP_UTF8_ISDIGIT,         /* [+0] consume trait `unicode_isdigit(ch) == true' */
	REOP_UTF8_ISDIGIT_NOT,     /* [+0] consume trait `unicode_isdigit(ch) == false' */
	REOP_UTF8_ISXDIGIT,        /* [+0] consume trait `unicode_isxdigit(ch) == true' */
	REOP_UTF8_ISXDIGIT_NOT,    /* [+0] consume trait `unicode_isxdigit(ch) == false' */
	REOP_UTF8_ISALNUM,         /* [+0] consume trait `unicode_isalnum(ch) == true' */
	REOP_UTF8_ISALNUM_NOT,     /* [+0] consume trait `unicode_isalnum(ch) == false' */
	REOP_UTF8_ISPUNCT,         /* [+0] consume trait `unicode_ispunct(ch) == true' */
	REOP_UTF8_ISPUNCT_NOT,     /* [+0] consume trait `unicode_ispunct(ch) == false' */
	REOP_UTF8_ISGRAPH,         /* [+0] consume trait `unicode_isgraph(ch) == true' */
	REOP_UTF8_ISGRAPH_NOT,     /* [+0] consume trait `unicode_isgraph(ch) == false' */
	REOP_UTF8_ISPRINT,         /* [+0] consume trait `unicode_isprint(ch) == true' */
	REOP_UTF8_ISPRINT_NOT,     /* [+0] consume trait `unicode_isprint(ch) == false' */
	REOP_UTF8_ISBLANK,         /* [+0] consume trait `unicode_isblank(ch) == true' */
	REOP_UTF8_ISBLANK_NOT,     /* [+0] consume trait `unicode_isblank(ch) == false' */
	REOP_UTF8_ISSYMSTRT,       /* [+0] consume trait `unicode_issymstrt(ch) == true' */
	REOP_UTF8_ISSYMSTRT_NOT,   /* [+0] consume trait `unicode_issymstrt(ch) == false' */
	REOP_UTF8_ISSYMCONT,       /* [+0] consume trait `unicode_issymcont(ch) == true' */
	REOP_UTF8_ISSYMCONT_NOT,   /* [+0] consume trait `unicode_issymcont(ch) == false' */
	REOP_UTF8_ISTAB,           /* [+0] consume trait `unicode_istab(ch) == true' */
	REOP_UTF8_ISTAB_NOT,       /* [+0] consume trait `unicode_istab(ch) == false' */
	REOP_UTF8_ISWHITE,         /* [+0] consume trait `unicode_iswhite(ch) == true' */
	REOP_UTF8_ISWHITE_NOT,     /* [+0] consume trait `unicode_iswhite(ch) == false' */
	REOP_UTF8_ISEMPTY,         /* [+0] consume trait `unicode_isempty(ch) == true' */
	REOP_UTF8_ISEMPTY_NOT,     /* [+0] consume trait `unicode_isempty(ch) == false' */
	REOP_UTF8_ISLF,            /* [+0] consume trait `unicode_islf(ch) == true' */
	REOP_UTF8_ISLF_NOT,        /* [+0] consume trait `unicode_islf(ch) == false' */
	REOP_UTF8_ISHEX,           /* [+0] consume trait `unicode_ishex(ch) == true' */
	REOP_UTF8_ISHEX_NOT,       /* [+0] consume trait `unicode_ishex(ch) == false' */
	REOP_UTF8_ISTITLE,         /* [+0] consume trait `unicode_istitle(ch) == true' */
	REOP_UTF8_ISTITLE_NOT,     /* [+0] consume trait `unicode_istitle(ch) == false' */
	REOP_UTF8_ISNUMERIC,       /* [+0] consume trait `unicode_isnumeric(ch) == true' */
	REOP_UTF8_ISNUMERIC_NOT,   /* [+0] consume trait `unicode_isnumeric(ch) == false' */
#define REOP_TRAIT_UTF8_MAX REOP_UTF8_ISNUMERIC_NOT

	/* Opcodes for asserting the current position in input (these don't consume anything) */
#define REOP_AT_MIN REOP_AT_SOI
	REOP_AT_SOI,          /* [+0] Start-of-input */
	REOP_AT_EOI,          /* [+0] End-of-input */
	REOP_AT_SOL,          /* [+0] Start-of-line (following a line-feed, or `REOP_AT_SOI' unless `RE_EXEC_NOTBOL' was set) */
	REOP_AT_SOL_UTF8,     /* [+0] Start-of-line (following a line-feed, or `REOP_AT_SOI' unless `RE_EXEC_NOTBOL' was set) */
	REOP_AT_EOL,          /* [+0] End-of-line (preceding a line-feed, or `REOP_AT_EOI' unless `RE_EXEC_NOTEOL' was set) */
	REOP_AT_EOL_UTF8,     /* [+0] End-of-line (preceding a line-feed, or `REOP_AT_EOI' unless `RE_EXEC_NOTEOL' was set) */
	REOP_AT_WOB,          /* [+0] WOrdBoundary (preceding and next character have non-equal `issymcont(ch)'; OOB counts as `issymcont == false') */
	REOP_AT_WOB_UTF8,     /* [+0] WOrdBoundary (preceding and next character have non-equal `unicode_issymcont(ch)'; OOB counts as `unicode_issymcont == false') */
	REOP_AT_WOB_NOT,      /* [+0] NOT WOrdBoundary (preceding and next character have equal `issymcont(ch)'; OOB counts as `unicode_issymcont == false') */
	REOP_AT_WOB_UTF8_NOT, /* [+0] NOT WOrdBoundary (preceding and next character have equal `unicode_issymcont(ch)'; OOB counts as `issymcont == false') */
	REOP_AT_SOW,          /* [+0] StartOfWord (preceding and next character are `!issymcont(lhs) && issymcont(rhs)'; OOB counts as `issymcont == false') */
	REOP_AT_SOW_UTF8,     /* [+0] StartOfWord (preceding and next character are `!unicode_issymcont(lhs) && unicode_issymcont(rhs)'; OOB counts as `unicode_issymcont == false') */
	REOP_AT_EOW,          /* [+0] EndOfWord (preceding and next character are `issymcont(lhs) && !issymcont(rhs)'; OOB counts as `issymcont == false') */
	REOP_AT_EOW_UTF8,     /* [+0] EndOfWord (preceding and next character are `unicode_issymcont(lhs) && !unicode_issymcont(rhs)'; OOB counts as `unicode_issymcont == false') */
#define REOP_AT_SOS REOP_AT_SOW
	REOP_AT_SOS_UTF8,     /* [+0] StartOfSymbol (preceding and next character are `!unicode_issymcont(lhs) && unicode_issymstrt(rhs)'; OOB counts as `issymcont[/strt] == false') */
#define REOP_AT_EOS      REOP_AT_EOW
#define REOP_AT_EOS_UTF8 REOP_AT_EOW_UTF8
#define REOP_AT_MAX      REOP_AT_SOS_UTF8

	/* Opcodes for expression logic and processing. */
	REOP_GROUP_START,           /* [+1] Mark the start of the (N = *PC++)'th group; open "(" (current input pointer is written to `regmatch_t[N].rm_so') */
	REOP_GROUP_END,             /* [+1] Mark the end of the (N = *PC++)'th group; closing ")" (current input pointer is written to `regmatch_t[N].rm_eo') */
#define REOP_GROUP_END_JMIN         REOP_GROUP_END_J3
#define REOP_GROUP_END_JMAX         REOP_GROUP_END_J10
#define REOP_GROUP_END_Jn(n)        (REOP_GROUP_END_J3 + (n) - 3)
#define REOP_GROUP_END_Joff(opcode) (3 + (opcode) - REOP_GROUP_END_J3)
	REOP_GROUP_END_J3,          /* [+1] Same as `REOP_GROUP_END', but skip the next 3 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_END_J4,          /* [+1] Same as `REOP_GROUP_END', but skip the next 4 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_END_J5,          /* [+1] Same as `REOP_GROUP_END', but skip the next 5 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_END_J6,          /* [+1] Same as `REOP_GROUP_END', but skip the next 6 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_END_J7,          /* [+1] Same as `REOP_GROUP_END', but skip the next 7 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_END_J8,          /* [+1] Same as `REOP_GROUP_END', but skip the next 8 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_END_J9,          /* [+1] Same as `REOP_GROUP_END', but skip the next 9 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_GROUP_END_J10,         /* [+1] Same as `REOP_GROUP_END', but skip the next 10 instruction bytes if epsilon was matched (iow: `regmatch_t[N].rm_so == regmatch_t[N].rm_eo') */
	REOP_JMP_ONFAIL,            /* [+2] push onto the "on-failure stack" a 16-bit, signed, relative addr */
	REOP_JMP,                   /* [+2] 16-bit, signed, relative jump (relative to instruction end) */
	REOP_JMP_AND_RETURN_ONFAIL, /* [+2] push onto the "on-failure stack" the address of the next instruction before doing `REOP_JMP' */
	REOP_DEC_JMP,               /* [+3] VAR = VARS[*PC++]; if VAR != 0, do `--VAR', followed by `REOP_JMP'; else, do nothing (and leave `VAR' unchanged) */
	REOP_DEC_JMP_AND_RETURN_ONFAIL, /* [+3] VAR = VARS[*PC++]; if VAR != 0, do `--VAR', followed by `REOP_JMP_AND_RETURN_ONFAIL'; else, do nothing (and leave `VAR' and the "on-failure stack" unchanged) */
	REOP_SETVAR,                /* [+2] VAR = VARS[*PC++]; VAR = *PC++; */
	REOP_NOP,                   /* [+0] No-op (for padding to get alignment right) */
	REOP_MATCHED,               /* [+0] Text has been matched (end of instruction stream)
	                             * If  the "on-failure stack" is  non-empty (and the  current match isn't perfect),
	                             * the current match is saved as a candidate, and the "on-failure stack" is popped.
	                             * Extra candidates are compared against prior ones, and the "best" one is  finally
	                             * used  (note that a  candidate consists of `{ end_input_pointer, regmatch_t[] }')
	                             * Candidate A is better than B if `A.end_input_pointer > B.end_input_pointer' */
	REOP_MATCHED_PERFECT,       /* [+0] Same as `REOP_MATCHED', but act as though the match was perfect (even if it might not be; s.a. `RE_SYNTAX_NO_POSIX_BACKTRACKING') */
};

/* Helper macros for `REOP_BITSET' and `REOP_BITSET_NOT' */
#define REOP_BITSET_LAYOUT_GETBYTES(layout) (((layout)&0x1f) + 1)
#define REOP_BITSET_LAYOUT_GETBASE(layout)  ((layout)&0xe0)



/* Regex syntax flags */
#define RE_SYNTAX_BACKSLASH_ESCAPE_IN_LISTS 0x00000001 /* '\' can be used to escape characters in sets: '[a\[\]\-]' */
#define RE_SYNTAX_BK_PLUS_QM                0x00000002 /* If clear: '+' and '?' are operators and '\+' and '\?' are literals; if set: the opposite is the case. */
#define RE_SYNTAX_CHAR_CLASSES              0x00000004 /* Support for char-classes (e.g. `[:alpha:]') */
#define RE_SYNTAX_CONTEXT_INDEP_ANCHORS     0x00000008 /* '^' and '$' are always anchors (as opposed to only at the start/end or after/before a '(' and ')') */
#define RE_SYNTAX_CONTEXT_INDEP_OPS         0x00000010 /* Ignored... */
#define RE_SYNTAX_CONTEXT_INVALID_OPS       0x00000020 /* '*', '+', '{' and '?' appearing at the start or after '(' or '|' results in `RE_BADRPT'; If not set, they are treated as literals. */
#define RE_SYNTAX_DOT_NEWLINE               0x00000040 /* '.' matches line-feeds (if not set, then it doesn't) */
#define RE_SYNTAX_DOT_NOT_NULL              0x00000080 /* '.' doesn't match '\0' (if not set, then it does) */
#define RE_SYNTAX_HAT_LISTS_NOT_NEWLINE     0x00000100 /* '[^abc]' will not match line-feeds (as though line-feeds were part of the set of characters never matched). If not set, [^]-sets will match them (unless explicitly added to the set of unmatched characters) */
#define RE_SYNTAX_INTERVALS                 0x00000200 /* Enable support for intervals: 'x{1,2}' (if not set, '{' and '}' are literals, though escaping is governed by `RE_SYNTAX_NO_BK_BRACES') */
#define RE_SYNTAX_LIMITED_OPS               0x00000400 /* If set, support for '+', '?' and '|' is disabled (if not set, support is enabled, though escaping is governed by `RE_SYNTAX_BK_PLUS_QM' and `RE_SYNTAX_NO_BK_VBAR') */
#define RE_SYNTAX_NEWLINE_ALT               0x00000800 /* '\n' (embedded ASCII 10h) is treated like as an alias for the '|'-operator (if not set, '\n' is a literal; but note the kos-exception "\" "n", which matches that 2-character sequence against arbitrary line-feeds) */
#define RE_SYNTAX_NO_BK_BRACES              0x00001000 /* If set, '{...}' are intervals, and '\{' and '\}' are literals; if clear, the opposite is the case (iow: '\{...\}' is an interval) */
#define RE_SYNTAX_NO_BK_PARENS              0x00002000 /* If set, '(...)' are groups, and '\(' and '\)' are literals; if clear, the opposite is the case (iow: '\(...\)' is a group) */
#define RE_SYNTAX_NO_BK_REFS                0x00004000 /* If set, '\<1-9>' matches the literal <1-9>; if clear, '\<1-9>' is a back-reference */
#define RE_SYNTAX_NO_BK_VBAR                0x00008000 /* If set, '|' is the alternation operation: '(a|b)' (and '\|' is a literal); if clear, the opposite is the case (iow: '(a\|b)' matches '[ab]') */
#define RE_SYNTAX_NO_EMPTY_RANGES           0x00010000 /* If set, a []-set like '[z-a]' results in `RE_ERANGE'; if clear, it is equal to '[]' (iow: bad ranges are simply ignored) */
#define RE_SYNTAX_UNMATCHED_RIGHT_PAREN_ORD 0x00020000 /* If set, unmatched ')' are treated as literals; if clear, unmatched an ')' results in `RE_ERPAREN' */
#define RE_SYNTAX_NO_POSIX_BACKTRACKING     0x00040000 /* If set, return the first match, rather than the perfect one (s.a. `REOP_MATCHED' and `REOP_MATCHED_PERFECT') */
#define RE_SYNTAX_NO_GNU_OPS                0x00080000 /* If set, disable support for '\<', '\>', '\b', '\B', '\w', '\W', '\s', '\S', '\`' and "\'" */
#define RE_SYNTAX_DEBUG                     0x00100000 /* Ignored... */
#define RE_SYNTAX_INVALID_INTERVAL_ORD      0x00200000 /* Invalid intervals like "a{b" are treated as literals (i.e. like "a\{b") */
#define RE_SYNTAX_ICASE                     0x00400000 /* Casing is ignored by literal-matches, and '[:lower:]', '[:upper:]', '[:title:]' are aliases for '[:alpha:]' */
#define RE_SYNTAX_CARET_ANCHORS_HERE        0x00800000 /* Alias for `RE_SYNTAX_CONTEXT_INDEP_ANCHORS', but only for '^', and used internally */
#define RE_SYNTAX_CONTEXT_INVALID_DUP       0x01000000 /* If set, '{' appearing at the start, or after '(', '|' or '}' results in `RE_BADRPT'; else, behavior is governed by `RE_SYNTAX_CONTEXT_INVALID_OPS' */
#define RE_SYNTAX_NO_SUB                    0x02000000 /* Ignored... (used at a different point to implement `RE_NOSUB') */
#define RE_SYNTAX_NO_UTF8                   0x40000000 /* If set, pattern is byte-based (rather than a utf-8 string; e.g. '[ä]' is like '[\xC3\xA4]'). Also disables support for '\uABCD', '\UABCDABCD' */
#define RE_SYNTAX_NO_KOS_OPS                0x80000000 /* If set, disable support for python- and kos-extensions: '\n', "[^:<foo>:]", '\d', '\D', '\0123', '\xAB', '\uABCD', '\UABCDABCD', '\A', '\Z' */

/* Regex parser structure.
 * The behavior of `re_parser_yield()' is affected by the following syntax flags:
 * - RE_SYNTAX_BK_PLUS_QM
 * - RE_SYNTAX_CONTEXT_INDEP_ANCHORS
 * - RE_SYNTAX_INTERVALS
 * - RE_SYNTAX_LIMITED_OPS
 * - RE_SYNTAX_NEWLINE_ALT
 * - RE_SYNTAX_NO_BK_BRACES
 * - RE_SYNTAX_NO_BK_PARENS
 * - RE_SYNTAX_NO_BK_REFS
 * - RE_SYNTAX_NO_BK_VBAR
 * - RE_SYNTAX_UNMATCHED_RIGHT_PAREN_ORD
 * - RE_SYNTAX_NO_GNU_OPS
 * - RE_SYNTAX_INVALID_INTERVAL_ORD
 * - RE_SYNTAX_CARET_ANCHORS_HERE
 * - RE_SYNTAX_NO_UTF8
 * - RE_SYNTAX_NO_KOS_OPS */
struct re_parser {
	char const *rep_pos;    /* [1..1][>= rep_pat] Pointer to next pattern-character that has yet to be compiled. */
	char const *rep_pat;    /* [1..1] Pointer to the start of the pattern being compiled. */
	__uintptr_t rep_syntax; /* [const] RE syntax flags (set of `RE_SYNTAX_*') */
};

#define re_parser_init(self, pattern)      \
	(void)((self)->rep_pos    = (pattern), \
	       (self)->rep_pat    = (pattern), \
	       (self)->rep_syntax = (syntax))

/* Regex token (one of `RE_TOKEN_*', or a utf-32 character (or byte when `RE_SYNTAX_NO_UTF8' is set)) */
typedef __uint32_t re_token_t;
#define RE_TOKEN_ISLITERAL(x)  ((x) < RE_TOKEN_BASE)
#define RE_TOKEN_BASE          0x110000             /* First regex token number */
#define RE_TOKEN_EOF           (RE_TOKEN_BASE + 0)  /* End-of-pattern */
#define RE_TOKEN_ISSUFFIX(x) ((x) >= RE_TOKEN_PLUS && (x) <= RE_TOKEN_STARTINTERVAL)
#define RE_TOKEN_ANY           (RE_TOKEN_BASE + 1)  /* '.' */
#define RE_TOKEN_PLUS          (RE_TOKEN_BASE + 2)  /* '+' */
#define RE_TOKEN_STAR          (RE_TOKEN_BASE + 3)  /* '*' */
#define RE_TOKEN_QMARK         (RE_TOKEN_BASE + 4)  /* '?' */
#define RE_TOKEN_STARTINTERVAL (RE_TOKEN_BASE + 5)  /* '{' */
#define RE_TOKEN_STARTSET      (RE_TOKEN_BASE + 6)  /* '[' */
#define RE_TOKEN_STARTGROUP    (RE_TOKEN_BASE + 7)  /* '(' */
#define RE_TOKEN_ENDGROUP      (RE_TOKEN_BASE + 8)  /* ')' */
#define RE_TOKEN_ALTERNATION   (RE_TOKEN_BASE + 9)  /* '|' */
#define RE_TOKEN_BK_w          (RE_TOKEN_BASE + 10)  /* '\w' */
#define RE_TOKEN_BK_W          (RE_TOKEN_BASE + 11) /* '\W' */
#define RE_TOKEN_BK_s          (RE_TOKEN_BASE + 12) /* '\s' */
#define RE_TOKEN_BK_S          (RE_TOKEN_BASE + 13) /* '\S' */
#define RE_TOKEN_BK_d          (RE_TOKEN_BASE + 14) /* '\d' */
#define RE_TOKEN_BK_D          (RE_TOKEN_BASE + 15) /* '\D' */
#define RE_TOKEN_BK_n          (RE_TOKEN_BASE + 16) /* '\n' */
#define RE_TOKEN_AT_MIN        RE_TOKEN_AT_SOL
#define RE_TOKEN_AT_SOL        (RE_TOKEN_BASE + 17) /* "^" */
#define RE_TOKEN_AT_EOL        (RE_TOKEN_BASE + 18) /* "$" */
#define RE_TOKEN_AT_SOI        (RE_TOKEN_BASE + 19) /* "\`" */
#define RE_TOKEN_AT_EOI        (RE_TOKEN_BASE + 20) /* "\'" */
#define RE_TOKEN_AT_WOB        (RE_TOKEN_BASE + 21) /* "\b" */
#define RE_TOKEN_AT_WOB_NOT    (RE_TOKEN_BASE + 22) /* "\B" */
#define RE_TOKEN_AT_SOW        (RE_TOKEN_BASE + 23) /* "\<" */
#define RE_TOKEN_AT_EOW        (RE_TOKEN_BASE + 24) /* "\>" */
#define RE_TOKEN_AT_SOS        (RE_TOKEN_BASE + 25) /* "\_<" */
#define RE_TOKEN_AT_EOS        (RE_TOKEN_BASE + 26) /* "\_>" */
#define RE_TOKEN_AT_MAX        RE_TOKEN_AT_EOS
#define RE_TOKEN_BKREF_1       (RE_TOKEN_BASE + 27) /* "\1" */
#define RE_TOKEN_BKREF_2       (RE_TOKEN_BASE + 28) /* "\2" */
#define RE_TOKEN_BKREF_3       (RE_TOKEN_BASE + 29) /* "\3" */
#define RE_TOKEN_BKREF_4       (RE_TOKEN_BASE + 30) /* "\4" */
#define RE_TOKEN_BKREF_5       (RE_TOKEN_BASE + 31) /* "\5" */
#define RE_TOKEN_BKREF_6       (RE_TOKEN_BASE + 32) /* "\6" */
#define RE_TOKEN_BKREF_7       (RE_TOKEN_BASE + 33) /* "\7" */
#define RE_TOKEN_BKREF_8       (RE_TOKEN_BASE + 34) /* "\8" */
#define RE_TOKEN_BKREF_9       (RE_TOKEN_BASE + 35) /* "\9" */
#define RE_TOKEN_UNMATCHED_BK  (RE_TOKEN_BASE + 36) /* "\" (followed by EOF; for `RE_EESCAPE') */

/* Parse and yield the next regex-token pointed-to by `self->rep_pos'.
 * @return: * : A unicode character, or one of `RE_TOKEN_*' */
typedef __ATTR_WUNUSED_T __ATTR_NONNULL_T((1)) re_token_t
__NOTHROW_NCX_T(LIBREGEX_CC *PRE_PARSER_YIELD)(struct re_parser *__restrict self);
#ifdef LIBREGEX_WANT_PROTOTYPES
LIBREGEX_DECL __ATTR_WUNUSED __ATTR_NONNULL((1)) re_token_t
__NOTHROW_NCX(LIBREGEX_CC re_parser_yield)(struct re_parser *__restrict self);
#define re_parser_yieldat(self, pos) ((self)->rep_pos = (pos), re_parser_yield(self))
#endif /* LIBREGEX_WANT_PROTOTYPES */



struct re_code {
	__byte_t   rc_fmap[256]; /* Fast map: take the first byte of the string to match as index:
	                          * - rc_fmap[input[0]] == 0xff --> input will never match
	                          * - rc_fmap[input[0]] != 0xff --> Start executing at `PC = rc_code + rc_fmap[input[0]]' */
	__size_t   rc_minmatch;  /* The smallest input length that can be matched by `rc_code' (or `0' when `rc_code' can match epsilon) */
	__size_t   rc_maxmatch;  /* The largest input length that can be matched by `rc_code' (or `SIZE_MAX' when `rc_code' can match an infinitely long input string) */
	__uint16_t rc_ngrps;     /* # of groups currently defined */
	__uint16_t rc_nvars;     /* # of variables referenced by code */
	__COMPILER_FLEXIBLE_ARRAY(__byte_t, rc_code); /* Code buffer (instructions) */
};


/* Regex compiler structure */
struct re_compiler {
	struct re_parser rec_parser;     /* Underlying parser. */
	__byte_t        *rec_cbase;      /* [?..1][<= rec_cend][owned] Base-pointer for output code-buffer. (points at `struct re_code') */
	__byte_t        *rec_estart;     /* [?..1][<= rec_cpos] Start-pointer for current expression's code. */
	__byte_t        *rec_cpos;       /* [?..1][>= rec_cbase && <= rec_cend] Pointer to next unset opcode in code-buffer. */
	__byte_t        *rec_cend;       /* [?..1][>= rec_cbase] End-pointer for output code-buffer. */
	__uint16_t       rec_ngrp;       /* # of groups currently defined / id of next group */
	__uint16_t       rec_nvar;       /* # of variables referenced by code / id of next unreferenced variable */
	__uint8_t        rec_grpinfo[9]; /* Information about the first 9 groups (for back-references); each is a set of `RE_COMPILER_GRPINFO_*' */
#define RE_COMPILER_GRPINFO_DEFINED 0x01 /* Group has been defined */
#define RE_COMPILER_GRPINFO_EPSILON 0x02 /* Group contents are able to match epsilon (for `REOP_GROUP_MATCH_Jn') */
};

#define re_compiler_init(self, pattern, syntax)                  \
	(void)(re_parser_init(&(self)->rec_parser, pattern, syntax), \
	       (self)->rec_cbase = (self)->rec_estart = __NULLPTR,   \
	       (self)->rec_cpos = (self)->rec_cend = __NULLPTR,      \
	       (self)->rec_ngrp = (self)->rec_nvar = 0,              \
	       __libc_bzero((self)->rec_grpinfo, sizeof((self)->rec_grpinfo)))
#define re_compiler_fini(self) \
	(void)__libc_free((self)->rec_cbase)
#ifdef LIBREGEX_WANT_PROTOTYPES
#define re_compiler_yield(self)        re_parser_yield(&(self)->rec_parser)
#define re_compiler_yieldat(self, pos) re_parser_yieldat(&(self)->rec_parser, pos)
#endif /* LIBREGEX_WANT_PROTOTYPES */

/* Parse and compile the pattern given to `self' to generate code.
 * Even upon error, `self' remains  in a valid state (except  that
 * you're  not allowed to  call `re_compiler_compile()' again), so
 * that the caller has to invoke `re_compiler_fini()' in order  to
 * perform cleanup.
 * Upon success, members of `self' are initialized as:
 * - *rec_parser.rep_pos    == '\0'
 * - rec_parser.rep_pos     == strend(rec_parser.rep_pat)
 * - rec_parser.rep_syntax  == <unchanged>
 * - rec_parser.rec_cbase   == <pointer-to-struct re_code>
 * - rec_parser.rec_estart  == <undefined>
 * - rec_parser.rec_cpos    == <undefined>
 * - rec_parser.rec_cend    == <code-end-pointer (1 past the `REOP_MATCHED[_PERFECT]' opcode)>
 * - rec_parser.rec_ngrp    == <greaters-referenced-group + 1>
 * - rec_parser.rec_nvar    == <greaters-referenced-var + 1>
 * - rec_parser.rec_grpinfo == <undefined>
 * @return: RE_NOERROR: Success
 * @return: RE_BADPAT:  General pattern syntax error.
 * @return: RE_ECTYPE:  Invalid/unknown character class name.
 * @return: RE_EESCAPE: Trailing backslash.
 * @return: RE_ESUBREG: Invalid back reference.
 * @return: RE_EBRACK:  Unmatched '['.
 * @return: RE_EPAREN:  Unmatched '('.
 * @return: RE_EBRACE:  Unmatched '{'.
 * @return: RE_BADBR:   Invalid contents of '{...}'.
 * @return: RE_ERANGE:  Invalid range end (e.g. '[z-a]').
 * @return: RE_ESPACE:  Out of memory.
 * @return: RE_BADRPT:  Nothing is preceding '+', '*', '?' or '{'.
 * @return: RE_EEND:    Unexpected end of pattern.
 * @return: RE_ESIZE:   Compiled pattern bigger than 2^16 bytes.
 * @return: RE_ERPAREN: Unmatched ')' (only when `RE_SYNTAX_UNMATCHED_RIGHT_PAREN_ORD' was set) */
typedef __ATTR_WUNUSED_T __ATTR_NONNULL_T((1)) re_errno_t
__NOTHROW_NCX_T(LIBREGEX_CC *PRE_COMPILER_COMPILE)(struct re_compiler *__restrict self);
#ifdef LIBREGEX_WANT_PROTOTYPES
LIBREGEX_DECL __ATTR_WUNUSED __ATTR_NONNULL((1)) re_errno_t
__NOTHROW_NCX(LIBREGEX_CC re_compiler_compile)(struct re_compiler *__restrict self);
#endif /* LIBREGEX_WANT_PROTOTYPES */



__DECL_END

#endif /* !_LIBREGEX_REGCOMP_H */