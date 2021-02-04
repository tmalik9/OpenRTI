/* -*-c++-*- OpenRTI - Copyright (C) 2004-2015 Mathias Froehlich
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

#include "Mutex.h"

#include "MutexPrivateDataWin32.h"

#ifndef __CPlusPlusStd
#error "must include OpenRTIConfig.h!"
#endif

namespace OpenRTI {

#if __CPlusPlusStd < 201103L

Mutex::Mutex() :
  _privateData(new PrivateData)
{
}

Mutex::~Mutex()
{
  delete _privateData;
  _privateData = 0;
}

void
Mutex::lock()
{
  _privateData->lock();
}

void
Mutex::unlock()
{
  _privateData->unlock();
}

#endif

} // namespace OpenRTI
