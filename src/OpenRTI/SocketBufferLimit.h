#pragma once

#include "Types.h"


namespace OpenRTI
{

class SocketBufferLimit 
{
  public:
    SocketBufferLimit() noexcept;
    ~SocketBufferLimit() noexcept;

    static SocketBufferLimit& GetInstance();

    void setBufferLimit(ssize_t bytes);
    ssize_t getBufferBalance();
    void adjustBufferBalance(ssize_t dbytes);
    bool isBufferBalanceCritical();
    void resetBufferBalance();

  private:
    bool _bufferLimitEnabled;
    ssize_t _bufferBalance;
    ssize_t _bufferLimit;
};

} // namespace OpenRTI
