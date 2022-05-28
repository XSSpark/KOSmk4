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
#ifndef GUARD_LIBC_USER_ARPA_NAMESER_C
#define GUARD_LIBC_USER_ARPA_NAMESER_C 1

#include "../api.h"
/**/

#include "arpa.nameser.h"

DECL_BEGIN





/*[[[head:libc_ns_initparse,hash:CRC-32=0x4c977503]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_initparse)(u_char const *a,
                                        int b,
                                        ns_msg *c)
/*[[[body:libc_ns_initparse]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	CRT_UNIMPLEMENTEDF("ns_initparse(%p, %x, %p)", a, b, c); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_initparse]]]*/

/*[[[head:libc_ns_skiprr,hash:CRC-32=0xe57afc59]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_skiprr)(u_char const *a,
                                     u_char const *b,
                                     ns_sect c,
                                     int d)
/*[[[body:libc_ns_skiprr]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	CRT_UNIMPLEMENTEDF("ns_skiprr(%p, %p, %x, %x)", a, b, c, d); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_skiprr]]]*/

/*[[[head:libc_ns_parserr,hash:CRC-32=0x80e5fe8e]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_parserr)(ns_msg *a,
                                      ns_sect b,
                                      int c,
                                      ns_rr *d)
/*[[[body:libc_ns_parserr]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	CRT_UNIMPLEMENTEDF("ns_parserr(%p, %x, %x, %p)", a, b, c, d); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_parserr]]]*/

/*[[[head:libc_ns_sprintrr,hash:CRC-32=0xa83c0980]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_sprintrr)(ns_msg const *a,
                                       ns_rr const *b,
                                       char const *c,
                                       char const *d,
                                       char *e,
                                       size_t f)
/*[[[body:libc_ns_sprintrr]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	CRT_UNIMPLEMENTEDF("ns_sprintrr(%p, %p, %q, %q, %q, %Ix)", a, b, c, d, e, f); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_sprintrr]]]*/

/*[[[head:libc_ns_sprintrrf,hash:CRC-32=0xbeb7b190]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_sprintrrf)(u_char const *a,
                                        size_t,
                                        char const *b,
                                        ns_class c,
                                        ns_type d,
                                        u_long e,
                                        u_char const *f,
                                        size_t g,
                                        char const *h,
                                        char const *i,
                                        char *j,
                                        size_t k)
/*[[[body:libc_ns_sprintrrf]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	(void)h;
	(void)i;
	(void)j;
	(void)k;
	CRT_UNIMPLEMENTED("ns_sprintrrf"); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_sprintrrf]]]*/

/*[[[head:libc_ns_format_ttl,hash:CRC-32=0x325671db]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_format_ttl)(u_long a,
                                         char *b,
                                         size_t c)
/*[[[body:libc_ns_format_ttl]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	CRT_UNIMPLEMENTEDF("ns_format_ttl(%lx, %q, %Ix)", a, b, c); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_format_ttl]]]*/

/*[[[head:libc_ns_parse_ttl,hash:CRC-32=0x844f1cd]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_parse_ttl)(char const *a,
                                        u_long *b)
/*[[[body:libc_ns_parse_ttl]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	CRT_UNIMPLEMENTEDF("ns_parse_ttl(%q, %p)", a, b); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_parse_ttl]]]*/

/*[[[head:libc_ns_datetosecs,hash:CRC-32=0x5a3e4342]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") u_int32_t
NOTHROW_NCX(LIBCCALL libc_ns_datetosecs)(char const *a,
                                         int *b)
/*[[[body:libc_ns_datetosecs]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	CRT_UNIMPLEMENTEDF("ns_datetosecs(%q, %p)", a, b); /* TODO */
	return (u_int32_t)libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_datetosecs]]]*/

/*[[[head:libc_ns_name_ntol,hash:CRC-32=0xc5dd8ee9]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_name_ntol)(u_char const *a,
                                        u_char *b,
                                        size_t c)
/*[[[body:libc_ns_name_ntol]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	CRT_UNIMPLEMENTEDF("ns_name_ntol(%p, %p, %Ix)", a, b, c); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_ntol]]]*/

/*[[[head:libc_ns_name_ntop,hash:CRC-32=0x7c461946]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_name_ntop)(u_char const *a,
                                        char *b,
                                        size_t c)
/*[[[body:libc_ns_name_ntop]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	CRT_UNIMPLEMENTEDF("ns_name_ntop(%p, %q, %Ix)", a, b, c); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_ntop]]]*/

/*[[[head:libc_ns_name_pton,hash:CRC-32=0xfb1fb8ff]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_name_pton)(char const *a,
                                        u_char *b,
                                        size_t c)
/*[[[body:libc_ns_name_pton]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	CRT_UNIMPLEMENTEDF("ns_name_pton(%q, %p, %Ix)", a, b, c); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_pton]]]*/

/*[[[head:libc_ns_name_unpack,hash:CRC-32=0xe54016ee]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_name_unpack)(u_char const *a,
                                          u_char const *b,
                                          u_char const *c,
                                          u_char *d,
                                          size_t e)
/*[[[body:libc_ns_name_unpack]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	CRT_UNIMPLEMENTEDF("ns_name_unpack(%p, %p, %p, %p, %Ix)", a, b, c, d, e); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_unpack]]]*/

/*[[[head:libc_ns_name_pack,hash:CRC-32=0xd2c8b9d]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_name_pack)(u_char const *a,
                                        u_char *b,
                                        int c,
                                        u_char const **d,
                                        u_char const **e)
/*[[[body:libc_ns_name_pack]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	CRT_UNIMPLEMENTEDF("ns_name_pack(%p, %p, %x, %p, %p)", a, b, c, d, e); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_pack]]]*/

/*[[[head:libc_ns_name_uncompress,hash:CRC-32=0x1196ce71]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_name_uncompress)(u_char const *a,
                                              u_char const *b,
                                              u_char const *c,
                                              char *d,
                                              size_t e)
/*[[[body:libc_ns_name_uncompress]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	CRT_UNIMPLEMENTEDF("ns_name_uncompress(%p, %p, %p, %q, %Ix)", a, b, c, d, e); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_uncompress]]]*/


/*[[[head:libc_ns_name_skip,hash:CRC-32=0xf2aba1e4]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") NONNULL((2)) int
NOTHROW_NCX(LIBCCALL libc_ns_name_skip)(u_char const **p_msg_ptr,
                                        u_char const *msg_end)
/*[[[body:libc_ns_name_skip]]]*/
/*AUTO*/{
	(void)p_msg_ptr;
	(void)msg_end;
	CRT_UNIMPLEMENTEDF("ns_name_skip(%p, %p)", p_msg_ptr, msg_end); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_skip]]]*/

/*[[[head:libc_ns_name_rollback,hash:CRC-32=0x56a66857]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") void
NOTHROW_NCX(LIBCCALL libc_ns_name_rollback)(u_char const *a,
                                            u_char const **b,
                                            u_char const **c)
/*[[[body:libc_ns_name_rollback]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	CRT_UNIMPLEMENTEDF("ns_name_rollback(%p, %p, %p)", a, b, c); /* TODO */
	libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_name_rollback]]]*/

/*[[[head:libc_ns_sign,hash:CRC-32=0x8fc2f95d]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_sign)(u_char *a,
                                   int *b,
                                   int c,
                                   int d,
                                   void *e,
                                   u_char const *f,
                                   int g,
                                   u_char *h,
                                   int *i,
                                   time_t j)
/*[[[body:libc_ns_sign]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	(void)h;
	(void)i;
	(void)j;
	CRT_UNIMPLEMENTEDF("ns_sign(%p, %p, %x, %x, %p, %p, %x, %p, %p, %" PRIxN(__SIZEOF_TIME32_T__) ")", a, b, c, d, e, f, g, h, i, j); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_sign]]]*/

/*[[[head:libc_ns_sign2,hash:CRC-32=0x7e66dc7d]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_sign2)(u_char *a,
                                    int *b,
                                    int c,
                                    int d,
                                    void *e,
                                    u_char const *f,
                                    int g,
                                    u_char *h,
                                    int *i,
                                    time_t j,
                                    u_char **k,
                                    u_char **l)
/*[[[body:libc_ns_sign2]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	(void)h;
	(void)i;
	(void)j;
	(void)k;
	(void)l;
	CRT_UNIMPLEMENTEDF("ns_sign2(%p, %p, %x, %x, %p, %p, %x, %p, %p, %" PRIxN(__SIZEOF_TIME32_T__) ", %p, %p)", a, b, c, d, e, f, g, h, i, j, k, l); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_sign2]]]*/

/*[[[head:libc_ns_sign_tcp,hash:CRC-32=0x528f70fc]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_sign_tcp)(u_char *a,
                                       int *b,
                                       int c,
                                       int d,
                                       ns_tcp_tsig_state *e,
                                       int f)
/*[[[body:libc_ns_sign_tcp]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	CRT_UNIMPLEMENTEDF("ns_sign_tcp(%p, %p, %x, %x, %p, %x)", a, b, c, d, e, f); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_sign_tcp]]]*/

/*[[[head:libc_ns_sign_tcp2,hash:CRC-32=0x2061c43e]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_sign_tcp2)(u_char *a,
                                        int *b,
                                        int c,
                                        int d,
                                        ns_tcp_tsig_state *e,
                                        int f,
                                        u_char **g,
                                        u_char **h)
/*[[[body:libc_ns_sign_tcp2]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	(void)h;
	CRT_UNIMPLEMENTEDF("ns_sign_tcp2(%p, %p, %x, %x, %p, %x, %p, %p)", a, b, c, d, e, f, g, h); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_sign_tcp2]]]*/

/*[[[head:libc_ns_sign_tcp_init,hash:CRC-32=0x45fff6aa]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_sign_tcp_init)(void *a,
                                            u_char const *b,
                                            int c,
                                            ns_tcp_tsig_state *d)
/*[[[body:libc_ns_sign_tcp_init]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	CRT_UNIMPLEMENTEDF("ns_sign_tcp_init(%p, %p, %x, %p)", a, b, c, d); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_sign_tcp_init]]]*/

/*[[[head:libc_ns_find_tsig,hash:CRC-32=0x639210fb]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") u_char *
NOTHROW_NCX(LIBCCALL libc_ns_find_tsig)(u_char *a,
                                        u_char *b)
/*[[[body:libc_ns_find_tsig]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	CRT_UNIMPLEMENTEDF("ns_find_tsig(%p, %p)", a, b); /* TODO */
	libc_seterrno(ENOSYS);
	return NULL;
}
/*[[[end:libc_ns_find_tsig]]]*/

/*[[[head:libc_ns_verify,hash:CRC-32=0xc68996a5]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_verify)(u_char *a,
                                     int *b,
                                     void *c,
                                     u_char const *d,
                                     int e,
                                     u_char *f,
                                     int *g,
                                     time_t *h,
                                     int i)
/*[[[body:libc_ns_verify]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	(void)e;
	(void)f;
	(void)g;
	(void)h;
	(void)i;
	CRT_UNIMPLEMENTEDF("ns_verify(%p, %p, %p, %p, %x, %p, %p, %p, %x)", a, b, c, d, e, f, g, h, i); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_verify]]]*/

/*[[[head:libc_ns_verify_tcp,hash:CRC-32=0x4242cce6]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_verify_tcp)(u_char *a,
                                         int *b,
                                         ns_tcp_tsig_state *c,
                                         int d)
/*[[[body:libc_ns_verify_tcp]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	CRT_UNIMPLEMENTEDF("ns_verify_tcp(%p, %p, %p, %x)", a, b, c, d); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_verify_tcp]]]*/

/*[[[head:libc_ns_verify_tcp_init,hash:CRC-32=0x9084a54a]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_verify_tcp_init)(void *a,
                                              u_char const *b,
                                              int c,
                                              ns_tcp_tsig_state *d)
/*[[[body:libc_ns_verify_tcp_init]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	(void)d;
	CRT_UNIMPLEMENTEDF("ns_verify_tcp_init(%p, %p, %x, %p)", a, b, c, d); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_verify_tcp_init]]]*/

/*[[[head:libc_ns_samedomain,hash:CRC-32=0xf4c2bf41]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_samedomain)(char const *a,
                                         char const *b)
/*[[[body:libc_ns_samedomain]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	CRT_UNIMPLEMENTEDF("ns_samedomain(%q, %q)", a, b); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_samedomain]]]*/

/*[[[head:libc_ns_subdomain,hash:CRC-32=0xe9f12c81]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_subdomain)(char const *a,
                                        char const *b)
/*[[[body:libc_ns_subdomain]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	CRT_UNIMPLEMENTEDF("ns_subdomain(%q, %q)", a, b); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_subdomain]]]*/

/*[[[head:libc_ns_makecanon,hash:CRC-32=0x7f60efb1]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_makecanon)(char const *a,
                                        char *b,
                                        size_t c)
/*[[[body:libc_ns_makecanon]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	(void)c;
	CRT_UNIMPLEMENTEDF("ns_makecanon(%q, %q, %Ix)", a, b, c); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_makecanon]]]*/

/*[[[head:libc_ns_samename,hash:CRC-32=0x5c68fa51]]]*/
INTERN ATTR_SECTION(".text.crt.net.nameser") int
NOTHROW_NCX(LIBCCALL libc_ns_samename)(char const *a,
                                       char const *b)
/*[[[body:libc_ns_samename]]]*/
/*AUTO*/{
	(void)a;
	(void)b;
	CRT_UNIMPLEMENTEDF("ns_samename(%q, %q)", a, b); /* TODO */
	return libc_seterrno(ENOSYS);
}
/*[[[end:libc_ns_samename]]]*/

/*[[[start:exports,hash:CRC-32=0x871e2899]]]*/
DEFINE_PUBLIC_ALIAS(ns_initparse, libc_ns_initparse);
DEFINE_PUBLIC_ALIAS(ns_skiprr, libc_ns_skiprr);
DEFINE_PUBLIC_ALIAS(ns_parserr, libc_ns_parserr);
DEFINE_PUBLIC_ALIAS(ns_sprintrr, libc_ns_sprintrr);
DEFINE_PUBLIC_ALIAS(ns_sprintrrf, libc_ns_sprintrrf);
DEFINE_PUBLIC_ALIAS(ns_format_ttl, libc_ns_format_ttl);
DEFINE_PUBLIC_ALIAS(ns_parse_ttl, libc_ns_parse_ttl);
DEFINE_PUBLIC_ALIAS(ns_datetosecs, libc_ns_datetosecs);
DEFINE_PUBLIC_ALIAS(ns_name_ntol, libc_ns_name_ntol);
DEFINE_PUBLIC_ALIAS(ns_name_ntop, libc_ns_name_ntop);
DEFINE_PUBLIC_ALIAS(ns_name_pton, libc_ns_name_pton);
DEFINE_PUBLIC_ALIAS(ns_name_unpack, libc_ns_name_unpack);
DEFINE_PUBLIC_ALIAS(ns_name_pack, libc_ns_name_pack);
DEFINE_PUBLIC_ALIAS(ns_name_uncompress, libc_ns_name_uncompress);
DEFINE_PUBLIC_ALIAS(ns_name_skip, libc_ns_name_skip);
DEFINE_PUBLIC_ALIAS(ns_name_rollback, libc_ns_name_rollback);
DEFINE_PUBLIC_ALIAS(ns_sign, libc_ns_sign);
DEFINE_PUBLIC_ALIAS(ns_sign2, libc_ns_sign2);
DEFINE_PUBLIC_ALIAS(ns_sign_tcp, libc_ns_sign_tcp);
DEFINE_PUBLIC_ALIAS(ns_sign_tcp2, libc_ns_sign_tcp2);
DEFINE_PUBLIC_ALIAS(ns_sign_tcp_init, libc_ns_sign_tcp_init);
DEFINE_PUBLIC_ALIAS(ns_find_tsig, libc_ns_find_tsig);
DEFINE_PUBLIC_ALIAS(ns_verify, libc_ns_verify);
DEFINE_PUBLIC_ALIAS(ns_verify_tcp, libc_ns_verify_tcp);
DEFINE_PUBLIC_ALIAS(ns_verify_tcp_init, libc_ns_verify_tcp_init);
DEFINE_PUBLIC_ALIAS(ns_samedomain, libc_ns_samedomain);
DEFINE_PUBLIC_ALIAS(ns_subdomain, libc_ns_subdomain);
DEFINE_PUBLIC_ALIAS(ns_makecanon, libc_ns_makecanon);
DEFINE_PUBLIC_ALIAS(ns_samename, libc_ns_samename);
/*[[[end:exports]]]*/

DECL_END

#endif /* !GUARD_LIBC_USER_ARPA_NAMESER_C */
