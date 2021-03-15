

#include "DebugNew.h"
#include "AbsTimeout.h"
#include <iostream>

namespace OpenRTI {

Clock AbsTimeout::_watchDogPeriod = Clock::fromMilliSeconds(100);

AbsTimeout::AbsTimeout(const Clock& abstime)
{
  _absTime = abstime;
  _delay = 0;
  _start = Clock::now();
  _done = false;
  _watchDogThread = std::thread(&AbsTimeout::watchDogFunc, this);
}


AbsTimeout::~AbsTimeout()
{
  {
    std::unique_lock<std::mutex> lock(_doneMutex);
    _done = true;
    _cvDone.notify_one();
  }
  _watchDogThread.join();
}

// Return true iff the specified absolute time, plus additional delays introduced by spending time
// in debug break, expired.
bool AbsTimeout::isExpired() const
{
  Clock delay = getDelay();
  Clock now = Clock::now();
  return now > _absTime + delay;
  //return getRemaining() == Clock::zero();
}

bool AbsTimeout::isExpired(Clock& remaining) const
{
  Clock delay = getDelay();
  Clock now = Clock::now();
  if (now < _absTime + delay)
  {
    remaining = (_absTime + delay) - now;
    return false;
  }
  else
  {
    remaining = Clock::zero();
    return true;
  }
}

// The awful double lock is required to circumvent a race condition between cv.wait_for
// and accessing the _delay provided by watchDogFunc.
// In essence, the following could happen:
// 1. watchDogFunc sits in _cvDone.wait_for, while the program is in break mode
// 2. upon return from debug break, _cvDone.wait_for returns (with timeout)
// 3. isExpired or getRemaining gets called, before watchDogFunc happened to update the break delay.
// The actual time spent in _cvDone.wait_for during break won't be taken into account.
OpenRTI::Clock AbsTimeout::getDelay() const
{
 // will wait until _cvDone.wait_for has been entered
  _doneMutex.lock();
  _cvDone.notify_one();
  
  // get the 2nd lock before watchDogFunc can acquire it
  std::unique_lock<std::mutex> delayLock(_delayMutex);
  // now release _cvDone.wait_for (in watchDogFunc)
  _doneMutex.unlock();
  // wait for watchDogFunc to update it's delay
  _cvDelay.wait(delayLock, [this] { return _delayProvided; });
  Clock delay = Clock::fromNSec(_delay);
  _delayProvided = false;

  return delay;
}

void AbsTimeout::watchDogFunc()
{
  while (true)
  {
    std::unique_lock<std::mutex> lock(_doneMutex);
    if (_done) break;
    Clock start = Clock::now();
    _cvDone.wait_for(lock, std::chrono::steady_clock::duration(_watchDogPeriod.getNSec()));

    std::unique_lock<std::mutex> delayLock(_delayMutex);
    Clock now = Clock::now();
    if ((now - start) > _watchDogPeriod)
    {
      _delay += ((now - start) - _watchDogPeriod).getNSec();
    }
    _delayProvided = true;
    _cvDelay.notify_one();
  }
}

}  // namespace OpenRTI
