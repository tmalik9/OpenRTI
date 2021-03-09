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

#ifndef OpenRTI_AbstractProtocolLayer_h
#define OpenRTI_AbstractProtocolLayer_h

#include "AbstractProtocolSocket.h"
#include "Referenced.h"

namespace OpenRTI {

class OPENRTI_API AbstractProtocolLayer : public Referenced {
public:
  AbstractProtocolLayer() noexcept;
  virtual ~AbstractProtocolLayer()  noexcept;

  // Is called from the parent protocol layer when there is data to read
  virtual void read(AbstractProtocolSocket&) = 0;
  // True if the protocol is ready to receive
  virtual bool getEnableRead() const = 0;
  // Is called from the parent protocol layer when there is space to write something
  virtual void write(AbstractProtocolSocket&) = 0;
  // True if there is something to send
  virtual bool getEnableWrite() const = 0;

  virtual size_t bytesQueued() const = 0;
  // Is called from the parent protocol layer when an unrecoverable error happens.
  // FIXME rethink
  virtual void error(const Exception& e) = 0;

private:
  AbstractProtocolLayer(const AbstractProtocolLayer&) = delete;
  AbstractProtocolLayer& operator=(const AbstractProtocolLayer&) = delete;
};

} // namespace OpenRTI

#endif
