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
#ifndef _NETINET_IP6_H
#define _NETINET_IP6_H 1

/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
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

#include <__stdinc.h>
#include <inttypes.h>
#include <netinet/in.h>

__SYSDECL_BEGIN

#ifdef __CC__
struct ip6_hdr {
	union {
		struct ip6_hdrctl {
			uint32_t      ip6_un1_flow; /* 4 bits version, 8 bits TC, 20 bits flow-ID */
			uint16_t      ip6_un1_plen; /* payload length */
			uint8_t       ip6_un1_nxt;  /* next header */
			uint8_t       ip6_un1_hlim; /* hop limit */
		}                 ip6_un1;
		uint8_t           ip6_un2_vfc;  /* 4 bits version, top 4 bits tclass */
	}                     ip6_ctlun;
	struct in6_addr       ip6_src;      /* source address */
	struct in6_addr       ip6_dst;      /* destination address */
#define ip6_vfc  ip6_ctlun.ip6_un2_vfc
#define ip6_flow ip6_ctlun.ip6_un1.ip6_un1_flow
#define ip6_plen ip6_ctlun.ip6_un1.ip6_un1_plen
#define ip6_nxt  ip6_ctlun.ip6_un1.ip6_un1_nxt
#define ip6_hlim ip6_ctlun.ip6_un1.ip6_un1_hlim
#define ip6_hops ip6_ctlun.ip6_un1.ip6_un1_hlim
};
#endif /* __CC__ */

/* Generic extension header. */
#ifdef __CC__
struct ip6_ext {
	uint8_t  ip6e_nxt; /* next header. */
	uint8_t  ip6e_len; /* length in units of 8 octets. */
};
#endif /* __CC__ */

/* Hop-by-Hop options header. */
#ifdef __CC__
struct ip6_hbh {
	uint8_t  ip6h_nxt; /* next header. */
	uint8_t  ip6h_len; /* length in units of 8 octets. */
	/* followed by options */
};
#endif /* __CC__ */

/* Destination options header */
#ifdef __CC__
struct ip6_dest {
	uint8_t  ip6d_nxt; /* next header */
	uint8_t  ip6d_len; /* length in units of 8 octets */
	/* followed by options */
};
#endif /* __CC__ */

/* Routing header */
#ifdef __CC__
struct ip6_rthdr {
	uint8_t ip6r_nxt;     /* next header */
	uint8_t ip6r_len;     /* length in units of 8 octets */
	uint8_t ip6r_type;    /* routing type */
	uint8_t ip6r_segleft; /* segments left */
	/* followed by routing type specific data */
};
#endif /* __CC__ */

/* Type 0 Routing header */
#ifdef __CC__
struct ip6_rthdr0 {
	uint8_t  ip6r0_nxt;      /* next header */
	uint8_t  ip6r0_len;      /* length in units of 8 octets */
	uint8_t  ip6r0_type;     /* always zero */
	uint8_t  ip6r0_segleft;  /* segments left */
	uint8_t  ip6r0_reserved; /* reserved field */
	uint8_t  ip6r0_slmap[3]; /* strict/loose bit map */
	/* followed by up to 127 struct in6_addr */
	__COMPILER_FLEXIBLE_ARRAY(struct in6_addr, ip6r0_addr);
};
#endif /* __CC__ */

/* Fragment header */
#ifdef __CC__
struct ip6_frag {
	uint8_t   ip6f_nxt;      /* next header */
	uint8_t   ip6f_reserved; /* reserved field */
	uint16_t  ip6f_offlg;    /* offset, reserved, and flag */
	uint32_t  ip6f_ident;    /* identification */
};
#endif /* __CC__ */

#if __BYTE_ORDER == __BIG_ENDIAN
#   define IP6F_OFF_MASK       0xfff8  /* mask out offset from _offlg */
#   define IP6F_RESERVED_MASK  0x0006  /* reserved bits in ip6f_offlg */
#   define IP6F_MORE_FRAG      0x0001  /* more-fragments flag */
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#   define IP6F_OFF_MASK       0xf8ff  /* mask out offset from _offlg */
#   define IP6F_RESERVED_MASK  0x0600  /* reserved bits in ip6f_offlg */
#   define IP6F_MORE_FRAG      0x0100  /* more-fragments flag */
#endif

/* IPv6 options */
#ifdef __CC__
struct ip6_opt {
	uint8_t  ip6o_type;
	uint8_t  ip6o_len;
};
#endif /* __CC__ */

/* The high-order 3 bits of the option type define the behavior
 * when processing an unknown option and whether or not the option
 * content changes in flight. */
#define IP6OPT_TYPE(o)       ((o) & 0xc0)
#define IP6OPT_TYPE_SKIP       0x00
#define IP6OPT_TYPE_DISCARD    0x40
#define IP6OPT_TYPE_FORCEICMP  0x80
#define IP6OPT_TYPE_ICMP       0xc0
#define IP6OPT_TYPE_MUTABLE    0x20
/* Special option types for padding. */
#define IP6OPT_PAD1            0
#define IP6OPT_PADN            1
#define IP6OPT_JUMBO           0xc2
#define IP6OPT_NSAP_ADDR       0xc3
#define IP6OPT_TUNNEL_LIMIT    0x04
#define IP6OPT_ROUTER_ALERT    0x05

/* Jumbo Payload Option */
#ifdef __CC__
struct ip6_opt_jumbo {
	uint8_t  ip6oj_type;
	uint8_t  ip6oj_len;
	uint8_t  ip6oj_jumbo_len[4];
};
#endif /* __CC__ */
#define IP6OPT_JUMBO_LEN    6

/* NSAP Address Option */
#ifdef __CC__
struct ip6_opt_nsap {
	uint8_t  ip6on_type;
	uint8_t  ip6on_len;
	uint8_t  ip6on_src_nsap_len;
	uint8_t  ip6on_dst_nsap_len;
	/* followed by source NSAP */
	/* followed by destination NSAP */
};
#endif /* __CC__ */

/* Tunnel Limit Option */
#ifdef __CC__
struct ip6_opt_tunnel {
	uint8_t  ip6ot_type;
	uint8_t  ip6ot_len;
	uint8_t  ip6ot_encap_limit;
};
#endif /* __CC__ */

/* Router Alert Option */
#ifdef __CC__
struct ip6_opt_router {
	uint8_t  ip6or_type;
	uint8_t  ip6or_len;
	uint8_t  ip6or_value[2];
};
#endif /* __CC__ */

/* Router alert values (in network byte order) */
#if __BYTE_ORDER == __BIG_ENDIAN
#   define IP6_ALERT_MLD  0x0000
#   define IP6_ALERT_RSVP 0x0001
#   define IP6_ALERT_AN   0x0002
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#   define IP6_ALERT_MLD  0x0000
#   define IP6_ALERT_RSVP 0x0100
#   define IP6_ALERT_AN   0x0200
#endif

__SYSDECL_END

#endif /* !_NETINET_IP6_H */
