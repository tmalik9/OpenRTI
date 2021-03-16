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
  bool wait_until(ScopeLock& scopeLock, const AbsTimeout& timeout)
  {
    if (timeout.getTimeout() == Clock::max())
    {
      _condition.wait(scopeLock);
      return true;
    }
    else
    {
      while (!timeout.isExpired())
      {
        std::chrono::steady_clock::time_point tp(timeout.getTimeout());
        std::cv_status status = _condition.wait_until(scopeLock, tp);
        if (status == std::cv_status::no_timeout) return true;
      }
      return false;
    }
  }
  bool wait_until(ScopeLock& scopeLock, const Clock& abstime)
  {
    AbsTimeout timeout(abstime);
    return wait_until(scopeLock, timeout);
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
