/* -*-c++-*- OpenRTI - Copyright (C) 2009-2016 Mathias Froehlich
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

#ifndef OpenRTI_ServerConfigContentHandler_h
#define OpenRTI_ServerConfigContentHandler_h

#include <string>
#include <vector>

#include "Attributes.h"
#include "ContentHandler.h"
#include "Exception.h"
#include "LogStream.h"

namespace OpenRTI {

class OPENRTI_LOCAL ServerConfigContentHandler : public XML::ContentHandler {
public:
  ServerConfigContentHandler() noexcept;
  virtual ~ServerConfigContentHandler() noexcept;

  void startDocument() override;
  void endDocument() override;
  void startElement(const char* uri, const char* name,
                            const char* qName, const XML::Attributes* atts) override;
  void endElement(const char* uri, const char* name, const char* qName) override;

  /// The parent server url from the config file
  const std::string& getParentServerUrl() const noexcept
  { return _parentServerUrl; }

  /// The server global default for clients connects using zlib conpression
  bool getEnableZLibCompression() const noexcept
  { return _enableZLibCompression; }

  /// Each listen tag in the config file is represented with such a struct
  struct ListenConfig {
    const std::string& getUrl() const noexcept
    { return _url; }

  private:
    friend class ServerConfigContentHandler;

    std::string _url;
  };

  unsigned getNumListenConfig() const noexcept
  { return unsigned(_listenConfig.size()); }

  const ListenConfig& getListenConfig(unsigned index) const
  {
    OpenRTIAssert(index < _listenConfig.size());
    return _listenConfig[index];
  }

  LogStream::Priority getLogPriority() const { return _LogPriority; }
  bool isLogPrioritySet() const { return _LogPrioritySet; }
  LogStream::Category getLogCategory() const { return static_cast<LogStream::Category>(_LogCategory); }
  bool isLogCategorySet() const { return _LogCategorySet; }
  std::string  getLogFile()     const { return _LogFile; }
  bool isLogFileSet() const { return _LogFileSet; }
  bool getLogToConsole() const { return _LogToConsole; }
  bool isLogToConsoleSet() const { return _LogToConsoleSet; }
  bool getEnableNetworkStatistics() const { return _enableNetworkStatistics; }
  bool isBufferLimitSet() const { return _BufferLimitSet; }
  size_t getBufferLimit() const { return _BufferLimit; }

private:
  void parseLogCategory(const std::string& s);
  // poor man's schema checking ...
  enum Mode {
    UnknownMode,

    OpenRTIServerConfigMode,

    ParentServerMode,
    EnableZLibCompressionMode,
    ListenMode,
    LogPriorityMode,
    LogCategoryMode,
    LogFileMode,
    LogToConsoleMode,
    LogToDebugMode,
    NetworkStatisticsMode,
    BufferLimitMode
  };

  Mode getCurrentMode()
  {
    if (_modeStack.empty())
      return UnknownMode;
    return _modeStack.back();
  }

  // Current modes in a stack
  std::vector<Mode> _modeStack;

  /// The parent server url from the config file
  std::string _parentServerUrl;

  /// Server defaults
  bool _enableZLibCompression = true;
  bool _enableNetworkStatistics = false;
  /// The config file configured listens
  std::vector<ListenConfig> _listenConfig;

  unsigned                  _LogCategory;
  bool                      _LogCategorySet = false;
  LogStream::Priority       _LogPriority;
  bool                      _LogPrioritySet = false;
  std::string               _LogFile;
  bool                      _LogFileSet = false;
  bool                      _LogToConsole = false;
  bool                      _LogToConsoleSet = false;

  // Blocking federates get kicked out if the send/receive balance is above this value (bytes)
  bool                      _BufferLimitSet = false;
  size_t                    _BufferLimit = 256000000;
};

}

#endif
