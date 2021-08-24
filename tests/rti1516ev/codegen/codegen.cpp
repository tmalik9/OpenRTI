
#include <algorithm>
#include <cstring>
#include <string>
#include <memory>

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

int compare(const rti1516ev::VariableLengthData& lhs, const rti1516ev::VariableLengthData& rhs)
{
  if (lhs.size() < rhs.size()) return -1;
  else if (lhs.size() < rhs.size()) return 1;
  else 
  {
    return memcmp(lhs.data(), rhs.data(), lhs.size());
  }
}
bool operator!=(const rti1516ev::VariableLengthData& lhs, const rti1516ev::VariableLengthData& rhs)
{
  return compare(lhs, rhs) != 0;
}
bool operator==(const rti1516ev::VariableLengthData& lhs, const rti1516ev::VariableLengthData& rhs)
{
  return compare(lhs, rhs) == 0;
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
  virtual void receiveInteraction(rti1516ev::InteractionClassHandle interaction,
                                    const rti1516ev::ParameterHandleValueMap& parameters,
                                    const rti1516ev::VariableLengthData&,
                                    rti1516ev::OrderType,
                                    rti1516ev::TransportationType,
                                    rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
    interactionClassRegistry.ReceiveInteraction(interaction, parameters);
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

using NDistSimIB::NRTFederateEncoding::optional;
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
  bool received = false;

  // initialize class registries
  ambassador.objectClassRegistry.Initialize(ambassador.getRtiAmbassador());
  ambassador.interactionClassRegistry.Initialize(ambassador.getRtiAmbassador());

  // publish/subscribe & send MeasurementInit interaction
  auto* measurementInitInteractionClass = ambassador.interactionClassRegistry.GetMeasurementInitInteractionClass();
  measurementInitInteractionClass->RegisterReceiveCallback([&received](optional<int64_t> time, optional<NDistSimIB::NRTFederateEncoding::OrderType> orderType)
  {
    std::cout << "received MeasurementInit:" << std::endl;
    received = true;
  });
  measurementInitInteractionClass->Publish();
  measurementInitInteractionClass->Subscribe(true);
  measurementInitInteractionClass->send();
  while (!received)
    ambassador.getRtiAmbassador()->evokeCallback(0.5);

  received = false;

  auto* canMessageInteractionClass = ambassador.interactionClassRegistry.GetCANMessageInteractionClass();
  canMessageInteractionClass->RegisterReceiveCallback([&received](
    optional<bool> IsRequest,
    optional<std::string> ChannelName,
    optional<NDistSimIB::NRTFederateEncoding::BusType> BusType,
    optional<rti1516ev::HLAhandle> RequestingFederate,
    optional<NDistSimIB::NRTFederateEncoding::IBusController*> Sender,
    optional<NDistSimIB::NRTFederateEncoding::IBusController*> Receiver,
    optional<int32_t> Id,
    optional<const NDistSimIB::NRTFederateEncoding::CANFrame&> Frame,
    optional<int64_t> time, optional<NDistSimIB::NRTFederateEncoding::OrderType> order)
  {
    std::cout << "received CANFrame:" << std::endl;
    if (Id) std::cout << "Id = " << std::hex << *Id << std::dec << std::endl;
    if (ChannelName) std::cout << "ChannelName = " << *ChannelName << std::endl;
    if (Frame)
    {
      std::cout << "Dir = " << Frame->GetDir() << " (" << (int)Frame->GetDir() << ")" << std::endl;
      std::cout << "DataLength = " << (int)(Frame->GetDataLength()) << std::endl;
      std::cout << "Data = " << Frame->GetData() << std::endl;
    }
    if (time)
    {
      std::cout << "time = " << *time << " order=" << *order << std::endl;
    }
    if (Sender)
    {
      std::cout << "Sender = " << OpenRTI::to_string((*Sender)->GetObjectInstanceName()) << std::endl;
    }
    if (Receiver)
    {
      std::cout << "Receiver = " << OpenRTI::to_string((*Receiver)->GetObjectInstanceName()) << std::endl;
    }
    received = true;
  });
  canMessageInteractionClass->Publish();
  canMessageInteractionClass->Subscribe(true);

  // set up a customized CAN controller
  auto* busControllerCanObjectClass = ambassador.objectClassRegistry.GetBusControllerCanObjectClass();
  static_cast<NDistSimIB::NRTFederateEncoding::BusControllerCanObjectClass*>(busControllerCanObjectClass)->SetObjectInstanceCreator([](NDistSimIB::NRTFederateEncoding::BusControllerCanObjectClass* objectClass, const std::wstring& instanceName, rti1516ev::RTIambassador* rtiAmbassador) -> NDistSimIB::NRTFederateEncoding::BusControllerCan* {
    return new MyBusControllerCan(objectClass, instanceName, rtiAmbassador);
  });
  busControllerCanObjectClass->Publish();
  busControllerCanObjectClass->Subscribe(true);
  std::shared_ptr<NDistSimIB::NRTFederateEncoding::IBusControllerCan> busControllerCan = busControllerCanObjectClass->CreateObjectInstance(L"CAN1");
  while (!busControllerCan->IsValid())
  {
    ambassador.getRtiAmbassador()->evokeCallback(0.5);
  }
  std::wcout << L"busControllerCan is valid" << std::endl;
  auto* busControllerObjectClass = ambassador.objectClassRegistry.GetBusControllerObjectClass();
  if (busControllerObjectClass->GetObjectInstance(L"CAN1") == nullptr)
  {
    std::cerr << "could not get bus controller from base object class" << std::endl;
    return false;
  }
  if (!busControllerObjectClass->GetObjectInstance(L"CAN1")->IsValid())
  {
    std::cerr << "bus controller base object is not valid" << std::endl;
    return false;
  }
  std::wcerr << L"bus controller base object is valid" << std::endl;
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

  // send a CAN frame
  NDistSimIB::NRTFederateEncoding::CANFrameEncoding canFrame;
  std::vector<uint8_t> data{1, 2, 3, 4};
  canFrame.SetDataLength(4);
  canFrame.SetData(data);

  std::cout << "canFrame.DataLength=" << (int)canFrame.GetDataLength() << std::endl;
  std::cout << "canFrame.Data=" << canFrame.GetData() << std::endl;
  rti1516ev::VariableLengthData canFrameEncodedData = canFrame.encode();
  NDistSimIB::NRTFederateEncoding::CANFrameEncoding canFrameDecoder;
  canFrameDecoder.decode(canFrameEncodedData);
  std::cout << "canFrameDecoder.DataLength=" << (int)canFrameDecoder.GetDataLength() << std::endl;
  std::cout << "canFrameDecoder.Data=" << canFrameDecoder.GetData() << std::endl;
  if (canFrameDecoder.GetData() != canFrame.GetData())
  {
    std::cerr << "decoded data incorrect" << std::endl;
    return false;
  }
  NDistSimIB::NRTFederateEncoding::CANFrameEncoding canFrameCopy(canFrame);
  std::cout << "canFrameCopy.DataLength=" << (int)canFrameCopy.GetDataLength() << std::endl;
  std::cout << "canFrameCopy.Data=" << canFrameCopy.GetData() << std::endl;
  if (canFrameCopy.GetData() != canFrame.GetData())
  {
    std::cerr << "copied data incorrect" << std::endl;
    return false;
  }
  rti1516ev::VariableLengthData canFrameCopyEncodedData = canFrameCopy.encode();
  if (canFrameCopy.encode() != canFrame.encode())
  {
    std::cerr << "encoded data from copied can frame incorrect" << std::endl;
    std::cerr << "canFrame.encode     = " << canFrame.encode() << std::endl;
    std::cerr << "canFrameCopy.encode = " << canFrameCopy.encode() << std::endl;
    return false;
  }
  NDistSimIB::NRTFederateEncoding::CANFrameEncoding canFrameCopyDecoder;
  canFrameCopyDecoder.decode(canFrameCopyEncodedData);
  std::cout << "canFrameCopyDecoder.DataLength=" << (int)canFrameCopyDecoder.GetDataLength() << std::endl;
  std::cout << "canFrameCopyDecoder.Data=" << canFrameCopyDecoder.GetData() << std::endl;
  if (canFrameCopyDecoder.GetData() != canFrame.GetData())
  {
    std::cerr << "copied data incorrect" << std::endl;
    return false;
  }

  received = false;
  canFrame.SetDir(NDistSimIB::NRTFederateEncoding::DirMask::kMskTx);
  canMessageInteractionClass->send(false, std::string("CAN1"), NDistSimIB::NRTFederateEncoding::BusType::kBtCAN, ambassador.getFederateHandle(), busControllerCan.get(), busControllerCan.get(), 0x100, canFrame);
  while (!received)
    ambassador.getRtiAmbassador()->evokeCallback(0.5);

  received = false;
  canFrameCopy.SetDir(NDistSimIB::NRTFederateEncoding::DirMask::kMskRx);
  canMessageInteractionClass->send(false, std::string("CAN1"), NDistSimIB::NRTFederateEncoding::BusType::kBtCAN, ambassador.getFederateHandle(), busControllerCan.get(), busControllerCan.get(), 0x100, canFrameCopy);
  while (!received)
    ambassador.getRtiAmbassador()->evokeCallback(0.5);


  // tear down
  measurementInitInteractionClass->Unsubscribe();
  measurementInitInteractionClass->Unpublish();
  canMessageInteractionClass->Unsubscribe();
  canMessageInteractionClass->Unpublish();
  busControllerCan->UnregisterUpdateCallback(callbackToken);
  busControllerCan->Release();
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

