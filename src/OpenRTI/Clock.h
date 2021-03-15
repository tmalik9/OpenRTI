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

#ifndef OpenRTI_Clock_h
#define OpenRTI_Clock_h

#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
#include "OpenRTIConfig.h"
#include "Export.h"
#include "Types.h"


#ifndef __CPlusPlusStd
#error "must include OpenRTIConfig.h!"
#endif

#if 201103L <= __CPlusPlusStd
# include <chrono>
# include <thread>
#endif

namespace OpenRTI {

// use something sophisticated for the posix clock stuff.
// win32 timeouts are just relative milliseconds which is easy to handle with any clock as long as it is consistent
class OPENRTI_API Clock {
public:
  using Rep = uint64_t;

  Clock() noexcept : _nsec(0) { }
  Clock(const Clock&) noexcept = default;
  Clock(Clock&&) noexcept = default;

  Clock(const std::chrono::seconds& s)
  {
    _nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(s).count();
  }
  Clock(const std::chrono::milliseconds& ms)
  {
    _nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(ms).count();
  }
  Clock(const std::chrono::nanoseconds& us)
  {
    _nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(us).count();
  }
  Clock(const std::chrono::steady_clock::time_point& tp)
  {
    _nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count();
  }

  ~Clock() noexcept = default;
  Clock& operator=(const Clock&) = default;
  Clock& operator=(Clock&&) = default;

  static Clock now()
  {
    return std::chrono::steady_clock::now();
  }

  operator std::chrono::steady_clock::time_point() const { return std::chrono::steady_clock::time_point(std::chrono::nanoseconds(_nsec)); }
  static void sleep_for(const Clock& reltime)
  {
    std::this_thread::sleep_for(std::chrono::nanoseconds(reltime.getNSec()));
  }

  static Clock zero()
  { return Clock(0); }
  static Clock max()
  { return Clock(std::numeric_limits<uint64_t>::max()); }

  /// Conversion from seconds, note that these conversions saturate
  static Clock fromSeconds(int seconds)
  {
    if (seconds <= 0)
      return zero();
    // Note that the unsigned cast has a guarantee to be the same
    // width than the int in the input argument. Those get upcasted
    // to the bigger unsigned then.
    if (std::numeric_limits<uint64_t>::max()/uint64_t(1000000000) <= uint64_t(seconds))
      return max();
    return Clock(seconds*uint64_t(1000000000));
  }
  static Clock fromSeconds(const double& seconds)
  {
    if (seconds <= 0.0)
      return zero();
    else {
      double nsec = 1e9*seconds;
      if (double(std::numeric_limits<uint64_t>::max()) <= nsec)
        return max();
      return Clock(uint64_t(nsec));
    }
  }
  static Clock fromMilliSeconds(uint32_t milliSeconds)
  {
    if (milliSeconds <= 0)
      return zero();
    // Note that the unsigned cast has a guarantee to be the same
    // width than the int in the input argument. Those get upcasted
    // to the bigger unsigned then.
    if (std::numeric_limits<uint64_t>::max()/1000000ULL <= uint64_t(milliSeconds))
      return max();
    return Clock(milliSeconds*1000000ULL);
  }
  uint64_t getMilliSeconds() const noexcept
  {
    return _nsec / 1000000ULL;
  }
  uint64_t getMicroSeconds() const noexcept
  {
    return _nsec / 1000ULL;
  }
  /// Conversion from nanoseconds
  static Clock fromNSec(uint64_t nsec)
  { return Clock(nsec); }

  uint64_t getNSec() const noexcept
  { return _nsec; }
  void setNSec(uint64_t nsec) noexcept
  { _nsec = nsec; }

  // Arithmetic, note that these two do not wrap.
  Clock& operator+=(const Clock& clock) noexcept
  {
    if (~_nsec <= clock._nsec)
      _nsec = std::numeric_limits<uint64_t>::max();
    else
      _nsec += clock._nsec;
    return *this;
  }
  Clock& operator-=(const Clock& clock) noexcept
  {
    if (_nsec <= clock._nsec)
      _nsec = 0;
    else
      _nsec -= clock._nsec;
    return *this;
  }

  // Compares
  bool operator==(const Clock& clock) const noexcept
  { return _nsec == clock._nsec; }
  bool operator!=(const Clock& clock) const noexcept
  { return _nsec != clock._nsec; }
  bool operator<(const Clock& clock) const noexcept
  { return _nsec < clock._nsec; }
  bool operator<=(const Clock& clock) const noexcept
  { return _nsec <= clock._nsec; }
  bool operator>(const Clock& clock) const noexcept
  { return _nsec > clock._nsec; }
  bool operator>=(const Clock& clock) const noexcept
  { return _nsec >= clock._nsec; }

private:
  explicit Clock(Rep nsecs)
    : _nsec(nsecs)
  { }

  // In the worst case this is nanoseconds since epoch.
  // In the usual case this is the simulation time in nanoseconds for the logical time
  // and the monotonic posix clock for timeouts.
  Rep _nsec;
};

inline
Clock operator+(const Clock& clock1, const Clock& clock2) noexcept
{ return Clock(clock1) += clock2; }
inline
Clock operator-(const Clock& clock1, const Clock& clock2) noexcept
{ return Clock(clock1) -= clock2; }
// inline
// Clock operator*(const Clock& clock1, const Clock& clock2)
// { return Clock(clock1) *= clock2; }
// inline
// Clock operator/(const Clock& clock1, const Clock& clock2)
// { return Clock(clock1) /= clock2; }

template<typename char_type, typename traits_type>
inline
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, const Clock& c)
{
  std::basic_stringstream<char_type, traits_type> stream;

  stream << (c.getNSec() / 1000000000) << stream.widen('.');
  stream << std::setw(9) << std::setfill(stream.widen('0')) << (c.getNSec() % 1000000000);

  return os << stream.str();
}

} // namespace OpenRTI

#endif
