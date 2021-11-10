#pragma once
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

#pragma once

#include <list>
#include <string>

#include "AbstractMessageSender.h"
#include "Exception.h"
#include "Handle.h"
#include "HandleAllocator.h"
#include "IntrusiveList.h"
#include "IntrusiveUnorderedMap.h"
#include "LogStream.h"
#include "Referenced.h"
#include "RegionSet.h"
#include "ServerOptions.h"
#include "StringUtils.h"
#include "Types.h"
#include "VariableLengthDataTuple.h"

#include <memory>

namespace OpenRTI {

class MomServer;
class AbstractServer;
struct AbstractFederateMetrics;

namespace ServerModel {

class AcquireRequest
{
  public:
    enum class Status { REQUEST_AVAILABLE, REQUEST, RELEASED };
    class AttributeRequest
    {
      public:
        AttributeRequest(FederateHandle federateHandle, Status status)
          : _federateHandle(federateHandle), _status(status) {}
        FederateHandle _federateHandle;
        Status _status;
    };

  public:
    AcquireRequest(ObjectInstanceHandle objectInstanceHandle,
                   AttributeHandleVector attributes,
                   FederateHandle federate,
                   Status initialStatus)
      : _objectInstanceHandle(objectInstanceHandle)
    {
      for (auto attribute : attributes)
      {
        _requests.insert(std::make_pair(attribute, AttributeRequest(federate, initialStatus)));
      }
    }

    std::set<AttributeHandle> getAttributesUnderAcquisitionRequest(const AttributeHandleVector& attributes,
        FederateHandle federateHandle)
    {
      std::set<AttributeHandle> underRequest;
      for (AttributeHandle requestedAttribute : attributes)
      {
        auto requestIter = _requests.find(requestedAttribute);
        if (requestIter != _requests.end() && requestIter->second._status == Status::REQUEST
            && requestIter->second._federateHandle == federateHandle)
        {
          underRequest.insert(requestedAttribute);
        }
      }
      return underRequest;
    }

    std::set<AttributeHandle> getAttributesUnderAcquisitionRequest(const AttributeHandleVector& attributes)
    {
      std::set<AttributeHandle> underRequest;
      for (AttributeHandle requestedAttribute : attributes)
      {
        auto requestIter = _requests.find(requestedAttribute);
        if (requestIter != _requests.end() && requestIter->second._status == Status::REQUEST)
        {
          underRequest.insert(requestedAttribute);
        }
      }
      return underRequest;
    }

    std::set<AttributeHandle> getAttributesUnderAcquisitionRequest(FederateHandle federateHandle)
    {
      std::set<AttributeHandle> underRequest;
      for (auto& request : _requests)
      {
        if (request.second._federateHandle == federateHandle)
        {
          underRequest.insert(request.first);
        }
      }
      return underRequest;
    }

    void updateRequest(AttributeHandleVector attributes, FederateHandle federateHandle, Status incomingStatus)
    {
      for (AttributeHandle attribute : attributes)
      {
        auto requestIter = _requests.find(attribute);
        if (requestIter == _requests.end())
        {
          _requests.insert(std::make_pair(attribute, AttributeRequest(federateHandle,incomingStatus)));
        }
        else
        {
          auto& attributeRequest = requestIter->second;
          // if the EXISTING status is RELEASED, only make a change if the incoming status
          // is a new request.
          // Fixes GH #166: A federate may release some attributes and then immediately
          //                request them back, which is fine, but we may not have yet
          //                received the acquisition notification from the other federate,
          //                so although these are just sitting here marked as released, it
          //                prevents us from marking them locally as requested by us. For
          //                this reason, if they are marked as released and the incoming
          //                status is a request, we mark it as that.
          if (attributeRequest._status == Status::RELEASED || incomingStatus == Status::RELEASED)
          {
            if (incomingStatus == Status::REQUEST)
            {
              attributeRequest._status = Status::REQUEST;
              attributeRequest._federateHandle = federateHandle;
            }

            // nothing more to do
            continue;
          }

          // if we get here, the only valid states for EXISTING status are
          // Status.REQUEST and Status.REQUEST_AVAILABLE

          // there is already a transfer request for the attribute, take action!
          if (attributeRequest._status == incomingStatus)
          {
            // they're the same status, either RA & RA or R & R, if the incoming
            // federate handle is lower, it trumps the existing request
            if (attributeRequest._federateHandle > federateHandle)
            {
              attributeRequest._federateHandle = federateHandle;
            }
          }
          else if (incomingStatus == Status::REQUEST)
          {
            // the two statuses are not the same, so if incoming is R, existing must be RA
            // in this case, overwrite existing, as RA is just a "best effort" style thing
            attributeRequest._status = Status::REQUEST;
            attributeRequest._federateHandle = federateHandle;
          }
          else
          {
            // existing must be R and incoming is RA, ignore it
          }
        }
      }
    }

    void cancelTransfer(AttributeHandleVector attributes)
    {
      for (auto attributeHandle : attributes)
        _requests.erase(attributeHandle);
    }

    std::set<AttributeHandle> completeTransfer(FederateHandle federateHandle, bool ifAvailable)
    {
      Status desiredStatus = ifAvailable ? Status::REQUEST_AVAILABLE : Status::RELEASED;

      AttributeHandleSet complete;
      for (auto& requestEntry : _requests)
      {
        AttributeHandle attributeHandle = requestEntry.first;
        AttributeRequest& attributeRequest = requestEntry.second;
        if (attributeRequest._status == desiredStatus && attributeRequest._federateHandle == federateHandle)
        {
          // remove it later, otherwise we'll cause a ConcurrentAccessException
          complete.insert(attributeHandle);
        }
      }

      for (AttributeHandle confirmed : complete)
        _requests.erase(confirmed);

      return complete;
    }

    std::map<AttributeHandle, FederateHandle> releaseAttributes(AttributeHandleVector attributes)
    {
      std::map<AttributeHandle, FederateHandle> released;
      for (AttributeHandle attributeHandle : attributes)
      {
        auto requestIter = _requests.find(attributeHandle);
        if (requestIter != _requests.end())
        {
          requestIter->second._status = Status::RELEASED;
          released.insert(std::make_pair(attributeHandle, requestIter->second._federateHandle));
        }
      }
      return released;
    }

    bool isAttributeUnderAcquisitionRequest(AttributeHandle attribute)
    {
      auto requestIter = _requests.find(attribute);
      if (requestIter == _requests.end())
        return false;

      switch (requestIter->second._status)
      {
        case Status::REQUEST:
        case Status::REQUEST_AVAILABLE:
          return true;
        default:
          return false;
      }
    }

    std::set<AttributeHandle> getAttributesReleasedToFederate(FederateHandle federateHandle)
    {
      AttributeHandleSet released;
      for (auto requestEntry : _requests)
      {
        const AttributeHandle& attributeHandle = requestEntry.first;
        AttributeRequest& request = requestEntry.second;
        if (request._status == Status::RELEASED && request._federateHandle == federateHandle)
          released.insert(attributeHandle);
      }
      return released;
    }

    bool isEmpty()
    {
      return _requests.empty();
    }

  private:
    ObjectInstanceHandle _objectInstanceHandle;
    std::map<AttributeHandle, AttributeRequest> _requests;
};

class DivestRequest
{
  public:
    DivestRequest(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector attributes, FederateHandle federateHandle)
      : _objectInstanceHandle(objectInstanceHandle)
    {
      for (AttributeHandle attributeHandle : attributes)
      {
        _requests.insert(std::make_pair(attributeHandle, federateHandle));
      }
    }

    void updateRequest(AttributeHandleVector attributes, FederateHandle federateHandle)
    {
      for (AttributeHandle attributeHandle : attributes)
      {
        _requests[attributeHandle] = federateHandle;
      }
    }

    AttributeHandleVector getAttributesUnderDivestRequest(const AttributeHandleVector& attributes, FederateHandle federateHandle)
    {
      AttributeHandleVector underRequest;
      for (AttributeHandle attribute : attributes)
      {
        auto requestIter = _requests.find(attribute);
        if (requestIter != _requests.end() && requestIter->second == federateHandle)
          underRequest.push_back(attribute);
      }
      return underRequest;
    }

    AttributeHandleSet completeDivest(AttributeHandleVector attributes)
    {
      AttributeHandleSet completed;
      for (AttributeHandle attribute : attributes)
      {
        auto requestIter = _requests.find(attribute);
        if (requestIter != _requests.end())
        {
          _requests.erase(attribute);
          completed.insert(attribute);
        }
      }
      return completed;
    }
    void cancelDivest(AttributeHandleVector attributes)
    {
      for (AttributeHandle attribute : attributes)
      {
        _requests.erase(attribute);
      }
    }

    bool isEmpty()
    {
      return _requests.empty();
    }

  private:
    ObjectInstanceHandle _objectInstanceHandle;
    std::map<AttributeHandle, FederateHandle> _requests;
};

class OwnershipManager
{
  public:
    OwnershipManager();
    ~OwnershipManager();

    void requestAcquisition(ObjectInstanceHandle objectHandle, AttributeHandleVector attributes, FederateHandle federateHandle);

    void requestAcquisitionIfAvailable(ObjectInstanceHandle objectHandle, AttributeHandleVector attributes, FederateHandle federateHandle);

    AttributeHandleSet completeAcquisition(ObjectInstanceHandle objectHandle, FederateHandle federateHandle);

    std::map< AttributeHandle, FederateHandle> releaseAttributes(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector attributes);

    AttributeHandleVector getAttributesOfferedForDivest(ObjectInstanceHandle objectHandle,
        const AttributeHandleVector& attributes,
        FederateHandle federateHandle);

    AttributeHandleSet completeDivest(ObjectInstanceHandle objectHandle, AttributeHandleVector attributes);

  private:
    std::map<ObjectInstanceHandle,AcquireRequest> _acquisitions;
    std::map<ObjectInstanceHandle,DivestRequest>  _divestitures;
};

} // namespace ServerModel
} // namespace OpenRTI
