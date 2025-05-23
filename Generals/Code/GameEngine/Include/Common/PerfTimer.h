/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
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

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// PerfTimer.h ////////////////////////////////////////////////////////////////////////////////////
// John McDonald
// July 2002

#pragma once



#include <cstdint>

#include <intrin.h>


#define cpuid(regs, cpuid_type) __cpuid(reinterpret_cast<int *>(regs), cpuid_type)





#define NO_PERF_TIMERS

// Forward Declarations
class DebugDisplayInterface;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#define NO_USE_QPF	// non-QPF is much faster.

#define DECLARE_PERF_TIMER(id)					
#define USE_PERF_TIMER(id)
#define IGNORE_PERF_TIMER(id)	
