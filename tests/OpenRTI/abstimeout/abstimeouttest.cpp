// abstimeout.cpp : Defines the entry point for the console application.
//

#include "AbsTimeout.h"
#include "Condition.h"
#include "Mutex.h"
#include "ScopeLock.h"
#include "ScopeUnlock.h"
#include "Thread.h"
#include <iostream>

using namespace OpenRTI;

Clock timeoutPeriod = Clock::fromMilliSeconds(5000);
std::chrono::milliseconds triggerPeriod = std::chrono::milliseconds(1910);
Clock kInfinite = Clock::max();
Clock kZero = Clock::zero();

class SimpleTimeoutTest
{
  public:
    static bool exec()
    {
      std::cout << "waiting for timeout " << timeoutPeriod << std::endl;
      Clock start = Clock::now();
      {
        Clock absTime = Clock::now() + Clock(timeoutPeriod);
        AbsTimeout timeout(absTime);
        Clock remaining;
        while (!timeout.isExpired(remaining))
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          std::cout << "remaining: " << remaining << std::endl;
        }
      }
      Clock now = Clock::now();
      std::cout << "time passed until timeout=" << (now - start).getNanoSeconds() * 1e-9 << std::endl;
      return ((now - start) >= timeoutPeriod);
    }
};

class InfiniteTimeoutTest
{
  public:
    static bool exec()
    {
      std::cout << "waiting for timeout " << kInfinite << std::endl;
      Clock start = Clock::now();
      {
        Clock absTime = Clock::now() + Clock(timeoutPeriod);
        AbsTimeout timeout(kInfinite);
        Clock remaining;
        while (!timeout.isExpired(remaining) && Clock::now() < absTime)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          std::cout << "remaining: " << remaining << std::endl;
        }
      }
      Clock now = Clock::now();
      std::cout << "time passed until timeout=" << (now - start).getNanoSeconds() * 1e-9 << std::endl;
      return ((now - start) >= timeoutPeriod);
    }
};

class ZeroTimeoutTest
{
  public:
    static bool exec()
    {
      std::cout << "waiting for timeout " << kZero << std::endl;
      Clock start = Clock::now();
      {
        Clock absTime = start + Clock(timeoutPeriod);
        AbsTimeout timeout(kZero);
        Clock remaining;
        while (!timeout.isExpired(remaining) && Clock::now() < absTime)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          std::cout << "remaining: " << remaining << std::endl;
        }
      }
      Clock now = Clock::now();
      std::cout << "time passed until timeout=" << (now - start).getNanoSeconds() * 1e-9 << std::endl;
      return ((now - start) <= Clock::fromMilliSeconds(100));
    }
};

class ConditionTest : Thread
{
  private:
    struct ConditionData
    {
      ConditionData() : _signaled(false) {}
      void notify_one()
      {
        ScopeLock scopeLock(_mutex);
        _signaled = true;
        _condition.notify_one();
      }
      void wait()
      {
        ScopeLock scopeLock(_mutex);
        while (!_signaled)
          _condition.wait(scopeLock);
        _signaled = false;
      }
      bool wait_until(const Clock& abstime)
      {
        ScopeLock scopeLock(_mutex);
        while (!_signaled)
        {
          if (!_condition.wait_until(scopeLock, abstime))
          {
            return false;
          }
        }
        _signaled = false;
        return true;
      }
      bool wait_until(const AbsTimeout& timeout)
      {
        ScopeLock scopeLock(_mutex);
        while (!_signaled)
          if (!_condition.wait_until(scopeLock, timeout))
            return false;
        _signaled = false;
        return true;
      }

      Condition _condition;
      Mutex _mutex;
      bool _signaled;
    };

  public:
    ConditionTest(ConditionData& data)
      : _data(data)
    {
    }

    static bool exec()
    {
      {
        ConditionData data;
        bool result;
        std::cout << "Condition.wait_until(" << timeoutPeriod << "), no notification" << std::endl;
        Clock start = Clock::now();
        result = data.wait_until(start + Clock::fromSeconds(5));
        Clock stop = Clock::now();
        std::cout << "Condition.wait_until returned after: " << (stop - start).getNanoSeconds() * 1e-9 << " with result " << result << std::endl;
      }
      {
        ConditionData data;
        bool result;
        std::cout << "Condition.wait_until(AbsTimeout(" << timeoutPeriod << ")), no notification" << std::endl;
        Clock start = Clock::now();
        AbsTimeout timeout(start + Clock::fromSeconds(5));
        result = data.wait_until(timeout);
        Clock stop = Clock::now();
        std::cout << "Condition.wait_until returned after: " << (stop - start).getNanoSeconds() * 1e-9 << " with result " << result << std::endl;
      }
      {
        ConditionData data;
        bool result;
        std::cout << "Condition.wait_until(" << 0 << "), no notification" << std::endl;
        Clock start = Clock::now();
        result = data.wait_until(start);
        Clock stop = Clock::now();
        std::cout << "Condition.wait_until returned after: " << (stop - start).getNanoSeconds() * 1e-9 << " with result " << result << std::endl;
      }
      uint32_t rounds = 10000;
      std::cout << "Busy Loop Test(" << std::dec << rounds << " rounds): Condition.wait_until(" << 0 << "), no notification" << std::endl;
      Clock loopStart = Clock::now();
      for (uint32_t i=0;i<rounds;i++)
      {
        Clock now = Clock::now();
        ConditionData data;
        data.wait_until(now);
      }
      Clock loopStop = Clock::now();
      std::cout << "Busy loop test took " << (loopStop - loopStart).getNanoSeconds() * 1e-9 << " seconds" << std::endl;
      {
        ConditionData data;
        ConditionTest testThread(data);
        bool result;
        std::cout << "Condition.wait_until(" << timeoutPeriod << "), with notification after " << std::dec << triggerPeriod.count() << "ms" << std::endl;
        testThread.start();
        Clock start = Clock::now();
        result = data.wait_until(start + Clock(timeoutPeriod));
        Clock stop = Clock::now();
        std::cout << "Condition.wait_until returned after: " << (stop - start).getNanoSeconds() * 1e-9 << " with result " << result << std::endl;
        testThread.wait();
      }
      {
        ConditionData data;
        ConditionTest testThread(data);
        bool result;
        std::cout << "Condition.wait_until(" << kInfinite << "), with notification after " << std::dec << triggerPeriod.count() << "ms" << std::endl;
        testThread.start();
        Clock start = Clock::now();
        result = data.wait_until(kInfinite);
        Clock stop = Clock::now();
        std::cout << "Condition.wait_until returned after: " << (stop - start).getNanoSeconds() * 1e-9 << " with result " << result << std::endl;
        testThread.wait();
      }
      return true;
    }
  protected:
    virtual void run() override
    {
      std::cout << "notification thread started" << std::endl;
      std::this_thread::sleep_for(triggerPeriod);
      std::cout << "notify cv" << std::endl;
      _data.notify_one();
    }
    ConditionData& _data;
};


int main()
{
  if (!SimpleTimeoutTest::exec())
  {
    std::cerr << "SimpleTimeoutTest failed!" << std::endl;
    return EXIT_FAILURE;
  }
  if (!InfiniteTimeoutTest::exec())
  {
    std::cerr << "InfiniteTimeoutTest failed!" << std::endl;
    return EXIT_FAILURE;
  }
  if (!ZeroTimeoutTest::exec())
  {
    std::cerr << "ZeroTimeoutTest failed!" << std::endl;
    return EXIT_FAILURE;
  }
  if (!ConditionTest::exec())
  {
    std::cerr << "ConditionTest failed!" << std::endl;
    return EXIT_FAILURE;
  }
}

