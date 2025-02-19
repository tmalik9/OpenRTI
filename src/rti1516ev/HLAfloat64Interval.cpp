/* -*-c++-*- OpenRTI - Copyright (C) 2009-2015 Mathias Froehlich
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

#include "DebugNew.h"
#include "RTI/time/HLAfloat64Interval.h"

#include <cmath>
#include <limits>
#include <sstream>
#include "VariableLengthDataImplementation.h"
#include "ValueImplementation.h"
#include "RTI/time/HLAfloat64Time.h"

#ifndef __CPlusPlusStd
#error "must include OpenRTIConfig.h!"
#endif

namespace rti1516ev {

DECLARE_VALUE_IMPLEMENTATION(HLAfloat64IntervalImpl, double)

static const HLAfloat64Time& toHLAfloat64Time(const LogicalTime& logicalTime)
{
  const HLAfloat64Time* float64Time = dynamic_cast<const HLAfloat64Time*>(&logicalTime);
  if (!float64Time)
    throw InvalidLogicalTime(logicalTime.implementationName());
  return *float64Time;
}

static const HLAfloat64Interval& toHLAfloat64Interval(const LogicalTimeInterval& logicalTimeInterval)
{
  const HLAfloat64Interval* float64Interval = dynamic_cast<const HLAfloat64Interval*>(&logicalTimeInterval);
  if (!float64Interval)
    throw InvalidLogicalTimeInterval(logicalTimeInterval.implementationName());
  return *float64Interval;
}

static inline bool isNaN(const double& fedTime)
{
#if 1
  // Code that here ourselfs, compilers might decide to optimize isnan away
  // because of some assumtions based on optimization flags

  // Neat trick:
  // A nan has all bits in its exponent set and any bit in the mantissa.
  // So a nan's bitpattern int representation can be written as
  //  0x7f800000 + x
  // where x > 0.
  // That means compute:
  //   t = 0x7f800000 - (0x7f800000 + x) = -x
  // and look for the sign of t.
  // If the exponent is not 0x7f800000 (that is less than 0x7f800000), the
  // resulting t value will be >= 0.

  union {
    double d;
    uint64_t u;
  } u;
  u.d = fedTime;

  // remove the sign bit since in the above the sign bit is not set
  int64_t i = (u.u & 0x7fffffffffffffffll);
  // compute the above t
  int64_t t = 0x7ff0000000000000ll - i;
  return t < 0;

#elif defined _WIN32
  return 0 != _isnan(fedTime);
#elif defined(__sun) || defined(__hpux)
  return isnan(fedTime);
#else
  return std::isnan(fedTime);
#endif
}

static inline double nextAfter(const double& logicalTime, const double& direction)
{
  return std::nextafter(logicalTime, direction);
}

HLAfloat64Interval::HLAfloat64Interval() :
  _impl(0)
{
}

HLAfloat64Interval::HLAfloat64Interval(double time) :
  _impl(0)
{
  HLAfloat64IntervalImpl::setValue(_impl, time);
}

HLAfloat64Interval::HLAfloat64Interval(const LogicalTimeInterval& logicalTimeInterval) :
  _impl(0)
{
  HLAfloat64IntervalImpl::assign(_impl, toHLAfloat64Interval(logicalTimeInterval)._impl);
}

HLAfloat64Interval::HLAfloat64Interval(const HLAfloat64Interval& float64Interval) :
  _impl(0)
{
  HLAfloat64IntervalImpl::assign(_impl, float64Interval._impl);
}

HLAfloat64Interval::~HLAfloat64Interval() noexcept
{
  HLAfloat64IntervalImpl::putAndDelete(_impl);
}

void
HLAfloat64Interval::setZero() noexcept
{
  HLAfloat64IntervalImpl::setValue(_impl, 0);
}

bool
HLAfloat64Interval::isZero() const
{
  double value = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(value))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  return 0 == value;
}

void
HLAfloat64Interval::setEpsilon() noexcept
{
  HLAfloat64IntervalImpl::setValue(_impl, std::numeric_limits<double>::denorm_min());
}

bool
HLAfloat64Interval::isEpsilon() const
{
  double value = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(value))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  return value == std::numeric_limits<double>::denorm_min();
}

LogicalTimeInterval&
HLAfloat64Interval::operator=(const LogicalTimeInterval& logicalTimeInterval)
{
  HLAfloat64IntervalImpl::assign(_impl, toHLAfloat64Interval(logicalTimeInterval)._impl);
  return *this;
}

void
HLAfloat64Interval::setToDifference(const LogicalTime& minuend, const LogicalTime& subtrahend)
{
  HLAfloat64IntervalImpl::setValue(_impl, toHLAfloat64Time(minuend).getTime() - toHLAfloat64Time(subtrahend).getTime());
}

LogicalTimeInterval&
HLAfloat64Interval::operator+=(const LogicalTimeInterval& logicalTimeInterval)
{
  double interval = HLAfloat64IntervalImpl::getValue(toHLAfloat64Interval(logicalTimeInterval)._impl);
  if (isNaN(interval))
    throw rti1516ev::InvalidLogicalTimeInterval(L"Logical time interval is NaN!");
  double value = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(value))
    throw rti1516ev::InvalidLogicalTimeInterval(L"Logical time interval is NaN!");
  // Since we do not know which one is the interval and which one the value, assume the smaller one is the interval
  if (fabs(value) < fabs(interval))
    std::swap(value, interval);
  if (0 < interval) {
    double next = nextAfter(value, std::numeric_limits<double>::infinity());
    double sum = value + interval;
    value = std::max(sum, next);
    if (isNaN(value))
      throw rti1516ev::IllegalTimeArithmetic(L"Result of logical time interval operation is NaN!");
    HLAfloat64IntervalImpl::setValue(_impl, value);
  } else if (interval < 0) {
    double next = nextAfter(value, -std::numeric_limits<double>::infinity());
    double sum = value + interval;
    value = std::min(sum, next);
    if (isNaN(value))
      throw rti1516ev::IllegalTimeArithmetic(L"Result of logical time interval operation is NaN!");
    HLAfloat64IntervalImpl::setValue(_impl, value);
  } else /* if (interval == 0) */ {
    // Since we may have swapped the arguments above we may need to store something
    HLAfloat64IntervalImpl::setValue(_impl, value);
  }
  return *this;
}

LogicalTimeInterval&
HLAfloat64Interval::operator-=(const LogicalTimeInterval& logicalTimeInterval)
{
  double interval = HLAfloat64IntervalImpl::getValue(toHLAfloat64Interval(logicalTimeInterval)._impl);
  if (isNaN(interval))
    throw rti1516ev::InvalidLogicalTimeInterval(L"Logical time interval is NaN!");
  double value = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(value))
    throw rti1516ev::InvalidLogicalTimeInterval(L"Logical time interval is NaN!");
  // Since we do not know which one is the interval and which one the value, assume the smaller one is the interval
  if (fabs(value) < fabs(interval))
    std::swap(value, interval);
  if (0 < interval) {
    double next = nextAfter(value, -std::numeric_limits<double>::infinity());
    double sum = value - interval;
    value = std::min(sum, next);
    if (isNaN(value))
      throw rti1516ev::IllegalTimeArithmetic(L"Result of logical time interval operation is NaN!");
    HLAfloat64IntervalImpl::setValue(_impl, value);
  } else if (interval < 0) {
    double next = nextAfter(value, std::numeric_limits<double>::infinity());
    double sum = value - interval;
    value = std::max(sum, next);
    if (isNaN(value))
      throw rti1516ev::IllegalTimeArithmetic(L"Result of logical time interval operation is NaN!");
    HLAfloat64IntervalImpl::setValue(_impl, value);
  } else /* if (interval == 0) */ {
    // Since we may have swapped the arguments above we may need to store something
    HLAfloat64IntervalImpl::setValue(_impl, value);
  }
  return *this;
}

bool
HLAfloat64Interval::operator>(const LogicalTimeInterval& logicalTimeInterval) const
{
  double left = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(left))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  double right = HLAfloat64IntervalImpl::getValue(toHLAfloat64Interval(logicalTimeInterval)._impl);
  if (isNaN(right))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  return left > right;
}

bool
HLAfloat64Interval::operator<(const LogicalTimeInterval& logicalTimeInterval) const
{
  double left = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(left))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  double right = HLAfloat64IntervalImpl::getValue(toHLAfloat64Interval(logicalTimeInterval)._impl);
  if (isNaN(right))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  return left < right;
}

bool
HLAfloat64Interval::operator==(const LogicalTimeInterval& logicalTimeInterval) const
{
  double left = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(left))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  double right = HLAfloat64IntervalImpl::getValue(toHLAfloat64Interval(logicalTimeInterval)._impl);
  if (isNaN(right))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  return left == right;
}

bool
HLAfloat64Interval::operator>=(const LogicalTimeInterval& logicalTimeInterval) const
{
  double left = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(left))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  double right = HLAfloat64IntervalImpl::getValue(toHLAfloat64Interval(logicalTimeInterval)._impl);
  if (isNaN(right))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  return left >= right;
}

bool
HLAfloat64Interval::operator<=(const LogicalTimeInterval& logicalTimeInterval) const
{
  double left = HLAfloat64IntervalImpl::getValue(_impl);
  if (isNaN(left))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  double right = HLAfloat64IntervalImpl::getValue(toHLAfloat64Interval(logicalTimeInterval)._impl);
  if (isNaN(right))
    throw InvalidLogicalTimeInterval(L"Can not compare with NaN!");
  return left <= right;
}

VariableLengthData
HLAfloat64Interval::encode() const
{
  OpenRTI::VariableLengthData variableLengthData(8);
  variableLengthData.setFloat64BE(HLAfloat64IntervalImpl::getValue(_impl), 0);
  return VariableLengthDataFriend::create(variableLengthData);
}

size_t
HLAfloat64Interval::encodedLength() const
{
  return 8;
}

size_t
HLAfloat64Interval::encode(void* buffer, size_t bufferSize) const
{
  if (bufferSize < 8)
    throw CouldNotEncode(L"Buffer size too short!");
  union {
    double d;
    uint64_t u;
  } u;
  u.d = HLAfloat64IntervalImpl::getValue(_impl);
  uint8_t* data = static_cast<uint8_t*>(buffer);
  data[0] = uint8_t(u.u >> 56);
  data[1] = uint8_t(u.u >> 48);
  data[2] = uint8_t(u.u >> 40);
  data[3] = uint8_t(u.u >> 32);
  data[4] = uint8_t(u.u >> 24);
  data[5] = uint8_t(u.u >> 16);
  data[6] = uint8_t(u.u >> 8);
  data[7] = uint8_t(u.u);
  return 8;
}

void
HLAfloat64Interval::decode(const VariableLengthData& variableLengthData)
{
  OpenRTI::VariableLengthData data = VariableLengthDataFriend::readPointer(variableLengthData);
  if (data.size() < 8)
    throw CouldNotDecode(L"Buffer size too short!");
  HLAfloat64IntervalImpl::setValue(_impl, data.getFloat64BE(0));
}

void
HLAfloat64Interval::decode(void* buffer, size_t bufferSize)
{
  if (bufferSize < 8)
    throw CouldNotDecode(L"Buffer size too short!");
  union {
    double d;
    uint64_t u;
  } u;
  const uint8_t* data = static_cast<const uint8_t*>(buffer);
  u.u = uint64_t(data[0]) << 56;
  u.u |= uint64_t(data[1]) << 48;
  u.u |= uint64_t(data[2]) << 40;
  u.u |= uint64_t(data[3]) << 32;
  u.u |= uint64_t(data[4]) << 24;
  u.u |= uint64_t(data[5]) << 16;
  u.u |= uint64_t(data[6]) << 8;
  u.u |= uint64_t(data[7]);
  HLAfloat64IntervalImpl::setValue(_impl, u.d);
}

std::wstring
HLAfloat64Interval::toString() const
{
  std::wstringstream stream;
  stream << getInterval();
  return stream.str();
}

std::wstring
HLAfloat64Interval::implementationName() const
{
  return L"HLAfloat64Time";
}

double
HLAfloat64Interval::getInterval() const
{
  return HLAfloat64IntervalImpl::getValue(_impl);
}

void
HLAfloat64Interval::setInterval(double value)
{
  HLAfloat64IntervalImpl::setValue(_impl, value);
}

HLAfloat64Interval&
HLAfloat64Interval::operator=(const HLAfloat64Interval& value)
{
  HLAfloat64IntervalImpl::assign(_impl, value._impl);
  return *this;
}

HLAfloat64Interval::operator double() const
{
  return getInterval();
}

}
