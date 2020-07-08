/* -*-c++-*- OpenRTI - Copyright (C) 2009-2012 Mathias Froehlich
 *
 * This file is part of OpenRTI.
 *
 * OpenRTI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * OpenRTI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenRTI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <utility>
#include <vector>


namespace OpenRTI
{
   // Platform Specific Typedefs
#if defined(_WIN32)
   typedef char      Integer8;
   typedef short     Integer16;
   typedef int       Integer32;
#if defined(__GNUC__)
   // gcc extension, but is 64 bits on any win32 target
   typedef long long Integer64;
#else
   typedef __int64   Integer64;
#endif
#else
#if defined(RTI_USE_64BIT_LONGS)
   typedef char      Integer8;
   typedef short     Integer16;
   typedef int       Integer32;
   typedef long      Integer64;
#else
   typedef char      Integer8;
   typedef short     Integer16;
   typedef int       Integer32;
   typedef long long Integer64;
#endif
#endif

   typedef Integer8  Octet;
   typedef std::pair< Octet, Octet > OctetPair;
}

