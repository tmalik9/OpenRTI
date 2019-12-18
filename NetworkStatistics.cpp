
#include <windows.h>
#include "NetworkStatistics.h"
#include "dprintf.h"
#include <sstream>
#include <limits.h>

namespace OpenRTI {

NetworkStatistics::NetworkStatistics() : _Collecting(false)
{
}


NetworkStatistics::~NetworkStatistics()
{
}

#ifdef ENABLE_NETWORKSTATISTICS
void NetworkStatistics::Reset()
{
  _BytesReceived = 0;
  _BytesReceivedTotal = 0;
  _BytesSent = 0;
  _BytesSentTotal = 0;
  _MessagesSent = 0;
  _MessagesSentTotal = 0;
  _MessagesReceived = 0;
  _MessagesReceivedTotal = 0;
  _MessagesSentByType.clear();
  _MessagesReceivedByType.clear();
}


void NetworkStatistics::BytesReceived(size_t bytes)
{
  if (_Collecting)
  {
    _BytesReceived += bytes;
    _BytesReceivedTotal += bytes;
  }
}

void NetworkStatistics::BytesSent(size_t bytes)
{
  if (_Collecting)
  {
    _BytesSent += bytes;
    _BytesSentTotal += bytes;
  }
}

void NetworkStatistics::MessageReceived(const char* typeName)
{
  if (_Collecting)
  {
    _MessagesReceived++;
    _MessagesReceivedTotal++;
    _MessagesReceivedByType[typeName]++;
  }
}

void NetworkStatistics::MessageSent(const char* typeName)
{
  if (_Collecting)
  {
    _MessagesSent++;
    _MessagesSentTotal++;
    _MessagesSentByType[typeName]++;
  }
}

void NetworkStatistics::DumpStatistics()
{
  std::ostringstream to;
  to << "OpenRTI Network Statistics:" << std::endl;
  to << "messages received: " << _MessagesReceivedTotal << "(" << _MessagesReceived <<"/s)" << std::endl;
  to << "messages sent:     " << _MessagesSentTotal << "(" << _MessagesSent <<"/s)" << std::endl;
  to << "bytes received:    " << _BytesReceivedTotal << "(" << _BytesReceived <<"/s)" << std::endl;
  to << "bytes sent:        " << _BytesSentTotal << "(" << _BytesSent <<"/s)" << std::endl;
  OutputDebugStringA(to.str().c_str());
  _MessagesReceived = 0;
  _MessagesSent = 0;
  _BytesSent = 0;
  _BytesReceived = 0;
}

void NetworkStatistics::DumpTotals()
{
  DumpStatistics();
  std::ostringstream to;
  to << "Messages sent, by type:" << std::endl;
  for (auto [name, count] : _MessagesSentByType)
  {
    to << "  " << name << ":     " << count << std::endl;
  }
  to << "Messages received, by type:" << std::endl;
  for (auto [name, count] : _MessagesReceivedByType)
  {
    to << "  " << name << ":     " << count << std::endl;
  }
  OutputDebugStringA(to.str().c_str());
}
#endif
}