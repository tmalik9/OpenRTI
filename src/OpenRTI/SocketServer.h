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

#ifndef OpenRTI_SocketServer_h
#define OpenRTI_SocketServer_h

#include "Export.h"
#include "Socket.h"
#include <SharedPtr.h>

namespace OpenRTI {

class SocketAddress;
class SocketStream;

class OPENRTI_API SocketServer : public Socket {
public:
  virtual SharedPtr<SocketStream> accept() = 0;

  SocketAddress getsockname() const;

protected:
  SocketServer(PrivateData* privateData);
  virtual ~SocketServer() noexcept;
};

} // namespace OpenRTI

#endif
