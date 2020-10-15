/* -*-c++-*- OpenRTI - Copyright (C) 2009-2015 Mathias Froehlich
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

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "ServerModel.h"

#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_WILDCARD -1
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_INVALID   0
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_CAN       1
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_unused    2
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_VAN       3
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_TTP       4
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_LIN       5
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_MOST      6
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_FLEXRAY   7
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_BEAN      8
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_J1708     9
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_DAIO      10
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_ETHERNET  11
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_GLLOGGER  12
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_WLAN      13
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_AFDX      14
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_KLINE     15
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_A429      16
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_PSI5      17
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_SENT      18
#define YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_MAXINDEX  18

#define YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_WILDCARD -1
#define YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_INVALID   0

enum VBusType : int32_t
{
  kBtInvalid  = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_INVALID,
  kBtCAN      = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_CAN,
  kBtVAN      = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_VAN,
  kBtLIN      = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_LIN,
  kBtMOST     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_MOST,
  kBtFlexRay  = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_FLEXRAY,
  kBtBEAN     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_BEAN,    // note: Option BEAN has been removed in version 8.0
  kBtJ1708    = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_J1708,
  kBtDAIO     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_DAIO,
  kBtEthernet = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_ETHERNET,
  kBtGlLogger = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_GLLOGGER,
  kBtWlan     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_WLAN,
  kBtAFDX     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_AFDX,
  kBtKLine    = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_KLINE,
  kBtA429     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_A429,
  kBtPSI5     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_PSI5,
  kBtSent     = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_SENT,
#ifdef V_HAS_TTCAN
  kBtTTCAN    = YOU_MAY_USE_THIS_DEFINITION_FOR_BUSTYPE_TTCAN,
#endif
  kBtWildcard = YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_WILDCARD
};
typedef uint16_t          VAppChannelNumber;

const VAppChannelNumber kChannelInvalid = ( VAppChannelNumber)YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_INVALID;
const VAppChannelNumber kChannelWildcard = ( VAppChannelNumber)YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_WILDCARD;

class VAppChannel
{
public:
    VAppChannel( VAppChannelNumber appChannelNumber,VBusType busType):
    mAppChannelNumber (appChannelNumber),
    mBusType          (busType)
    {};

    VAppChannel():
    mAppChannelNumber(kChannelInvalid),
    mBusType(kBtInvalid)
    {}
      
  bool operator ==( const VAppChannel& other) const
  {
    return mAppChannelNumber == other.mAppChannelNumber &&
           mBusType == other.mBusType;
  }

  bool Matches(const VAppChannel& other) const
  {
    bool const channelNumberMatches = (mAppChannelNumber == other.mAppChannelNumber)
      || (mAppChannelNumber == (VAppChannelNumber)YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_WILDCARD)
      || (other.mAppChannelNumber == (VAppChannelNumber)YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_WILDCARD);
    bool const busTypesMatch = (mBusType == other.mBusType)
      || (mBusType == (VBusType)YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_WILDCARD)
      || (other.mBusType == (VBusType)YOU_MAY_USE_THIS_DEFINITION_FOR_CHANNEL_WILDCARD);
    return (channelNumberMatches && busTypesMatch);
  }

  bool operator !=( const VAppChannel& other) const
  {
    return !( *this == other);
  }

  bool operator <(const VAppChannel& other) const
  {
      return mBusType == other.mBusType ? mAppChannelNumber < other.mAppChannelNumber : mBusType < other.mBusType;
  }

  // lm 22.01.01: nur den Kanal alleine: v.a. für die vielen Stellen alten codes (COM!?)
  //              die eben nur mit dem Kanal hantieren...
  VAppChannelNumber GetAppChannelNumber() const { return mAppChannelNumber;};
  // lm 22.01.01: i.A. sollte auch der Bustyp interessant sein
  VBusType          GetBusType() const { return mBusType;};

  bool IsValid() const { return mAppChannelNumber != kChannelInvalid && mBusType != kBtInvalid; }
    
protected:
  VAppChannelNumber mAppChannelNumber;
  VBusType          mBusType;
};

namespace OpenRTI {

struct ParameterTest1
{
  ParameterTest1() 
  : _node(), _federation(_node), _interactionClass(_federation) {}

  ParameterValueVector BuildParameterValuesChannel(VAppChannel channel)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue1;
    VariableLengthData data1(&channel, sizeof(channel));

    ServerModel::ParameterDefinition* pParamDef1 = _interactionClass.getParameterDefinition(std::string("Channel"));

    paramValue1.setParameterHandle(pParamDef1->getParameterHandle());
    paramValue1.setValue(data1);

    parameters.push_back(paramValue1);
    return parameters;
  }

  ParameterValueVector BuildParameterValues1(uint32_t v1)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue1;
    VariableLengthData data1(&v1, sizeof(v1));

    ServerModel::ParameterDefinition* pParamDef1 = _interactionClass.getParameterDefinition(std::string("A"));

    paramValue1.setParameterHandle(pParamDef1->getParameterHandle());
    paramValue1.setValue(data1);

    parameters.push_back(paramValue1);
    return parameters;
  }
  ParameterValueVector BuildParameterValues2(uint32_t v2)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue2;
    VariableLengthData data2(&v2, sizeof(v2));

    ServerModel::ParameterDefinition* pParamDef2 = _interactionClass.getParameterDefinition(std::string("B"));

    paramValue2.setParameterHandle(pParamDef2->getParameterHandle());
    paramValue2.setValue(data2);

    parameters.push_back(paramValue2);
    return parameters;
  }
  ParameterValueVector BuildParameterValues3(uint32_t v3)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue3;
    VariableLengthData data3(&v3, sizeof(v3));

    ServerModel::ParameterDefinition* pParamDef3 = _interactionClass.getParameterDefinition(std::string("C"));

    paramValue3.setParameterHandle(pParamDef3->getParameterHandle());
    paramValue3.setValue(data3);

    parameters.push_back(paramValue3);
    return parameters;
  }
  ParameterValueVector BuildParameterValues12(uint32_t v1, uint32_t v2)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue1;
    ParameterValue paramValue2;
    VariableLengthData data1(&v1, sizeof(v1));
    VariableLengthData data2(&v2, sizeof(v2));

    ServerModel::ParameterDefinition* pParamDef1 = _interactionClass.getParameterDefinition(std::string("A"));
    ServerModel::ParameterDefinition* pParamDef2 = _interactionClass.getParameterDefinition(std::string("B"));

    paramValue1.setParameterHandle(pParamDef1->getParameterHandle());
    paramValue1.setValue(data1);
    paramValue2.setParameterHandle(pParamDef2->getParameterHandle());
    paramValue2.setValue(data2);

    parameters.push_back(paramValue1);
    parameters.push_back(paramValue2);
    return parameters;
  }

  ParameterValueVector BuildParameterValues23(uint32_t v2, uint32_t v3)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue2;
    ParameterValue paramValue3;
    VariableLengthData data2(&v2, sizeof(v2));
    VariableLengthData data3(&v3, sizeof(v3));

    ServerModel::ParameterDefinition* pParamDef2 = _interactionClass.getParameterDefinition(std::string("B"));
    ServerModel::ParameterDefinition* pParamDef3 = _interactionClass.getParameterDefinition(std::string("C"));

    paramValue2.setParameterHandle(pParamDef2->getParameterHandle());
    paramValue2.setValue(data2);
    paramValue3.setParameterHandle(pParamDef3->getParameterHandle());
    paramValue3.setValue(data3);

    parameters.push_back(paramValue2);
    parameters.push_back(paramValue3);
    return parameters;
  }
  ParameterValueVector BuildParameterValues13(uint32_t v1, uint32_t v3)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue1;
    ParameterValue paramValue3;
    VariableLengthData data1(&v1, sizeof(v1));
    VariableLengthData data3(&v3, sizeof(v3));

    ServerModel::ParameterDefinition* pParamDef1 = _interactionClass.getParameterDefinition(std::string("A"));
    ServerModel::ParameterDefinition* pParamDef3 = _interactionClass.getParameterDefinition(std::string("C"));

    paramValue1.setParameterHandle(pParamDef1->getParameterHandle());
    paramValue1.setValue(data1);
    paramValue3.setParameterHandle(pParamDef3->getParameterHandle());
    paramValue3.setValue(data3);

    parameters.push_back(paramValue1);
    parameters.push_back(paramValue3);
    return parameters;
  }

  ParameterValueVector BuildParameterValues123(uint32_t v1, uint32_t v2, uint32_t v3)
  {
    ParameterValueVector parameters;
    ParameterValue paramValue1;
    ParameterValue paramValue2;
    ParameterValue paramValue3;
    VariableLengthData data1(&v1, sizeof(v1));
    VariableLengthData data2(&v2, sizeof(v2));
    VariableLengthData data3(&v3, sizeof(v3));

    ServerModel::ParameterDefinition* pParamDef1 = _interactionClass.getParameterDefinition(std::string("A"));
    ServerModel::ParameterDefinition* pParamDef2 = _interactionClass.getParameterDefinition(std::string("B"));
    ServerModel::ParameterDefinition* pParamDef3 = _interactionClass.getParameterDefinition(std::string("C"));

    paramValue1.setParameterHandle(pParamDef1->getParameterHandle());
    paramValue1.setValue(data1);
    paramValue2.setParameterHandle(pParamDef2->getParameterHandle());
    paramValue2.setValue(data2);
    paramValue3.setParameterHandle(pParamDef3->getParameterHandle());
    paramValue3.setValue(data3);

    parameters.push_back(paramValue1);
    parameters.push_back(paramValue2);
    parameters.push_back(paramValue3);
    return parameters;
  }
  

  bool exec()
  {
    ServerModel::ParameterDefinition paramDef1(_interactionClass);
    ServerModel::ParameterDefinition paramDef2(_interactionClass);
    ServerModel::ParameterDefinition paramDef3(_interactionClass);
    ServerModel::ParameterDefinition paramDef4(_interactionClass);
    ServerModel::ParameterDefinition paramDef5(_interactionClass);
    ConnectHandle connect(1);

    ParameterHandle paramHandle1 = ParameterHandle(1);
    paramDef1.setParameterHandle(paramHandle1);
    paramDef1.setName("A");
    _interactionClass.insert(paramDef1);

    ParameterHandle paramHandle2 = ParameterHandle(2);
    paramDef2.setParameterHandle(paramHandle2);
    paramDef2.setName("B");
    _interactionClass.insert(paramDef2);

    ParameterHandle paramHandle3 = ParameterHandle(3);
    paramDef3.setParameterHandle(paramHandle3);
    paramDef3.setName("C");
    _interactionClass.insert(paramDef3);

    // NOTE: never used, just for testing the algorithm
    ParameterHandle paramHandle4 = ParameterHandle(4);
    paramDef4.setParameterHandle(paramHandle4);
    paramDef4.setName("D");
    _interactionClass.insert(paramDef4);

    ParameterHandle paramHandle5 = ParameterHandle(5);
    paramDef5.setParameterHandle(paramHandle5);
    paramDef5.setName("Channel");
    _interactionClass.insert(paramDef5);

    ParameterValueVector parameters1 = BuildParameterValues123(1,2,3);
    std::cout << "update(1,2,3) => " << _interactionClass.updateParameterFilterValues(connect, parameters1) << std::endl;
    ParameterValueVector parameters2 = BuildParameterValues123(2,3,4);
    std::cout << "update(2,3,4) => " << _interactionClass.updateParameterFilterValues(connect, parameters2) << std::endl;
    ParameterValueVector parameters3 = BuildParameterValues12(2,3);
    std::cout << "update(2,3,*) => " << _interactionClass.updateParameterFilterValues(connect, parameters3) << std::endl;
    ParameterValueVector parameters4 = BuildParameterValues23(4,5);
    std::cout << "update(*,4,5) => " << _interactionClass.updateParameterFilterValues(connect, parameters4) << std::endl;
    ParameterValueVector parameters5 = BuildParameterValues123(0x88,0xCC,0xFF);
    std::cout << "update(0x88,0xCC,0xFF) => " << _interactionClass.updateParameterFilterValues(connect, parameters5) << std::endl;
    ParameterValueVector parameters6 = BuildParameterValuesChannel(VAppChannel(1, kBtCAN));
    std::cout << "update(CAN1) => " << _interactionClass.updateParameterFilterValues(connect, parameters6) << std::endl;
    ParameterValueVector parameters7 = BuildParameterValuesChannel(VAppChannel(2, kBtCAN));
    std::cout << "update(CAN2) => " << _interactionClass.updateParameterFilterValues(connect, parameters7) << std::endl;
    ParameterValueVector parameters8 = BuildParameterValuesChannel(VAppChannel(1, kBtEthernet));
    std::cout << "update(Eth1) => " << _interactionClass.updateParameterFilterValues(connect, parameters8) << std::endl;



    // match the tuples we just inserted
    std::cout << "isMatching(1,2,3) (INIT) => " << _interactionClass.isMatching(connect, parameters1) << std::endl;
    std::cout << "isMatching(2,3,4) (INIT) => " << _interactionClass.isMatching(connect, parameters2) << std::endl;
    std::cout << "isMatching(2,3,*) (INIT) => " << _interactionClass.isMatching(connect, parameters3) << std::endl;
    std::cout << "isMatching(*,4,5) (INIT) => " << _interactionClass.isMatching(connect, parameters4) << std::endl;
    std::cout << "isMatching(0x88,0xCC,0xFF) (INIT) => " << _interactionClass.isMatching(connect, parameters5) << std::endl;
    std::cout << "isMatching(CAN1) (INIT) => " << _interactionClass.isMatching(connect, parameters6) << std::endl;
    std::cout << "isMatching(CAN2) (INIT) => " << _interactionClass.isMatching(connect, parameters7) << std::endl;

    ParameterValueVector input1 = BuildParameterValues12(2,3);
    std::cout << "isMatching(2,3,*) (YES) => " << _interactionClass.isMatching(connect, input1) << std::endl;
    ParameterValueVector input2 = BuildParameterValues23(2,3);
    std::cout << "isMatching(*,2,3) (YES) => " << _interactionClass.isMatching(connect, input2) << std::endl;
    ParameterValueVector input3 = BuildParameterValues13(2,4);
    std::cout << "isMatching(2,*,4) (YES) => " << _interactionClass.isMatching(connect, input3) << std::endl;
    ParameterValueVector input4 = BuildParameterValues123(1,4,5);
    std::cout << "isMatching(1,4,5) (YES) => " << _interactionClass.isMatching(connect, input4) << std::endl;
    ParameterValueVector parameters9 = BuildParameterValues23(2,3);
    std::cout << "isMatching(*,2,3) (YES) => " << _interactionClass.isMatching(connect, parameters9) << std::endl;
    ParameterValueVector parameters10 = BuildParameterValues123(3,2,1);
    std::cout << "isMatching(3,2,1) (NO) => " << _interactionClass.isMatching(connect, parameters10) << std::endl;
    ParameterValueVector parameters11 = BuildParameterValues2(2);
    std::cout << "isMatching(*,2,*) (YES) => " << _interactionClass.isMatching(connect, parameters11) << std::endl;
    ParameterValueVector parameters12 = BuildParameterValues2(1);
    std::cout << "isMatching(*,1,*) (NO) => " << _interactionClass.isMatching(connect, parameters12) << std::endl;
    ParameterValueVector parameters13;
    std::cout << "isMatching(*,*,*) (YES) => " << _interactionClass.isMatching(connect, parameters13) << std::endl;
    return true;
  }
  ServerModel::Node _node;
  ServerModel::Federation _federation;
  ServerModel::InteractionClass _interactionClass;
};

}

int
main(int argc, char* argv[])
{
  OpenRTI::ParameterTest1 test1;
  if (!test1.exec()) {
    std::cerr << "ThreadTest failed!" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
