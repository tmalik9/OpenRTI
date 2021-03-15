#pragma once

#include "Clock.h"
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <atomic>

#ifndef __CPlusPlusStd
  #error "must include OpenRTIConfig.h!"
#endif

namespace OpenRTI {

class OPENRTI_API AbsTimeout
{
  public:

    AbsTimeout(const Clock& abstime);

    ~AbsTimeout();
    void set(const Clock& abstime) { _absTime = abstime; }
    bool isExpired() const;
    bool isExpired(Clock& remaining) const;

    Clock getTimeout() const
    {
      return _absTime + Clock::fromNSec(_delay);
    }
  private:
    Clock getDelay() const;
    void watchDogFunc();

    Clock                           _start;
    Clock                           _absTime;
    std::thread                     _watchDogThread;
    std::atomic<Clock::Rep>         _delay;
    static Clock                    _watchDogPeriod;

    bool                            _done = false;
    mutable std::condition_variable _cvDone;
    mutable std::mutex              _doneMutex;
    mutable bool                    _delayProvided = false;
    mutable std::condition_variable _cvDelay;
    mutable std::mutex              _delayMutex;
};

}  // namespace OpenRTI
