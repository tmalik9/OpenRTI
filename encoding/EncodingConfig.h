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
#include <cstdint>

namespace OpenRTI
{
   // Platform Specific Typedefs
#if defined(_WIN32)
   typedef int8_t      Integer8;
   typedef int16_t     Integer16;
   typedef int32_t     Integer32;
   typedef int64_t     Integer64;
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

