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

// This time, the first include is above the api include.
// the rti1516/Exception header misses that.
#include <iosfwd>

#include <RTI/HLAfloat64TimeFactory.h>

#include <memory>
#include <RTI/LogicalTimeFactory.h>
#include <RTI/HLAfloat64Interval.h>
#include <RTI/HLAfloat64Time.h>

HLAfloat64TimeFactory::HLAfloat64TimeFactory()
{
}

HLAfloat64TimeFactory::~HLAfloat64TimeFactory()
{
}

std::unique_ptr<rti1516::LogicalTime>
HLAfloat64TimeFactory::makeLogicalTime()
{
  return std::unique_ptr<rti1516::LogicalTime>(new HLAfloat64Time);
}

std::unique_ptr<rti1516::LogicalTimeInterval>
HLAfloat64TimeFactory::makeLogicalTimeInterval()
{
  return std::unique_ptr<rti1516::LogicalTimeInterval>(new HLAfloat64Interval);
}

