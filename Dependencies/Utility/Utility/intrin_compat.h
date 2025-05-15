/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 TheSuperHackers
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// This file contains macros to help compiling on non-windows platforms and VC6 compatibility macros.
#pragma once



#include <cstdint>

#if !defined(_lrotl) && !defined(_WIN32)
static inline uint32_t _lrotl(uint32_t value, int shift)
{
#if defined(__has_builtin) && __has_builtin(__builtin_rotateleft32)
    return __builtin_rotateleft32(value, shift);
#else
    shift &= 31;
    return ((value << shift) | (value >> (32 - shift)));
#endif
}
#endif

#ifndef _rdtsc
#ifdef _WIN32
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif // _WIN32
#endif // _rdtsc
#ifndef _rdtsc
static inline uint64_t _rdtsc()
{
#ifdef _WIN32
    return __rdtsc();
#elif defined(__has_builtin) && __has_builtin(__builtin_readcyclecounter)
    return __builtin_readcyclecounter();
#elif defined(__has_builtin) && __has_builtin(__builtin_ia32_rdtsc)
    return __builtin_ia32_rdtsc();
#else
#error "No implementation for _rdtsc"
#endif
}
#endif // _rdtsc
 
#ifdef _WIN32
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)
#elif defined(__has_builtin)
    #if __has_builtin(__builtin_return_address)
    static inline uintptr_t _ReturnAddress()
    {
        return reinterpret_cast<uintptr_t>(__builtin_return_address(0));
    }
    #else
    #error "No implementation for _ReturnAddress"
    #endif
#else
#error "No implementation for _ReturnAddress"
#endif

#if defined(__has_builtin) 
    #if  __has_builtin(__builtin_debugtrap)
    #define __debugbreak() __builtin_debugtrap()
    #elif __has_builtin(__builtin_trap)
    #define __debugbreak() __builtin_trap()
    #else
    #error "No implementation for __debugbreak"
    #endif
#elif !defined(_MSC_VER)
#error "No implementation for __debugbreak"
#endif

#ifndef cpuid
#if (defined _M_IX86 || defined _M_X64 || defined __i386__ || defined __amd64__)
#ifdef _WIN32
#include <intrin.h>
#define cpuid(regs, cpuid_type) __cpuid(reinterpret_cast<int *>(regs), cpuid_type)
#elif (defined __clang__ || defined __GNUC__)
#include <cpuid.h>
#define cpuid(regs, cpuid_type) __cpuid(cpuid_type, regs[0], regs[1], regs[2], regs[3])
#endif
#endif // (defined _M_IX86 || defined _M_X64 || defined __i386__ || defined __amd64__)
#endif // cpuid
#ifndef cpuid
/* Just return 0 for everything if its not x86 or as fallback */
#include <string.h>
#define cpuid(regs, cpuid_type) memset(regs, 0, 16)
#endif // cpuid

