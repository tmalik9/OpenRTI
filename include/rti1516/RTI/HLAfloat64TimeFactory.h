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

#ifndef RTI_HLAfloat64TimeFactory_h
#define RTI_HLAfloat64TimeFactory_h

#include "LogicalTimeFactory.h"

class HLAfloat64Interval;
class HLAfloat64Time;

class RTI_EXPORT_FEDTIME HLAfloat64TimeFactory : public rti1516::LogicalTimeFactory {
public:
  HLAfloat64TimeFactory();
  virtual ~HLAfloat64TimeFactory();
  virtual std::unique_ptr<rti1516::LogicalTime> makeLogicalTime();
  virtual std::unique_ptr<rti1516::LogicalTimeInterval> makeLogicalTimeInterval();
};

#endif
