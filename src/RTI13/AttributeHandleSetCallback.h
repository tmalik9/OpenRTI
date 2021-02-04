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

#ifndef AttributeHandleSetCallback_h
#define AttributeHandleSetCallback_h

/// The original RTI13 include
#include <RTI.hh>

#include "Handle.h"

class OPENRTI_LOCAL AttributeHandleSetCallback : public RTI::AttributeHandleSet {
public:
  AttributeHandleSetCallback(const std::vector<OpenRTI::AttributeHandle>& attributeHandleVector);
  virtual ~AttributeHandleSetCallback();

  virtual RTI::ULong size() const;
  virtual RTI::AttributeHandle getHandle(RTI::ULong index) const;
  virtual void add(RTI::AttributeHandle handle);
  virtual void remove(RTI::AttributeHandle handle);
  virtual void empty();
  virtual RTI::Boolean isEmpty() const;
  virtual RTI::Boolean isMember(RTI::AttributeHandle handle) const;

private:
  const std::vector<OpenRTI::AttributeHandle>& _attributeHandleVector;
};

#endif
