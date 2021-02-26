
#include "SocketBufferLimit.h"
#include "dprintf.h"

namespace OpenRTI 
{

  SocketBufferLimit& SocketBufferLimit::GetInstance()
  {
    static SocketBufferLimit _instance;
    return _instance;
  }

  SocketBufferLimit::SocketBufferLimit() noexcept : _bufferLimitEnabled(false), _bufferBalance(0), _bufferLimit(256000000)
  {
  }

  SocketBufferLimit::~SocketBufferLimit() noexcept
  {
  }

  void SocketBufferLimit::setBufferLimit(ssize_t bytes)
  {
    _bufferLimitEnabled = true;
    _bufferLimit = bytes;
  }

  ssize_t SocketBufferLimit::getBufferBalance()
  {
    return _bufferBalance;
  }

  void SocketBufferLimit::adjustBufferBalance(ssize_t dbytes)
  {
    _bufferBalance += dbytes;
  }

  bool SocketBufferLimit::isBufferBalanceCritical() 
  {
    return _bufferLimitEnabled && _bufferBalance > _bufferLimit;
  }

  void SocketBufferLimit::resetBufferBalance() 
  { 
    _bufferBalance = 0; 
  }




}
