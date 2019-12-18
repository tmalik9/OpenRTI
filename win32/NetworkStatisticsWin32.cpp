
#include "NetworkStatisticsWin32.h"

#pragma comment(lib, "winmm.lib")

namespace OpenRTI {


NetworkStatisticsWin32::NetworkStatisticsWin32()
#ifdef ENABLE_NETWORKSTATISTICS
  : mTimer(0)
#endif
{
}

NetworkStatisticsWin32::~NetworkStatisticsWin32()
{
}

#ifdef ENABLE_NETWORKSTATISTICS
void WINAPI NetworkStatisticsWin32::timerCallback(UINT /*uTimerID*/, UINT /*uMsg*/, DWORD_PTR /*dwUser*/, DWORD_PTR /*dw1*/, DWORD_PTR /*dw2*/)
{
  GetNetworkStatistics().DumpStatistics();
}

void NetworkStatisticsWin32::Start(bool startTimer)
{
  _Collecting = true;
  Reset();
  if (mTimer != 0)
  {
    ::timeKillEvent(mTimer);
    mTimer = 0;
  }
  if (startTimer)
  {
    mTimer = ::timeSetEvent(1000, 1, timerCallback, reinterpret_cast<DWORD_PTR>(this), TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
  }
}

void NetworkStatisticsWin32::Stop()
{
  _Collecting = false;
  if (mTimer != 0)
  {
    ::timeKillEvent(mTimer);
    mTimer = 0;
  }
  DumpTotals();
}

NetworkStatisticsWin32& NetworkStatisticsWin32::GetInstance()
{
  static NetworkStatisticsWin32 _statistics;
  return _statistics;
}

INetworkStatistics& GetNetworkStatistics()
{
  return NetworkStatisticsWin32::GetInstance();
}
#endif

}