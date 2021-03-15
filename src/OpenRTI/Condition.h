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

#ifndef OpenRTI_Condition_h
#define OpenRTI_Condition_h

#include "Export.h"
#include "ScopeLock.h"

#ifndef __CPlusPlusStd
#error "must include OpenRTIConfig.h!"
#endif

#if 201103L <= __CPlusPlusStd
# include <condition_variable>
# include "Clock.h"
# include "AbsTimeout.h"
# include "Mutex.h"
#endif

namespace OpenRTI {

class Clock;
class ScopeLock;

class OPENRTI_API Condition {
public:
#if 201103L <= __CPlusPlusStd
  Condition() noexcept
  { }
#else
  Condition() noexcept;
#endif
#if 201103L <= __CPlusPlusStd
  ~Condition() noexcept
  { }
#else
  ~Condition();
#endif

#if 201103L <= __CPlusPlusStd
  void notify_one() noexcept
  { _condition.notify_one(); }
#else
  void notify_one();
#endif
#if 201103L <= __CPlusPlusStd
  void notify_all()
  { _condition.notify_all(); }
#else
  void notify_all();
#endif

#if 201103L <= __CPlusPlusStd
  void wait(ScopeLock& scopeLock)
  { _condition.wait(scopeLock); }
#else
  void wait(ScopeLock& scopeLock);
#endif
#if 201103L <= __CPlusPlusStd
  bool wait_until(ScopeLock& scopeLock, const Clock& abstime)
  {
    // Try to prevent overflow
    std::chrono::nanoseconds nsec;
    if (abstime.getNSec() <= (uint64_t)std::chrono::nanoseconds::max().count())
      nsec = std::chrono::nanoseconds(abstime.getNSec());
    else
      nsec = std::chrono::nanoseconds::max();

    typedef std::chrono::steady_clock::duration duration_type;
    duration_type duration;
    if (nsec <= std::chrono::duration_cast<std::chrono::nanoseconds>(duration_type::max()))
      duration = std::chrono::duration_cast<duration_type>(nsec);
    else
      duration = duration_type::max();

    AbsTimeout timeout(abstime);
    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::time_point(duration);
    while (!timeout.isExpired())
    {
      std::cv_status status = _condition.wait_until(scopeLock, std::chrono::steady_clock::time_point(timeout.getTimeout()));
      if (status == std::cv_status::no_timeout) return true;
    }
    return false;
  }
#else
  bool wait_until(ScopeLock& scopeLock, const Clock& abstime);
#endif

private:
  Condition(const Condition&) = delete;
  Condition& operator=(const Condition&) = delete;

#if 201103L <= __CPlusPlusStd
  std::condition_variable _condition;
#else
  struct PrivateData;
  PrivateData* _privateData;
#endif
};

} // namespace OpenRTI

#endif
