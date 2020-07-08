#pragma once


#include <list>
#include <map>
#include <set>
#include <algorithm>

//#include "RTI/RTIAmbassador.h"
#include "encoding/BasicDataElements.h"
#include "encoding/HLAhandle.h"
#include "encoding/HLAfixedRecord.h"
#include "encoding/Encoding.h"
#include <functional>

namespace OpenRTI {

class MomManager;
class MomFederation;
class MomFederate;

typedef HLAinteger32BE HLAcount;

class OPENRTI_LOCAL HLAobjectClassBasedCount : public HLAfixedRecord
{
  public:
    HLAobjectClassBasedCount(ObjectClassHandle classHandle, int32_t count)
      : mClassHandle(new HLAhandle(classHandle))
      , mCount(new HLAcount(count))
    {
      HLAfixedRecord::appendElementPointer(mClassHandle);
      HLAfixedRecord::appendElementPointer(mCount);
    }

    ObjectClassHandle getClassHandle() const
    {
      return mClassHandle->getObjectClassHandle();
    }

    int32_t getCount()
    {
      return mCount->get();
    }

  private:
    // IMPORTANT NOTE: ownership will be transferred to the base class
    HLAhandle* mClassHandle;
    HLAcount* mCount;
};

class OPENRTI_LOCAL HLAinteractionCount : public HLAfixedRecord
{
  public:
    HLAinteractionCount()
      : mClassHandle(new HLAhandle(InteractionClassHandle()))
      , mCount(new HLAcount(0))
    {
      HLAfixedRecord::appendElementPointer(mClassHandle);
      HLAfixedRecord::appendElementPointer(mCount);
    }

    HLAinteractionCount(InteractionClassHandle classHandle, int32_t count)
      : mClassHandle(new HLAhandle(classHandle))
      , mCount(new HLAcount(count))
    {
      HLAfixedRecord::appendElementPointer(mClassHandle);
      HLAfixedRecord::appendElementPointer(mCount);
    }
    HLAinteractionCount(const HLAinteractionCount& ref)
      : mClassHandle(new HLAhandle(InteractionClassHandle()))
      , mCount(new HLAcount(0))
    {
      mClassHandle->set(ref.mClassHandle->getInteractionClassHandle());
      mCount->set(ref.mCount->get());
      HLAfixedRecord::appendElementPointer(mClassHandle);
      HLAfixedRecord::appendElementPointer(mCount);
    }

    ObjectClassHandle getClassHandle() const
    {
      return mClassHandle->getObjectClassHandle();
    }

    int32_t getCount() const
    {
      return mCount->get();
    }

    void increment()
    {
      mCount->set(mCount->get() + 1);
    }
    void setCount(uint32_t count)
    {
      mCount->set(count);
    }
  private:
    void operator=(const HLAinteractionCount& ) = delete;

    //----------------------------------------------------------
    //                   INSTANCE VARIABLES
    //----------------------------------------------------------
    // IMPORTANT NOTE: ownership will be transferred to the base class
    HLAhandle* mClassHandle;
    HLAcount* mCount;
};

class OPENRTI_LOCAL HLAinteractionSubscription : public HLAfixedRecord
{
  public:
    HLAinteractionSubscription()
      : mClassHandle(new HLAhandle(InteractionClassHandle()))
      , mActive(new HLAboolean(false))
    {
      HLAfixedRecord::appendElementPointer(mClassHandle);
      HLAfixedRecord::appendElementPointer(mActive);
    }

    HLAinteractionSubscription(InteractionClassHandle classHandle, bool active)
      : mClassHandle(new HLAhandle(classHandle))
      , mActive(new HLAboolean(active))
    {
      HLAfixedRecord::appendElementPointer(mClassHandle);
      HLAfixedRecord::appendElementPointer(mActive);
    }
    HLAinteractionSubscription(const HLAinteractionSubscription& ref)
      : mClassHandle(new HLAhandle(InteractionClassHandle()))
      , mActive(new HLAboolean(false))
    {
      mClassHandle->set(ref.mClassHandle->getInteractionClassHandle());
      mActive->set(ref.mActive->get());
      HLAfixedRecord::appendElementPointer(mClassHandle);
      HLAfixedRecord::appendElementPointer(mActive);
    }
    ~HLAinteractionSubscription()
    {
      // do NOT delete members here 
    }
    ObjectClassHandle getClassHandle() const
    {
      return mClassHandle->getObjectClassHandle();
    }

    bool getActive() const
    {
      return mActive->get();
    }

    void setCount(bool active)
    {
      mActive->set(active);
    }
  private:
    void operator=(const HLAinteractionCount& ) = delete;

    //----------------------------------------------------------
    //                   INSTANCE VARIABLES
    //----------------------------------------------------------
    // IMPORTANT NOTE: ownership will be transferred to the base class
    HLAhandle* mClassHandle;
    HLAboolean* mActive;
};

// Counts events (updates or reflections), together with the object instances concerned,
// for a given object class.
// At any time, the total number of events and the number of concerned object instances 
// can be returned as HLAobjectClassBasedCount
struct OPENRTI_LOCAL OCMetricTracker
{
  ObjectClassHandle mClassHandle;
  int mTotal;
  std::set<ObjectInstanceHandle> mInstances;
  OCMetricTracker() {}

  OCMetricTracker(ObjectClassHandle classHandle)
  {
    mClassHandle = classHandle;
    mTotal = 0;
  }

  void increment(ObjectInstanceHandle instanceId)
  {
    ++mTotal;
    mInstances.insert(instanceId);
  }

  HLAobjectClassBasedCount getTotal() const
  {
    return HLAobjectClassBasedCount(mClassHandle, mTotal);
  }

  HLAobjectClassBasedCount getTotalUniqueInstances() const
  {
    return HLAobjectClassBasedCount(mClassHandle, static_cast<int32_t>(mInstances.size()));
  }
};

struct OPENRTI_LOCAL AbstractFederateMetrics
{
    virtual void reflectionReceived(ObjectClassHandle classId, ObjectInstanceHandle instanceId) = 0;
    virtual void sentUpdate(ObjectClassHandle classId, ObjectInstanceHandle instanceId) = 0;
    virtual void interactionReceived(InteractionClassHandle classId) = 0;
    virtual void interactionSent(InteractionClassHandle classId) = 0;
    virtual void objectRegistered(ObjectInstanceHandle instanceId) = 0;

    virtual void objectClassSubscribed(ObjectClassHandle classId, const AttributeHandleVector& attributes, bool active) = 0;
    virtual void objectClassUnsubscribed(ObjectClassHandle classId, const AttributeHandleVector& attributes) = 0;
    virtual void objectClassPublished(ObjectClassHandle classId, const AttributeHandleVector& attributes) = 0;
    virtual void objectClassUnpublished(ObjectClassHandle classId, const AttributeHandleVector& attributes) = 0;

    virtual void interactionSubscribed(InteractionClassHandle classId, bool active) = 0;
    virtual void interactionUnsubscribed(InteractionClassHandle classId) = 0;
    virtual void interactionPublished(InteractionClassHandle classId) = 0;
    virtual void interactionUnpublished(InteractionClassHandle classId) = 0;

    // probably throw these out
    virtual VariableLengthData getObjectInstancesReflected() const = 0;
    virtual VariableLengthData getObjectInstancesUpdated() const = 0;
    virtual VariableLengthData getObjectClassUpdatesSent() const = 0;
    virtual VariableLengthData getObjectClassReflectionsReceived() const = 0;

    virtual VariableLengthData getInteractionsReceived() const = 0;
    virtual VariableLengthData getInteractionsSent() const = 0;
    virtual VariableLengthData getInteractionSubscriptions() const = 0;
    virtual VariableLengthData getInteractionPublications() const = 0;

    virtual VariableLengthData getNumberOfPublishedClasses() const = 0;
    virtual VariableLengthData getNumberOfSubscribedClasses() const = 0;
};

}