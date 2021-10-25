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

#ifndef OpenRTI_ServerNode_h
#define OpenRTI_ServerNode_h

#include <string>
#include "AbstractMessageSender.h"
#include "AbstractServerNode.h"
#include "Referenced.h"
#include "SharedPtr.h"
#include "StringUtils.h"

namespace OpenRTI {

class ConnectHandle;
class ServerMessageDispatcher;
class ServerOptions;
namespace ServerModel {
class NodeConnect;
}
class OPENRTI_LOCAL ServerNode : public AbstractServerNode {
public:
  ServerNode();
  virtual ~ServerNode();

  const std::string& getServerName() const;
  void setServerName(const std::string& name);

  /// Returns true if the server is idle.
  /// Whare idle means that it is save to shut down the server completely.
  /// This is false for any root server or, for a child server that has child connects.
  virtual bool isIdle() const override;

  /// Hmm, make that more callback based during connection setup, but for now ...
  virtual ServerOptions& getServerOptions() override;
  virtual const ServerOptions& getServerOptions() const override;

  virtual ConnectHandle _insertConnect(const SharedPtr<AbstractMessageSender>& messageSender, const StringStringListMap& clientOptions) override;
  virtual ConnectHandle _insertParentConnect(const SharedPtr<AbstractMessageSender>& messageSender, const StringStringListMap& parentOptions) override;
  virtual void _eraseConnect(const ConnectHandle& connectHandle) override;
  virtual void _dispatchMessage(const AbstractMessage* message, const ConnectHandle& connectHandle) override;

  void setServer(AbstractServer* server) override { _server = server; }
  AbstractServer* getServer() const override { return _server; }

  ServerModel::NodeConnect* getParentConnect() const;
private:
  ServerNode(const ServerNode&) = delete;
  ServerNode& operator=(const ServerNode&) = delete;

  ServerMessageDispatcher* _serverMessageDispatcher;
  AbstractServer* _server;
};

} // namespace OpenRTI

#endif
