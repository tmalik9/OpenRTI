#include "TranslateTypes.h"

namespace OpenRTI {


TranslateTypes::TranslateTypes()
{
}


TranslateTypes::~TranslateTypes()
{
}

FOMStringModule TranslateTypes::translate(const FOMStringModule2& ref)
{
  FOMStringModule result;
  result.setDesignator(ref.getDesignator());
  result.setTransportationTypeList(ref.getTransportationTypeList());
  result.setDimensionList(ref.getDimensionList());
  result.setRoutingSpaceList(ref.getRoutingSpaceList());
  result.setInteractionClassList(ref.getInteractionClassList());
  result.setObjectClassList(ref.getObjectClassList());
  result.setUpdateRateList(ref.getUpdateRateList());
  result.setSwitchList(ref.getSwitchList());
  result.setSimpleDataTypeList(ref.getSimpleDataTypeList());
  result.setEnumeratedDataTypeList(ref.getEnumeratedDataTypeList());
  result.setEnumeratedDataTypeList(ref.getEnumeratedDataTypeList());
  result.setArrayDataTypeList(translate(ref.getArrayDataTypeList()));
  //result.setFixedRecordDataTypeList(ref.getFixedRecordDataTypeList());
  //result.setVariantRecordDataTypeList(ref.getVariantRecordDataTypeList());
  result.setArtificialInteractionRoot(ref.getArtificialInteractionRoot());
  result.setArtificialObjectRoot(ref.getArtificialObjectRoot());
  return result;
}


FOMStringModuleList TranslateTypes::translate(const FOMStringModule2List& ref)
{
  FOMStringModuleList result;
  for (auto& fomModule : ref)
  {
    result.push_back(translate(fomModule));
  }
  return result;
}

FOMStringArrayDataType TranslateTypes::translate(const FOMStringArrayDataType2& ref)
{
  FOMStringArrayDataType result;
  result.setName(ref.getName());
  result.setEncoding(to_string(ref.getEncoding()));
  result.setDataType(ref.getDataType());
  result.setCardinality(ref.getCardinality());
  return result;
}

FOMStringArrayDataTypeList TranslateTypes::translate(const FOMStringArrayDataType2List& ref)
{
  FOMStringArrayDataTypeList result;
  for (auto& dataType : ref)
  {
    result.push_back(translate(dataType));
  }
  return result;
}

}