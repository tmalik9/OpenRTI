
#include <algorithm>
#include <cstring>
#include <string>
#include <memory>

#include <iostream>

#include <iostream>

#include "momDataTypes.h"
#include "momEncodings.h"
#include "../../../src/rti1516ev/HandleFactory.h"
#include "RTI1516ETestLib.h"

#include "implementation/RTFederateObjects.h"
#include "implementation/RTFederateInteractions.h"

template<typename TInputIter>
static inline std::string make_hex_string(TInputIter first, TInputIter last, bool use_uppercase = true, bool insert_spaces = false)
{
  std::ostringstream ss;
  ss << std::hex << std::setfill('0');
  if (use_uppercase)
    ss << std::uppercase;
  while (first != last)
  {
    unsigned char currentByte = (*first++) & 0xff;
    ss << std::setw(2) << (static_cast<unsigned int>(currentByte) & 0x000000ff);
    if (insert_spaces && first != last)
      ss << " ";
  }
  return ss.str();
}

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, const std::vector<uint8_t>& value)
{
  if (value.size() == 0)
  {
    os << "<empty>";
  }
  else
  {
    os << "{sz=" << value.size() << " [" << make_hex_string(static_cast<const uint8_t*>(value.data()), static_cast<const uint8_t*>(value.data()) + value.size(), true, true) << "]}";
  }
  return os;
}

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, const rti1516ev::VariableLengthData& value)
{
  if (value.size() == 0)
  {
    os << "<empty>";
  }
  else
  {
    os << "{sz=" << value.size() << " [" << make_hex_string(static_cast<const uint8_t*>(value.data()), static_cast<const uint8_t*>(value.data()) + value.size(), true, true) << "]}";
  }
  return os;
}

constexpr bool gDebugPrint = true;

using namespace rti1516ev;
bool testHLAhandleListEncoding()
{
  std::vector<HLAhandle> handleList;
  for (int i = 0; i < 5; i++)
  {
    handleList.push_back(rti1516ev::HLAhandle(rti1516ev::createFederateHandle(i)));
  }
  OpenRTI::Mom::HLAhandleListEncoding handleListEncoding;
  handleListEncoding.set(handleList);
  for (int i = 0; i < 5; i++)
  {
    std::wcout << static_cast<const HLAhandle&>(handleListEncoding[i]).getFederateHandle() << std::endl;
  }
  rti1516ev::VariableLengthData encodedData = handleListEncoding.encode();
  std::cout << "encodedData=" << encodedData << std::endl;
  OpenRTI::Mom::HLAhandleListEncoding handleListDecoder;
  handleListDecoder.decode(encodedData);
  for (size_t i = 0; i < handleListDecoder.size(); i++)
  {
    std::wcout << static_cast<const HLAhandle&>(handleListDecoder[i]).getFederateHandle() << std::endl;
  }
  return true;
}

bool testHLAobjectClassBasedCountsEncoding()
{
  std::vector<OpenRTI::Mom::HLAobjectClassBasedCountEncoding> objectClassBasedCounts;
  for (int i = 0; i < 5; i++)
  {
    OpenRTI::Mom::HLAobjectClassBasedCountEncoding objectClassBasedCount;
    objectClassBasedCount.SetHLAobjectClass(rti1516ev::HLAhandle(rti1516ev::createObjectClassHandle(i)));
    objectClassBasedCount.SetHLAcount(i + 1);

    objectClassBasedCounts.push_back(objectClassBasedCount);
  }
  OpenRTI::Mom::HLAobjectClassBasedCountsEncoding objectClassBasedCountsEncoding;
  objectClassBasedCountsEncoding.set(objectClassBasedCounts);
  for (int i = 0; i < 5; i++)
  {
    const OpenRTI::Mom::HLAobjectClassBasedCountEncoding& objectClassBasedCount =
      static_cast<const OpenRTI::Mom::HLAobjectClassBasedCountEncoding&>(objectClassBasedCountsEncoding[i]);
    std::wcout 
      << L"class=" << objectClassBasedCount.GetHLAobjectClass().getObjectClassHandle()
      << L" count=" << objectClassBasedCount.GetHLAcount()
      << std::endl;
  }
  rti1516ev::VariableLengthData encodedData = objectClassBasedCountsEncoding.encode();
  std::cout << "encodedData=" << encodedData << std::endl;
  OpenRTI::Mom::HLAobjectClassBasedCountsEncoding objectClassBasedCountsDecoder;
  objectClassBasedCountsDecoder.decode(encodedData);
  for (size_t i = 0; i < objectClassBasedCountsDecoder.size(); i++)
  {
    const OpenRTI::Mom::HLAobjectClassBasedCountEncoding& objectClassBasedCount =
      static_cast<const OpenRTI::Mom::HLAobjectClassBasedCountEncoding&>(objectClassBasedCountsEncoding[i]);
    std::wcout 
      << L"class=" << objectClassBasedCount.GetHLAobjectClass().getObjectClassHandle()
      << L" count=" << objectClassBasedCount.GetHLAcount()
      << std::endl;
  }
  return true;
}

class OPENRTI_LOCAL RTFederateAmbassador : public OpenRTI::RTI1516ESimpleAmbassador
{
public:
  RTFederateAmbassador() 
    : OpenRTI::RTI1516ESimpleAmbassador(), objectClassRegistry(), interactionClassRegistry(&objectClassRegistry)
  {
  }
  virtual void objectInstanceNameReservationSucceeded(const std::wstring& name) override
  {
    objectClassRegistry.ObjectInstanceNameReservationSucceeded(name);
  }

  virtual void objectInstanceNameReservationFailed(const std::wstring& name) override
  {
    objectClassRegistry.ObjectInstanceNameReservationFailed(name);
  }
  void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
    const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
    const rti1516ev::VariableLengthData& tag,
    rti1516ev::OrderType, rti1516ev::TransportationType,
    rti1516ev::SupplementalReflectInfo) override
  {
    objectClassRegistry.ReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
  }
  NDistSimIB::NRTFederateEncoding::ObjectClassRegistry objectClassRegistry;
  NDistSimIB::NRTFederateEncoding::InteractionClassRegistry interactionClassRegistry;
};

class MyBusControllerCan : public NDistSimIB::NRTFederateEncoding::BusControllerCan
{
public:
  MyBusControllerCan(NDistSimIB::NRTFederateEncoding::BusControllerCanObjectClass* objectClass, const std::wstring& instanceName, rti1516ev::RTIambassador* ambassador)
    : NDistSimIB::NRTFederateEncoding::BusControllerCan(objectClass, instanceName, ambassador)
  {
  }
  void SetBaudRate(int32_t newValue) override {
    NDistSimIB::NRTFederateEncoding::BusControllerCan::SetBaudRate(newValue);
    std::cout << "MyBusControllerCan::SetBaudRate has been called " << std::endl;
    if (mDirty & NDistSimIB::NRTFederateEncoding::BusControllerCan::kBaudRateBit)
    {
      std::cout << "baudrate has been changed" << std::endl;
    }
  }
};

bool testRTFederate(int argc, char* argv[])
{
  OpenRTI::Options options(argc, argv);
  std::wstring fomModule = L"RTFederate.xml";
  std::wstring federationExecutionName = L"test";

  std::vector<std::wstring> args;
  while (options.next("F:O:")) {
    switch (options.getOptChar()) {
    case 'F':
      federationExecutionName = OpenRTI::localeToUcs(options.getArgument());
      break;
    case 'O':
      fomModule = OpenRTI::localeToUcs(options.getArgument());
      break;
    }
  }
  RTFederateAmbassador ambassador;
  ambassador.setUseDataUrlObjectModels(false);

  try
  {
    ambassador.connect(L"thread://");
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return false;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return false;
  }
  // create, must work
  try
  {
    ambassador.createFederationExecution(federationExecutionName, fomModule);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return false;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return false;
  }

  rti1516ev::FederateHandle federateHandle;

  // join must work
  try
  {
    federateHandle = ambassador.joinFederationExecution(L"federate", federationExecutionName);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return false;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return false;
  }
  ambassador.objectClassRegistry.Initialize(ambassador.getRtiAmbassador());
  ambassador.interactionClassRegistry.Initialize(ambassador.getRtiAmbassador());

  auto* canMessageInteractionClass = ambassador.interactionClassRegistry.GetCANMessageInteractionClass();
  canMessageInteractionClass->RegisterReceiveCallback([](bool IsRequest, const std::string& ChannelName, NDistSimIB::NRTFederateEncoding::BusType BusType, rti1516ev::HLAhandle RequestingFederate, rti1516ev::HLAhandle Sender, rti1516ev::HLAhandle Receiver, int32_t Id, const NDistSimIB::NRTFederateEncoding::CANFrame& Frame)
  {
  });
  canMessageInteractionClass->Publish();
  canMessageInteractionClass->Subscribe();
  NDistSimIB::NRTFederateEncoding::CANFrameEncoding canFrame;
  canMessageInteractionClass->send(false, "CAN1", NDistSimIB::NRTFederateEncoding::BusType::kBtCAN, ambassador.getFederateHandle(), HLAhandle(ObjectInstanceHandle()), HLAhandle(ObjectInstanceHandle()), 0x100, canFrame);

  auto* busControllerObjectClass = ambassador.objectClassRegistry.GetBusControllerCanObjectClass();
  static_cast<NDistSimIB::NRTFederateEncoding::BusControllerCanObjectClass*>(busControllerObjectClass)->SetObjectInstanceCreator([](NDistSimIB::NRTFederateEncoding::BusControllerCanObjectClass* objectClass, const std::wstring& instanceName, rti1516ev::RTIambassador* rtiAmbassador) -> NDistSimIB::NRTFederateEncoding::BusControllerCan* {
    return new MyBusControllerCan(objectClass, instanceName, rtiAmbassador);
  });
  busControllerObjectClass->Publish();
  busControllerObjectClass->Subscribe(true);
  NDistSimIB::NRTFederateEncoding::IBusControllerCan* busControllerCan = busControllerObjectClass->CreateObjectInstance(L"CAN1");
  while (!static_cast<NDistSimIB::NRTFederateEncoding::BusControllerCan*>(busControllerCan)->IsValid())
  {
    ambassador.getRtiAmbassador()->evokeCallback(0.5);
  }
  std::wcout << L"busControllerCan is valid" << std::endl;
  bool updateReceived = false;
  auto callbackToken = busControllerCan->RegisterUpdateCallback([&updateReceived](NDistSimIB::NRTFederateEncoding::IBusControllerCan* busControllerCan) {
    std::wcout << L"update received: " << busControllerCan->GetObjectInstanceName() << std::endl;
    if (busControllerCan->GetUpdatedAttributes() & NDistSimIB::NRTFederateEncoding::IBusControllerCan::kBaudRateBit)
      std::wcout << L"  BaudRate = " << busControllerCan->GetBaudRate() << std::endl;
    if (busControllerCan->GetUpdatedAttributes() & NDistSimIB::NRTFederateEncoding::IBusControllerCan::kOperationModeBit)
      std::wcout << L"  OperationMode = " << busControllerCan->GetOperationMode() << std::endl;
    updateReceived = true;
  });
  busControllerCan->SetBaudRate(250000);
  busControllerCan->SetOperationMode(NDistSimIB::NRTFederateEncoding::CanOperationMode::kCanOperationModeCanFD);
  busControllerCan->UpdateModifiedAttributeValues();
  while (!updateReceived)
  {
    ambassador.getRtiAmbassador()->evokeCallback(0.5);
  }



  busControllerCan->UnregisterUpdateCallback(callbackToken);
  try
  {
    ambassador.resignFederationExecution(rti1516ev::NO_ACTION);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return false;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return false;
  }

  // destroy, must work
  try
  {
    ambassador.destroyFederationExecution(federationExecutionName);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << e.what() << std::endl;
    return false;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return false;
  }

  try
  {
    ambassador.shutdown();
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << e.what() << std::endl;
    return false;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return false;
  }
  if (gDebugPrint) std::wcout << "all tests SUCCEEDED!" << std::endl;
  return true;
}

int main(int argc, char* argv[])
{
  if (!testHLAhandleListEncoding())
    return EXIT_FAILURE;
  if (!testHLAobjectClassBasedCountsEncoding())
    return EXIT_FAILURE;
  if (!testRTFederate(argc, argv))
    return EXIT_FAILURE;

}

