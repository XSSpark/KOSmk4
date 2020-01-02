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
#ifndef _BITS_TIMEVAL_CXX_SUPPORT_H
#define _BITS_TIMEVAL_CXX_SUPPORT_H 1

#include <__stdinc.h>
#include <features.h>

#if defined(__cplusplus) && defined(__USE_KOS)
#include <__stdcxx.h>

#include <kos/anno.h>

#ifndef __USECS_PER_SEC
#define __USECS_PER_SEC __UINTPTR_C(1000000)
#endif /* !__USECS_PER_SEC */

#define __TIMEVAL_CXX_DECL_BEGIN extern "C++" {
#define __TIMEVAL_CXX_DECL_END }
#define __TIMEVAL_CXX_SUPPORT(T, TV_SEC_TYPE, TV_USEC_TYPE)                \
	/* Add microseconds (1/1_000_000 seconds) */                           \
	__CXX_CLASSMEMBER __NOBLOCK void                                       \
	add_microseconds(TV_USEC_TYPE __n) __CXX_NOEXCEPT {                    \
		tv_usec += __n;                                                    \
		if (tv_usec > __USECS_PER_SEC) {                                   \
			tv_sec += tv_usec / __USECS_PER_SEC;                           \
			tv_usec %= __USECS_PER_SEC;                                    \
		}                                                                  \
	}                                                                      \
	/* Add milliseconds (1/1_000 seconds) */                               \
	__CXX_CLASSMEMBER __NOBLOCK void                                       \
	add_milliseconds(TV_USEC_TYPE __n) __CXX_NOEXCEPT {                    \
		(add_microseconds)(__n * 1000);                                    \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T &                                        \
	operator+=(T const &__other) __CXX_NOEXCEPT {                          \
		tv_usec += __other.tv_usec;                                        \
		if (tv_usec > __USECS_PER_SEC) {                                   \
			tv_sec += tv_usec / __USECS_PER_SEC;                           \
			tv_usec %= __USECS_PER_SEC;                                    \
		}                                                                  \
		tv_sec += __other.tv_sec;                                          \
		return *this;                                                      \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T &                                        \
	operator-=(T const &__other) __CXX_NOEXCEPT {                          \
		bool __is_negative;                                                \
		tv_sec -= __other.tv_sec;                                          \
		__is_negative = tv_usec < __other.tv_usec;                         \
		tv_usec -= __other.tv_usec;                                        \
		if (__is_negative) {                                               \
			tv_sec -= (__UINTPTR_C(0) - tv_usec) / __USECS_PER_SEC;        \
			tv_usec = (__UINTPTR_C(0) - tv_usec) % __USECS_PER_SEC;        \
		}                                                                  \
		return *this;                                                      \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T &                                        \
	operator*=(unsigned int __n) __CXX_NOEXCEPT {                          \
		tv_sec *= __n;                                                     \
		tv_usec *= __n;                                                    \
		if (tv_usec > __USECS_PER_SEC) {                                   \
			tv_sec += tv_usec / __USECS_PER_SEC;                           \
			tv_usec %= __USECS_PER_SEC;                                    \
		}                                                                  \
		return *this;                                                      \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T &                                        \
	operator/=(unsigned int __n) __THROWS(E_DIVIDE_BY_ZERO) {              \
		tv_sec /= __n;                                                     \
		tv_usec /= __n;                                                    \
		return *this;                                                      \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T &                                        \
	operator%=(unsigned int __n) __THROWS(E_DIVIDE_BY_ZERO) {              \
		tv_sec %= __n;                                                     \
		tv_usec %= __n;                                                    \
		return *this;                                                      \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T                                          \
	operator+(T const &__other) const __CXX_NOEXCEPT {                     \
		T __result = *this;                                                \
		__result += __other;                                               \
		return __result;                                                   \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T                                          \
	operator-(T const &__other) const __CXX_NOEXCEPT {                     \
		T __result = *this;                                                \
		__result -= __other;                                               \
		return __result;                                                   \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T                                          \
	operator*(unsigned int __n) const __CXX_NOEXCEPT {                     \
		T __result = *this;                                                \
		__result *= __n;                                                   \
		return __result;                                                   \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T                                          \
	operator/(unsigned int __n) const __THROWS(E_DIVIDE_BY_ZERO) {         \
		T __result = *this;                                                \
		__result /= __n;                                                   \
		return __result;                                                   \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK T                                          \
	operator%(unsigned int __n) const __THROWS(E_DIVIDE_BY_ZERO) {         \
		T __result = *this;                                                \
		__result %= __n;                                                   \
		return __result;                                                   \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK bool                                       \
	operator==(T const &__other) __CXX_NOEXCEPT {                          \
		return (tv_sec == __other.tv_sec && tv_usec == __other.tv_usec);   \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK bool                                       \
	operator!=(T const &__other) __CXX_NOEXCEPT {                          \
		return (tv_sec != __other.tv_sec || tv_usec != __other.tv_usec);   \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK bool                                       \
	operator<(T const &__other) __CXX_NOEXCEPT {                           \
		return ((tv_sec < __other.tv_sec) ||                               \
		        (tv_sec == __other.tv_sec && tv_usec < __other.tv_usec));  \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK bool                                       \
	operator<=(T const &__other) __CXX_NOEXCEPT {                          \
		return ((tv_sec < __other.tv_sec) ||                               \
		        (tv_sec == __other.tv_sec && tv_usec <= __other.tv_usec)); \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK bool                                       \
	operator>(T const &__other) __CXX_NOEXCEPT {                           \
		return ((tv_sec > __other.tv_sec) ||                               \
		        (tv_sec == __other.tv_sec && tv_usec > __other.tv_usec));  \
	}                                                                      \
	__CXX_CLASSMEMBER __NOBLOCK bool                                       \
	operator>=(T const &__other) __CXX_NOEXCEPT {                          \
		return ((tv_sec > __other.tv_sec) ||                               \
		        (tv_sec == __other.tv_sec && tv_usec >= __other.tv_usec)); \
	}

#define __TIMEVAL_CXX_SUPPORT2(T, TV_SEC_TYPE, TV_USEC_TYPE)    \
	__CXX_CLASSMEMBER __NOBLOCK T                               \
	operator*(unsigned int __n, T const &__tmv)__CXX_NOEXCEPT { \
		return __tmv * __n;                                     \
	}

#else
#define __TIMEVAL_CXX_DECL_BEGIN                             /* nothing */
#define __TIMEVAL_CXX_DECL_END                               /* nothing */
#define __TIMEVAL_CXX_SUPPORT(T, TV_SEC_TYPE, TV_USEC_TYPE)  /* nothing */
#define __TIMEVAL_CXX_SUPPORT2(T, TV_SEC_TYPE, TV_USEC_TYPE) /* nothing */
#endif

#endif /* !_BITS_TIMEVAL_CXX_SUPPORT_H */
