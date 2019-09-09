#pragma once
#include <Windows.h>
#include <mmsystem.h>

#include "NetworkStatistics.h"

namespace OpenRTI
{

class NetworkStatisticsWin32 : public NetworkStatistics
{
  public:
    NetworkStatisticsWin32();
    ~NetworkStatisticsWin32();

#ifdef ENABLE_NETWORKSTATISTICS
    void Start() override;
    void Stop() override;
    static NetworkStatisticsWin32& GetInstance();
#endif
  
  private:

#ifdef ENABLE_NETWORKSTATISTICS
    static void WINAPI timerCallback(UINT /*uTimerID*/, UINT /*uMsg*/, DWORD_PTR /*dwUser*/, DWORD_PTR /*dw1*/, DWORD_PTR /*dw2*/);
    MMRESULT mTimer;
#endif
};

} // namespace OpenRTI
