#include "OwnershipManager.h"

/* -*-c++-*- OpenRTI - Copyright (C) 2009-2014 Mathias Froehlich
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

#include "DebugNew.h"
#include "OwnershipManager.h"
#include "ServerModel.h"

#include "ServerOptions.h"
#include <sstream>
#include <iomanip>
#include "MessageQueue.h"
#include "ServerNode.h"
#include "mom/MomServer.h"
#include "mom/MomFederateMetrics.h"
#include "mom/AbstractFederateMetrics.h"
#include "MessageEncodingRegistry.h"

namespace OpenRTI {
namespace ServerModel {


OwnershipManager::OwnershipManager()
{

}

OwnershipManager::~OwnershipManager()
{

}

void OwnershipManager::requestAcquisition(ObjectInstanceHandle objectHandle, AttributeHandleVector attributes, FederateHandle federateHandle)
{
  // find the existing transfer request
  auto requestiter = _acquisitions.find(objectHandle);
  if (requestiter == _acquisitions.end())
  {
    _acquisitions.insert(
      std::make_pair(
      objectHandle,
      AcquireRequest(objectHandle, attributes, federateHandle, AcquireRequest::Status::REQUEST)));
  }
  else
  {
    requestiter->second.updateRequest(attributes, federateHandle, AcquireRequest::Status::REQUEST);
  }
}

void OwnershipManager::requestAcquisitionIfAvailable(ObjectInstanceHandle objectHandle, AttributeHandleVector attributes, FederateHandle federateHandle)
{
  // find the existing transfer request
  auto requestiter = _acquisitions.find(objectHandle);
  if (requestiter == _acquisitions.end())
  {
    _acquisitions.insert(
      std::make_pair(
      objectHandle,
      AcquireRequest(objectHandle, attributes, federateHandle, AcquireRequest::Status::REQUEST_AVAILABLE)));
  }
  else
  {
    requestiter->second.updateRequest(attributes, federateHandle, AcquireRequest::Status::REQUEST_AVAILABLE);
  }
}

OpenRTI::AttributeHandleSet OwnershipManager::completeAcquisition(ObjectInstanceHandle objectHandle, FederateHandle federateHandle)
{
  auto requestiter = _acquisitions.find(objectHandle);
  if (requestiter == _acquisitions.end())
    return AttributeHandleSet();
  auto& request = requestiter->second;
  AttributeHandleSet obtained = request.completeTransfer(federateHandle, false);
  if (request.isEmpty())
    _acquisitions.erase(objectHandle);

  return obtained;
}

std::map<AttributeHandle,FederateHandle> OwnershipManager::releaseAttributes(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector attributes)
{
  auto requestIter = _acquisitions.find(objectInstanceHandle);
  if (requestIter != _acquisitions.end())
  {
    return requestIter->second.releaseAttributes(attributes);
  }
  else
  {
    return std::map< AttributeHandle, FederateHandle>();
  }
}

AttributeHandleVector OwnershipManager::getAttributesOfferedForDivest(ObjectInstanceHandle objectHandle, const AttributeHandleVector& attributes, FederateHandle federateHandle)
{
  auto requestEntry = _divestitures.find(objectHandle);
  if (requestEntry == _divestitures.end())
    return AttributeHandleVector();
  else
    return requestEntry->second.getAttributesUnderDivestRequest(attributes, federateHandle);
}

OpenRTI::AttributeHandleSet OwnershipManager::completeDivest(ObjectInstanceHandle objectHandle, AttributeHandleVector attributes)
{
  auto requestEntry = _divestitures.find(objectHandle);
  if (requestEntry == _divestitures.end())
    return AttributeHandleSet();

  AttributeHandleSet completed = requestEntry->second.completeDivest(attributes);
  if (requestEntry->second.isEmpty())
    _divestitures.erase(objectHandle);

  return completed;
}

} // namespace ServerModel
} // namespace OpenRTI
