/* -*-c++-*- OpenRTI - Copyright (C) 2009-2012 Mathias Froehlich
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

#ifndef OpenRTI_SocketAddress_h
#define OpenRTI_SocketAddress_h

#include <list>
#include <string>

#include "Export.h"
#include "SharedPtr.h"

namespace OpenRTI {

class SocketAddress;
class SocketPacket;
class SocketServer;
class SocketServerTCP;
class SocketStream;
class SocketTCP;
class SocketUDP;
class VariableLengthData;

typedef std::list<SocketAddress> SocketAddressList;

class OPENRTI_API SocketAddress {
public:
  SocketAddress() noexcept;
  SocketAddress(const SocketAddress& socketAddress);
  // NOTE: the destructor must stay in implementation file, otherwise it won't compile
  ~SocketAddress() noexcept;

  SocketAddress& operator=(const SocketAddress& socketAddress);

  bool operator<(const SocketAddress& socketAddress) const
  { return cmp(socketAddress) < 0; }
  bool operator>(const SocketAddress& socketAddress) const
  { return socketAddress.operator<(*this); }
  bool operator<=(const SocketAddress& socketAddress) const
  { return !socketAddress.operator<(*this); }
  bool operator>=(const SocketAddress& socketAddress) const
  { return !operator<(socketAddress); }
  bool operator==(const SocketAddress& socketAddress) const
  { return cmp(socketAddress) == 0; }
  bool operator!=(const SocketAddress& socketAddress) const
  { return !operator==(socketAddress); }

  bool valid() const;
  bool isPipe() const noexcept;
  bool isInet4() const;
  bool isInet6() const;

  std::string getNumericName() const;

  static SocketAddressList
  resolve(const std::string& address, const std::string& service, bool passive);
  static SocketAddressList
  resolve(const std::string& address, bool passive);
  static std::wstring getHostName();
  VariableLengthData getNetworkAddressData() const;
  VariableLengthData getNetworkPortData() const;

  static SocketAddress
  fromInet4Network(const SocketAddress& socketAddress, const VariableLengthData& networkAddressData, const VariableLengthData& networkPortData);
  static SocketAddress
  fromInet6Network(const SocketAddress& socketAddress, const VariableLengthData& networkAddressData, const VariableLengthData& networkPortData);

private:
  struct PrivateData;

  SocketAddress(SharedPtr<PrivateData> privateData);

  int cmp(const SocketAddress& socketAddress) const;

  PrivateData* data();
  const PrivateData* constData() const;

  SharedPtr<PrivateData> _privateData;

  friend class SocketPacket;
  friend class SocketServer;
  friend class SocketServerTCP;
  friend class SocketStream;
  friend class SocketTCP;
};

}

#endif
