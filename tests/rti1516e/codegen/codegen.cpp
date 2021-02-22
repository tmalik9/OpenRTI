
#include <iostream>

#include "momDataTypes.h"
#include "momEncodings.h"
#include "../../../src/rti1516e/HandleFactory.h"
#include "RTI1516ETestLib.h"

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
operator<<(std::basic_ostream<char_type, traits_type>& os, const rti1516e::VariableLengthData& value)
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

using namespace rti1516e;
bool testHLAhandleListEncoding()
{
  std::vector<HLAhandle> handleList;
  for (int i = 0; i < 5; i++)
  {
    handleList.push_back(rti1516e::HLAhandle(rti1516e::createFederateHandle(i)));
  }
  OpenRTI::Mom::HLAhandleListEncoding handleListEncoding;
  handleListEncoding.set(handleList);
  for (int i = 0; i < 5; i++)
  {
    std::wcout << static_cast<const HLAhandle&>(handleListEncoding[i]).getFederateHandle() << std::endl;
  }
  rti1516e::VariableLengthData encodedData = handleListEncoding.encode();
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
    objectClassBasedCount.SetHLAobjectClass(rti1516e::HLAhandle(rti1516e::createObjectClassHandle(i)));
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
  rti1516e::VariableLengthData encodedData = objectClassBasedCountsEncoding.encode();
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

int main(int /*argc*/, char* /*argv*/[])
{
  if (!testHLAhandleListEncoding())
    return EXIT_FAILURE;
  if (!testHLAobjectClassBasedCountsEncoding())
    return EXIT_FAILURE;
}

