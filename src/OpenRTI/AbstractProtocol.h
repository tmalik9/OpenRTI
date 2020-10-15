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

#ifndef OpenRTI_AbstractProtocol_h
#define OpenRTI_AbstractProtocol_h

#include "AbstractConnect.h"
#include "StringUtils.h"

namespace OpenRTI {

class Clock;

class OPENRTI_API AbstractProtocol : public Referenced {
public:
  AbstractProtocol() noexcept = default;
  AbstractProtocol(const AbstractProtocol&) noexcept = default;
  AbstractProtocol(AbstractProtocol&&) noexcept = default;
  AbstractProtocol& operator=(const AbstractProtocol&) noexcept = default;
  AbstractProtocol& operator=(AbstractProtocol&&) noexcept = default;
  virtual ~AbstractProtocol() noexcept
  { }

  virtual SharedPtr<AbstractConnect> connect(const StringStringListMap& clientOptions, const Clock& abstime) const = 0;
};

} // namespace OpenRTI

#endif
