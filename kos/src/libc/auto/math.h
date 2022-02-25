/* HASH CRC-32:0x3992c396 */
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
#ifndef GUARD_LIBC_AUTO_MATH_H
#define GUARD_LIBC_AUTO_MATH_H 1

#include "../api.h"

#include <hybrid/typecore.h>
#include <kos/types.h>
#include <math.h>

DECL_BEGIN

#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc cosine of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_acos)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc cosine of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_acos)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc sine of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_asin)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc sine of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_asin)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc tangent of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_atan)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc tangent of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_atan)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc tangent of `y / x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_atan2)(double y, double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc tangent of `y / x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_atan2)(double y, double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Cosine of `x' */
INTDEF WUNUSED __DECL_SIMD_cos double NOTHROW(LIBDCALL libd_cos)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Cosine of `x' */
INTDEF WUNUSED __DECL_SIMD_cos double NOTHROW(LIBCCALL libc_cos)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Sine of `x' */
INTDEF WUNUSED __DECL_SIMD_sin double NOTHROW(LIBDCALL libd_sin)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Sine of `x' */
INTDEF WUNUSED __DECL_SIMD_sin double NOTHROW(LIBCCALL libc_sin)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Tangent of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_tan)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Tangent of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_tan)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc cosine of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_acosf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc cosine of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_acosf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc sine of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_asinf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc sine of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_asinf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc tangent of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_atanf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc tangent of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_atanf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc tangent of `y / x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_atan2f)(float y, float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc tangent of `y / x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_atan2f)(float y, float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Cosine of `x' */
INTDEF WUNUSED __DECL_SIMD_cosf float NOTHROW(LIBDCALL libd_cosf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Cosine of `x' */
INTDEF WUNUSED __DECL_SIMD_cosf float NOTHROW(LIBCCALL libc_cosf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Sine of `x' */
INTDEF WUNUSED __DECL_SIMD_sinf float NOTHROW(LIBDCALL libd_sinf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Sine of `x' */
INTDEF WUNUSED __DECL_SIMD_sinf float NOTHROW(LIBCCALL libc_sinf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Tangent of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_tanf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Tangent of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_tanf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc cosine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_acosl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc cosine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_acosl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc sine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_asinl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc sine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_asinl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc tangent of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_atanl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc tangent of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_atanl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Arc tangent of `y / x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_atan2l)(__LONGDOUBLE y, __LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Arc tangent of `y / x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_atan2l)(__LONGDOUBLE y, __LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Cosine of `x' */
INTDEF WUNUSED __DECL_SIMD_cosl __LONGDOUBLE NOTHROW(LIBDCALL libd_cosl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Cosine of `x' */
INTDEF WUNUSED __DECL_SIMD_cosl __LONGDOUBLE NOTHROW(LIBCCALL libc_cosl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Sine of `x' */
INTDEF WUNUSED __DECL_SIMD_sinl __LONGDOUBLE NOTHROW(LIBDCALL libd_sinl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Sine of `x' */
INTDEF WUNUSED __DECL_SIMD_sinl __LONGDOUBLE NOTHROW(LIBCCALL libc_sinl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Tangent of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_tanl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Tangent of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_tanl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic cosine of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_cosh)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic cosine of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_cosh)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic sine of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_sinh)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic sine of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_sinh)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic tangent of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_tanh)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic tangent of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_tanh)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic cosine of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_coshf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic cosine of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_coshf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic sine of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_sinhf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic sine of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_sinhf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic tangent of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_tanhf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic tangent of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_tanhf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic cosine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_coshl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic cosine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_coshl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic sine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_sinhl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic sine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_sinhl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic tangent of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_tanhl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic tangent of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_tanhl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc cosine of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_acosh)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc cosine of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_acosh)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc sine of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_asinh)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc sine of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_asinh)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc tangent of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_atanh)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc tangent of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_atanh)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc cosine of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_acoshf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc cosine of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_acoshf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc sine of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_asinhf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc sine of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_asinhf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc tangent of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_atanhf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc tangent of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_atanhf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc cosine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_acoshl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc cosine of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_acoshl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc sine of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_asinhl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc sine of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_asinhl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Hyperbolic arc tangent of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_atanhl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Hyperbolic arc tangent of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_atanhl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Exponential function of `x' */
INTDEF WUNUSED __DECL_SIMD_exp double NOTHROW(LIBDCALL libd_exp)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Exponential function of `x' */
INTDEF WUNUSED __DECL_SIMD_exp double NOTHROW(LIBCCALL libc_exp)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Break `value' into a normalized fraction and an integral power of 2 */
INTDEF NONNULL((2)) double NOTHROW_NCX(LIBDCALL libd_frexp)(double x, int *pexponent);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Break `value' into a normalized fraction and an integral power of 2 */
INTDEF NONNULL((2)) double NOTHROW_NCX(LIBCCALL libc_frexp)(double x, int *pexponent);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* `x' times (two to the `exponent' power) */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_ldexp)(double x, int exponent);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* `x' times (two to the `exponent' power) */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_ldexp)(double x, int exponent);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Natural logarithm of `x' */
INTDEF WUNUSED __DECL_SIMD_log double NOTHROW(LIBDCALL libd_log)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Natural logarithm of `x' */
INTDEF WUNUSED __DECL_SIMD_log double NOTHROW(LIBCCALL libc_log)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Base-ten logarithm of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_log10)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Base-ten logarithm of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_log10)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Break `value' into integral and fractional parts */
INTDEF WUNUSED NONNULL((2)) double NOTHROW_NCX(LIBDCALL libd_modf)(double x, double *iptr);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Break `value' into integral and fractional parts */
INTDEF WUNUSED NONNULL((2)) double NOTHROW_NCX(LIBCCALL libc_modf)(double x, double *iptr);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Exponential function of `x' */
INTDEF WUNUSED __DECL_SIMD_expf float NOTHROW(LIBDCALL libd_expf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Exponential function of `x' */
INTDEF WUNUSED __DECL_SIMD_expf float NOTHROW(LIBCCALL libc_expf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Break `value' into a normalized fraction and an integral power of 2 */
INTDEF NONNULL((2)) float NOTHROW_NCX(LIBDCALL libd_frexpf)(float x, int *pexponent);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Break `value' into a normalized fraction and an integral power of 2 */
INTDEF NONNULL((2)) float NOTHROW_NCX(LIBCCALL libc_frexpf)(float x, int *pexponent);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* `x' times (two to the `exponent' power) */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_ldexpf)(float x, int exponent);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* `x' times (two to the `exponent' power) */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_ldexpf)(float x, int exponent);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Natural logarithm of `x' */
INTDEF WUNUSED __DECL_SIMD_logf float NOTHROW(LIBDCALL libd_logf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Natural logarithm of `x' */
INTDEF WUNUSED __DECL_SIMD_logf float NOTHROW(LIBCCALL libc_logf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Base-ten logarithm of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_log10f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Base-ten logarithm of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_log10f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Break `value' into integral and fractional parts */
INTDEF WUNUSED NONNULL((2)) float NOTHROW_NCX(LIBDCALL libd_modff)(float x, float *iptr);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Break `value' into integral and fractional parts */
INTDEF WUNUSED NONNULL((2)) float NOTHROW_NCX(LIBCCALL libc_modff)(float x, float *iptr);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Exponential function of `x' */
INTDEF WUNUSED __DECL_SIMD_expl __LONGDOUBLE NOTHROW(LIBDCALL libd_expl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Exponential function of `x' */
INTDEF WUNUSED __DECL_SIMD_expl __LONGDOUBLE NOTHROW(LIBCCALL libc_expl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Break `value' into a normalized fraction and an integral power of 2 */
INTDEF NONNULL((2)) __LONGDOUBLE NOTHROW_NCX(LIBDCALL libd_frexpl)(__LONGDOUBLE x, int *pexponent);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Break `value' into a normalized fraction and an integral power of 2 */
INTDEF NONNULL((2)) __LONGDOUBLE NOTHROW_NCX(LIBCCALL libc_frexpl)(__LONGDOUBLE x, int *pexponent);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* `x' times (two to the `exponent' power) */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_ldexpl)(__LONGDOUBLE x, int exponent);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* `x' times (two to the `exponent' power) */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_ldexpl)(__LONGDOUBLE x, int exponent);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Natural logarithm of `x' */
INTDEF WUNUSED __DECL_SIMD_logl __LONGDOUBLE NOTHROW(LIBDCALL libd_logl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Natural logarithm of `x' */
INTDEF WUNUSED __DECL_SIMD_logl __LONGDOUBLE NOTHROW(LIBCCALL libc_logl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Base-ten logarithm of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_log10l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Base-ten logarithm of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_log10l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Break `value' into integral and fractional parts */
INTDEF WUNUSED NONNULL((2)) __LONGDOUBLE NOTHROW_NCX(LIBDCALL libd_modfl)(__LONGDOUBLE x, __LONGDOUBLE *iptr);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Break `value' into integral and fractional parts */
INTDEF WUNUSED NONNULL((2)) __LONGDOUBLE NOTHROW_NCX(LIBCCALL libc_modfl)(__LONGDOUBLE x, __LONGDOUBLE *iptr);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `exp(x) - 1' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_expm1)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `exp(x) - 1' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_expm1)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `log(1 + x)' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_log1p)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `log(1 + x)' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_log1p)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the base 2 signed integral exponent of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_logb)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the base 2 signed integral exponent of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_logb)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `exp(x) - 1' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_expm1f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `exp(x) - 1' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_expm1f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `log(1 + x)' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_log1pf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `log(1 + x)' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_log1pf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the base 2 signed integral exponent of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_logbf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the base 2 signed integral exponent of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_logbf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `exp(x) - 1' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_expm1l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `exp(x) - 1' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_expm1l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `log(1 + x)' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_log1pl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `log(1 + x)' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_log1pl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the base 2 signed integral exponent of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_logbl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the base 2 signed integral exponent of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_logbl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Compute base-2 exponential of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_exp2)(double x);
/* Compute base-2 logarithm of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_log2)(double x);
/* Compute base-2 exponential of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_exp2f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Compute base-2 exponential of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_exp2f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Compute base-2 logarithm of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_log2f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Compute base-2 logarithm of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_log2f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Compute base-2 exponential of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_exp2l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Compute base-2 exponential of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_exp2l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Compute base-2 logarithm of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_log2l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Compute base-2 logarithm of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_log2l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' to the `y' power */
INTDEF WUNUSED __DECL_SIMD_pow double NOTHROW(LIBDCALL libd_pow)(double x, double y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' to the `y' power */
INTDEF WUNUSED __DECL_SIMD_pow double NOTHROW(LIBCCALL libc_pow)(double x, double y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the square root of `x' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_sqrt)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the square root of `x' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_sqrt)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' to the `y' power */
INTDEF WUNUSED __DECL_SIMD_powf float NOTHROW(LIBDCALL libd_powf)(float x, float y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' to the `y' power */
INTDEF WUNUSED __DECL_SIMD_powf float NOTHROW(LIBCCALL libc_powf)(float x, float y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the square root of `x' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_sqrtf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the square root of `x' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_sqrtf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' to the `y' power */
INTDEF WUNUSED __DECL_SIMD_powl __LONGDOUBLE NOTHROW(LIBDCALL libd_powl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' to the `y' power */
INTDEF WUNUSED __DECL_SIMD_powl __LONGDOUBLE NOTHROW(LIBCCALL libc_powl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the square root of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_sqrtl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the square root of `x' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_sqrtl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `sqrt(x*x + y*y)' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_hypot)(double x, double y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `sqrt(x*x + y*y)' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_hypot)(double x, double y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `sqrt(x*x + y*y)' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_hypotf)(float x, float y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `sqrt(x*x + y*y)' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_hypotf)(float x, float y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `sqrt(x*x + y*y)' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_hypotl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `sqrt(x*x + y*y)' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_hypotl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the cube root of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_cbrt)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the cube root of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_cbrt)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the cube root of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_cbrtf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the cube root of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_cbrtf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the cube root of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_cbrtl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the cube root of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_cbrtl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Smallest integral value not less than `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_ceil)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Smallest integral value not less than `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_ceil)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Absolute value of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_fabs)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Absolute value of `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_fabs)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Largest integer not greater than `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_floor)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Largest integer not greater than `x' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_floor)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Floating-point modulo remainder of `x / y' */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_fmod)(double x, double y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Floating-point modulo remainder of `x / y' */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_fmod)(double x, double y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Smallest integral value not less than `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_ceilf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Smallest integral value not less than `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_ceilf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Absolute value of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_fabsf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Absolute value of `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_fabsf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Largest integer not greater than `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_floorf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Largest integer not greater than `x' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_floorf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Floating-point modulo remainder of `x / y' */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_fmodf)(float x, float y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Floating-point modulo remainder of `x / y' */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_fmodf)(float x, float y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Smallest integral value not less than `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_ceill)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Smallest integral value not less than `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_ceill)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Absolute value of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_fabsl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Absolute value of `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_fabsl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Largest integer not greater than `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_floorl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Largest integer not greater than `x' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_floorl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Floating-point modulo remainder of `x / y' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_fmodl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Floating-point modulo remainder of `x / y' */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_fmodl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' with its signed changed to `y's */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_copysign)(double num, double sign);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' with its signed changed to `y's */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_copysign)(double num, double sign);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return representation of qNaN for double type */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_nan)(char const *tagb);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return representation of qNaN for double type */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_nan)(char const *tagb);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' with its signed changed to `y's */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_copysignf)(float num, float sign);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' with its signed changed to `y's */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_copysignf)(float num, float sign);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return representation of qNaN for double type */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_nanf)(char const *tagb);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return representation of qNaN for double type */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_nanf)(char const *tagb);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' with its signed changed to `y's */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_copysignl)(__LONGDOUBLE num, __LONGDOUBLE sign);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' with its signed changed to `y's */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_copysignl)(__LONGDOUBLE num, __LONGDOUBLE sign);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return representation of qNaN for double type */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_nanl)(char const *tagb);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return representation of qNaN for double type */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_nanl)(char const *tagb);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_erf)(double x);
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_erfc)(double x);
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_lgamma)(double x);
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_erff)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_erff)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_erfcf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_erfcf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_lgammaf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_lgammaf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_erfl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_erfl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_erfcl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_erfcl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_lgammal)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_lgammal)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* True gamma function */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_tgamma)(double x);
/* True gamma function */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_tgammaf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* True gamma function */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_tgammaf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* True gamma function */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_tgammal)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* True gamma function */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_tgammal)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the integer nearest `x' in the direction of the prevailing rounding mode */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_rint)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the integer nearest `x' in the direction of the prevailing rounding mode */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_rint)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x + epsilon' if `x < y', and `x - epsilon' if `x > y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_nextafter)(double x, double y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x + epsilon' if `x < y', and `x - epsilon' if `x > y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_nextafter)(double x, double y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the remainder of integer division `x / p' with infinite precision */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_remainder)(double x, double p);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the remainder of integer division `x / p' with infinite precision */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_remainder)(double x, double p);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the binary exponent of `x', which must be nonzero */
INTDEF WUNUSED int NOTHROW(LIBDCALL libd_ilogb)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the binary exponent of `x', which must be nonzero */
INTDEF WUNUSED int NOTHROW(LIBCCALL libc_ilogb)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the integer nearest `x' in the direction of the prevailing rounding mode */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_rintf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the integer nearest `x' in the direction of the prevailing rounding mode */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_rintf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x + epsilon' if `x < y', and `x - epsilon' if `x > y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_nextafterf)(float x, float y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x + epsilon' if `x < y', and `x - epsilon' if `x > y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_nextafterf)(float x, float y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the remainder of integer division `x / p' with infinite precision */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_remainderf)(float x, float p);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the remainder of integer division `x / p' with infinite precision */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_remainderf)(float x, float p);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the binary exponent of `x', which must be nonzero */
INTDEF WUNUSED int NOTHROW(LIBDCALL libd_ilogbf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the binary exponent of `x', which must be nonzero */
INTDEF WUNUSED int NOTHROW(LIBCCALL libc_ilogbf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the integer nearest `x' in the direction of the prevailing rounding mode */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_rintl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the integer nearest `x' in the direction of the prevailing rounding mode */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_rintl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x + epsilon' if `x < y', and `x - epsilon' if `x > y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_nextafterl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x + epsilon' if `x < y', and `x - epsilon' if `x > y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_nextafterl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the remainder of integer division `x / p' with infinite precision */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_remainderl)(__LONGDOUBLE x, __LONGDOUBLE p);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the remainder of integer division `x / p' with infinite precision */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_remainderl)(__LONGDOUBLE x, __LONGDOUBLE p);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the binary exponent of `x', which must be nonzero */
INTDEF WUNUSED int NOTHROW(LIBDCALL libd_ilogbl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the binary exponent of `x', which must be nonzero */
INTDEF WUNUSED int NOTHROW(LIBCCALL libc_ilogbl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_nexttoward)(double x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_nexttoward)(double x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_scalbn)(double x, int n);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_scalbn)(double x, int n);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_scalbln)(double x, long int n);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_scalbln)(double x, long int n);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_round)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_round)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to the integral value in floating-point
 * format   nearest  but  not  larger  in  magnitude */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_trunc)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to the integral value in floating-point
 * format   nearest  but  not  larger  in  magnitude */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_trunc)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Compute remainder of `x' and `p' and put in `*pquo' a value with
 * sign of x/p and magnitude  congruent `mod 2^n' to the  magnitude
 * of the integral quotient x/p, with n >= 3 */
INTDEF WUNUSED NONNULL((3)) double NOTHROW(LIBDCALL libd_remquo)(double x, double p, int *pquo);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Compute remainder of `x' and `p' and put in `*pquo' a value with
 * sign of x/p and magnitude  congruent `mod 2^n' to the  magnitude
 * of the integral quotient x/p, with n >= 3 */
INTDEF WUNUSED NONNULL((3)) double NOTHROW(LIBCCALL libc_remquo)(double x, double p, int *pquo);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBDCALL libd_lrint)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBCCALL libc_lrint)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBDCALL libd_lround)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBCCALL libc_lround)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return positive difference between `x' and `y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_fdim)(double x, double y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return positive difference between `x' and `y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_fdim)(double x, double y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return maximum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_fmax)(double x, double y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return maximum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_fmax)(double x, double y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return minimum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_fmin)(double x, double y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return minimum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_fmin)(double x, double y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Multiply-add function computed as a ternary operation */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_fma)(double x, double y, double z);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Multiply-add function computed as a ternary operation */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_fma)(double x, double y, double z);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBDCALL libd_llrint)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBCCALL libc_llrint)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBDCALL libd_llround)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBCCALL libc_llround)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_nexttowardf)(float x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_nexttowardf)(float x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_scalbnf)(float x, int n);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_scalbnf)(float x, int n);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_scalblnf)(float x, long int n);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_scalblnf)(float x, long int n);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_roundf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_roundf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to the integral value in floating-point
 * format   nearest  but  not  larger  in  magnitude */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_truncf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to the integral value in floating-point
 * format   nearest  but  not  larger  in  magnitude */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_truncf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Compute remainder of `x' and `p' and put in `*pquo' a value with
 * sign of x/p and magnitude  congruent `mod 2^n' to the  magnitude
 * of the integral quotient x/p, with n >= 3 */
INTDEF WUNUSED NONNULL((3)) float NOTHROW(LIBDCALL libd_remquof)(float x, float p, int *pquo);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Compute remainder of `x' and `p' and put in `*pquo' a value with
 * sign of x/p and magnitude  congruent `mod 2^n' to the  magnitude
 * of the integral quotient x/p, with n >= 3 */
INTDEF WUNUSED NONNULL((3)) float NOTHROW(LIBCCALL libc_remquof)(float x, float p, int *pquo);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBDCALL libd_lrintf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBCCALL libc_lrintf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBDCALL libd_lroundf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBCCALL libc_lroundf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return positive difference between `x' and `y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_fdimf)(float x, float y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return positive difference between `x' and `y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_fdimf)(float x, float y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return maximum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_fmaxf)(float x, float y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return maximum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_fmaxf)(float x, float y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return minimum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_fminf)(float x, float y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return minimum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_fminf)(float x, float y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Multiply-add function computed as a ternary operation */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_fmaf)(float x, float y, float z);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Multiply-add function computed as a ternary operation */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_fmaf)(float x, float y, float z);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBDCALL libd_llrintf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBCCALL libc_llrintf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBDCALL libd_llroundf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBCCALL libc_llroundf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_scalbnl)(__LONGDOUBLE x, int n);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_scalbnl)(__LONGDOUBLE x, int n);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_scalblnl)(__LONGDOUBLE x, long int n);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_scalblnl)(__LONGDOUBLE x, long int n);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_roundl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_roundl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to the integral value in floating-point
 * format   nearest  but  not  larger  in  magnitude */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_truncl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to the integral value in floating-point
 * format   nearest  but  not  larger  in  magnitude */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_truncl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Compute remainder of `x' and `p' and put in `*pquo' a value with
 * sign of x/p and magnitude  congruent `mod 2^n' to the  magnitude
 * of the integral quotient x/p, with n >= 3 */
INTDEF WUNUSED NONNULL((3)) __LONGDOUBLE NOTHROW(LIBDCALL libd_remquol)(__LONGDOUBLE x, __LONGDOUBLE p, int *pquo);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Compute remainder of `x' and `p' and put in `*pquo' a value with
 * sign of x/p and magnitude  congruent `mod 2^n' to the  magnitude
 * of the integral quotient x/p, with n >= 3 */
INTDEF WUNUSED NONNULL((3)) __LONGDOUBLE NOTHROW(LIBCCALL libc_remquol)(__LONGDOUBLE x, __LONGDOUBLE p, int *pquo);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBDCALL libd_lrintl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBCCALL libc_lrintl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBDCALL libd_lroundl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED long int NOTHROW(LIBCCALL libc_lroundl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return positive difference between `x' and `y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_fdiml)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return positive difference between `x' and `y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_fdiml)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return maximum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_fmaxl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return maximum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_fmaxl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return minimum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_fminl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return minimum numeric value from `x' and `y' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_fminl)(__LONGDOUBLE x, __LONGDOUBLE y);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Multiply-add function computed as a ternary operation */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_fmal)(__LONGDOUBLE x, __LONGDOUBLE y, __LONGDOUBLE z);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Multiply-add function computed as a ternary operation */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_fmal)(__LONGDOUBLE x, __LONGDOUBLE y, __LONGDOUBLE z);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBDCALL libd_llrintl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value according to current rounding direction */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBCCALL libc_llrintl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBDCALL libd_llroundl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Round `x' to nearest integral value, rounding halfway cases away from zero */
INTDEF ATTR_CONST WUNUSED __LONGLONG NOTHROW(LIBCCALL libc_llroundl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Cosine and sine of `x' */
INTDEF __DECL_SIMD_sincos NONNULL((2, 3)) void NOTHROW(LIBDCALL libd_sincos)(double x, double *psinx, double *pcosx);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Cosine and sine of `x' */
INTDEF __DECL_SIMD_sincos NONNULL((2, 3)) void NOTHROW(LIBCCALL libc_sincos)(double x, double *psinx, double *pcosx);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* A function missing in all standards: compute exponent to base ten */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_exp10)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* A function missing in all standards: compute exponent to base ten */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_exp10)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Another name occasionally used */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_pow10)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Another name occasionally used */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_pow10)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Cosine and sine of `x' */
INTDEF __DECL_SIMD_sincosf NONNULL((2, 3)) void NOTHROW(LIBDCALL libd_sincosf)(float x, float *psinx, float *pcosx);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Cosine and sine of `x' */
INTDEF __DECL_SIMD_sincosf NONNULL((2, 3)) void NOTHROW(LIBCCALL libc_sincosf)(float x, float *psinx, float *pcosx);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_exp10f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_exp10f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Another name occasionally used */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_pow10f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Another name occasionally used */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_pow10f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Cosine and sine of `x' */
INTDEF __DECL_SIMD_sincosl NONNULL((2, 3)) void NOTHROW(LIBDCALL libd_sincosl)(__LONGDOUBLE x, __LONGDOUBLE *psinx, __LONGDOUBLE *pcosx);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Cosine and sine of `x' */
INTDEF __DECL_SIMD_sincosl NONNULL((2, 3)) void NOTHROW(LIBCCALL libc_sincosl)(__LONGDOUBLE x, __LONGDOUBLE *psinx, __LONGDOUBLE *pcosx);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_exp10l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_exp10l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Another name occasionally used */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_pow10l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Another name occasionally used */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_pow10l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return 0 if `value' is finite or NaN, +1 if it is +Infinity, -1 if it is -Infinity */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_isinf)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return 0 if `value' is finite or NaN, +1 if it is +Infinity, -1 if it is -Infinity */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_isinf)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return 0 if `value' is finite or NaN, +1 if it is +Infinity, -1 if it is -Infinity */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_isinff)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return 0 if `value' is finite or NaN, +1 if it is +Infinity, -1 if it is -Infinity */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_isinff)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return 0 if `value' is finite or NaN, +1 if it is +Infinity, -1 if it is -Infinity */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_isinfl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return 0 if `value' is finite or NaN, +1 if it is +Infinity, -1 if it is -Infinity */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_isinfl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return nonzero if `value' is finite and not NaN */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_finite)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return nonzero if `value' is finite and not NaN */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_finite)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the fractional part of `x' after dividing out `ilogb(x)' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBDCALL libd_significand)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the fractional part of `x' after dividing out `ilogb(x)' */
INTDEF ATTR_CONST WUNUSED double NOTHROW(LIBCCALL libc_significand)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return nonzero if `value' is finite and not NaN */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_finitef)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return nonzero if `value' is finite and not NaN */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_finitef)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the fractional part of `x' after dividing out `ilogb(x)' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBDCALL libd_significandf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the fractional part of `x' after dividing out `ilogb(x)' */
INTDEF ATTR_CONST WUNUSED float NOTHROW(LIBCCALL libc_significandf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return nonzero if `value' is finite and not NaN */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_finitel)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return nonzero if `value' is finite and not NaN */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_finitel)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return the fractional part of `x' after dividing out `ilogb(x)' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_significandl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return the fractional part of `x' after dividing out `ilogb(x)' */
INTDEF ATTR_CONST WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_significandl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return nonzero if `value' is not a number */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_isnan)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return nonzero if `value' is not a number */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_isnan)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return nonzero if `value' is not a number */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_isnanf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return nonzero if `value' is not a number */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_isnanf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return nonzero if `value' is not a number */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd_isnanl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return nonzero if `value' is not a number */
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc_isnanl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_j0)(double x);
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_j1)(double x);
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_jn)(int n, double x);
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_y0)(double x);
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_y1)(double x);
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_yn)(int n, double x);
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_j0f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_j0f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_j1f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_j1f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_jnf)(int n, float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_jnf)(int n, float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_y0f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_y0f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_y1f)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_y1f)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_ynf)(int n, float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_ynf)(int n, float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_j0l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_j0l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_j1l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_j1l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_jnl)(int n, __LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_jnl)(int n, __LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_y0l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_y0l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_y1l)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_y1l)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_ynl)(int n, __LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_ynl)(int n, __LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Reentrant version of lgamma. This function uses the global variable
 * `signgam'. The reentrant version instead takes a pointer and stores
 * the value through it */
INTDEF WUNUSED double NOTHROW_NCX(LIBDCALL libd_lgamma_r)(double x, int *signgamp);
/* Reentrant version of lgamma. This function uses the global variable
 * `signgam'. The reentrant version instead takes a pointer and stores
 * the value through it */
INTDEF WUNUSED float NOTHROW_NCX(LIBDCALL libd_lgammaf_r)(float x, int *signgamp);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Reentrant version of lgamma. This function uses the global variable
 * `signgam'. The reentrant version instead takes a pointer and stores
 * the value through it */
INTDEF WUNUSED float NOTHROW_NCX(LIBCCALL libc_lgammaf_r)(float x, int *signgamp);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Reentrant version of lgamma. This function uses the global variable
 * `signgam'. The reentrant version instead takes a pointer and stores
 * the value through it */
INTDEF WUNUSED __LONGDOUBLE NOTHROW_NCX(LIBDCALL libd_lgammal_r)(__LONGDOUBLE x, int *signgamp);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Reentrant version of lgamma. This function uses the global variable
 * `signgam'. The reentrant version instead takes a pointer and stores
 * the value through it */
INTDEF WUNUSED __LONGDOUBLE NOTHROW_NCX(LIBCCALL libc_lgammal_r)(__LONGDOUBLE x, int *signgamp);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF WUNUSED double NOTHROW(LIBDCALL libd_scalb)(double x, double fn);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF WUNUSED double NOTHROW(LIBCCALL libc_scalb)(double x, double fn);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF WUNUSED float NOTHROW(LIBDCALL libd_scalbf)(float x, float fn);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF WUNUSED float NOTHROW(LIBCCALL libc_scalbf)(float x, float fn);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
/* Return `x' times (2 to the Nth power) */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBDCALL libd_scalbl)(__LONGDOUBLE x, __LONGDOUBLE fn);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
/* Return `x' times (2 to the Nth power) */
INTDEF WUNUSED __LONGDOUBLE NOTHROW(LIBCCALL libc_scalbl)(__LONGDOUBLE x, __LONGDOUBLE fn);
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___fpclassify)(double x);
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___fpclassify)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___signbit)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___signbit)(double x);
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___fpclassifyf)(float x);
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___fpclassifyf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___signbitf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___signbitf)(float x);
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___fpclassifyl)(__LONGDOUBLE x);
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___fpclassifyl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___signbitl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___signbitl)(__LONGDOUBLE x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___issignaling)(double x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___issignaling)(double x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___issignalingf)(float x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___issignalingf)(float x);
#endif /* !__KERNEL__ */
#if !defined(__LIBCCALL_IS_LIBDCALL) && !defined(__KERNEL__)
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBDCALL libd___issignalingl)(__LONGDOUBLE x);
#endif /* !__LIBCCALL_IS_LIBDCALL && !__KERNEL__ */
#ifndef __KERNEL__
INTDEF ATTR_CONST WUNUSED int NOTHROW(LIBCCALL libc___issignalingl)(__LONGDOUBLE x);
INTDEF ATTR_PURE WUNUSED NONNULL((1)) short NOTHROW_NCX(LIBDCALL libd__dtest)(double __KOS_FIXED_CONST *px);
INTDEF ATTR_PURE WUNUSED NONNULL((1)) short NOTHROW_NCX(LIBCCALL libc__dtest)(double __KOS_FIXED_CONST *px);
INTDEF ATTR_PURE WUNUSED NONNULL((1)) short NOTHROW_NCX(LIBDCALL libd__fdtest)(float __KOS_FIXED_CONST *px);
INTDEF ATTR_PURE WUNUSED NONNULL((1)) short NOTHROW_NCX(LIBCCALL libc__fdtest)(float __KOS_FIXED_CONST *px);
INTDEF ATTR_PURE WUNUSED NONNULL((1)) short NOTHROW_NCX(LIBDCALL libd__ldtest)(__LONGDOUBLE __KOS_FIXED_CONST *px);
INTDEF ATTR_PURE WUNUSED NONNULL((1)) short NOTHROW_NCX(LIBCCALL libc__ldtest)(__LONGDOUBLE __KOS_FIXED_CONST *px);
#endif /* !__KERNEL__ */

DECL_END

#endif /* !GUARD_LIBC_AUTO_MATH_H */
