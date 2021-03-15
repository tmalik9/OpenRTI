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

std::chrono::milliseconds timeoutPeriod = std::chrono::milliseconds(5000);
std::chrono::milliseconds triggerPeriod = std::chrono::milliseconds(1910);

class SimpleTimeoutTest
{
  public:
    static bool exec()
    {
      std::cout << "waiting for timeout " << timeoutPeriod << std::endl;
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
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
      std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
      std::cout << "time passed until timeout=" << std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count() * 1e-9 << std::endl;
      return (std::chrono::duration_cast<std::chrono::milliseconds>(now - start) >= timeoutPeriod);
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
      bool wait_until(Clock abstime)
      {
        ScopeLock scopeLock(_mutex);
        while (!_signaled)
          if (!_condition.wait_until(scopeLock, abstime))
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
      ConditionData data;
      ConditionTest testThread(data);
      {
        bool result;
        std::cout << "Condition.wait_until(" << timeoutPeriod << "), no notification" << std::endl;
        Clock start = Clock::now();
        result = data.wait_until(start + Clock::fromSeconds(5));
        Clock stop = Clock::now();
        std::cout << "Condition.wait_until returned after: " << (stop - start).getNSec() * 1e-9 << " with result " << result << std::endl;
      }
      {
        bool result;
        std::cout << "Condition.wait_until(" << timeoutPeriod << "), with notification after " << triggerPeriod << std::endl;
        testThread.start();
        Clock start = Clock::now();
        result = data.wait_until(start + Clock(timeoutPeriod));
        Clock stop = Clock::now();
        std::cout << "Condition.wait_until returned after: " << (stop - start).getNSec() * 1e-9 << " with result " << result << std::endl;
      }
      return true;
    }
  protected:
    virtual void run() override
    {
      std::this_thread::sleep_for(triggerPeriod);
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
  //if (!ConditionTest::exec())
  //{
  //  std::cerr << "ConditionTest failed!" << std::endl;
  //  return EXIT_FAILURE;
  //}
}

