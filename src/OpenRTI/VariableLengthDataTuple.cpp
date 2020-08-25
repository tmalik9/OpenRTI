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

#include "DebugNew.h"
#include "OpenRTIConfig.h"
#include "Types.h"
#include "VariableLengthDataTuple.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iterator>

namespace OpenRTI {

// update index sets of tuple column 'positionInTuple'
void VariableLengthDataTupleSet::UpdateIndexSet(const VariableLengthData& value, VariableLengthDataTuple::size_type positionInTuple, VariableLengthDataTupleSet::ImplType::size_type newIndex)
{
  auto& valueIndexSet = mIndexSets[positionInTuple];
  // rows containing wildcards
  auto& wildcardIndexSet = mWildcardIndexSets[positionInTuple];
  ValueToIndexSet::iterator indexSetIter = valueIndexSet.find(value);
  if (indexSetIter == valueIndexSet.end())
  {
    // new value for tuple column: insert new set
    IndexSet indexSet;
    indexSet.insert(newIndex);
    // add rows containing wildcards to new index set - this is needed to catch the
    // wildcarded elements in the set intersection
    indexSet.insert(wildcardIndexSet.begin(), wildcardIndexSet.end());
    valueIndexSet.insert(ValueToIndexSet::value_type(value, indexSet));
  }
  else
  {
    // value already exists in tuple column: extend index set
    IndexSet& indexSet = indexSetIter->second;
    indexSet.insert(wildcardIndexSet.begin(), wildcardIndexSet.end());
    indexSet.insert(newIndex);
  }
  // wildcard handling: add index of new entry to every existing value's index set
  if (value.size() == 0)
  {
    wildcardIndexSet.insert(newIndex);
    for (auto& indexSetEntry : valueIndexSet)
    {
      IndexSet& indexSet = indexSetEntry.second;
      indexSet.insert(wildcardIndexSet.begin(), wildcardIndexSet.end());
    }
  }
}

void VariableLengthDataTupleSet::Insert(const VariableLengthDataTuple& newTuple)
{
  //std::cout << "Insert " << newTuple << std::endl;
  if (mTupleSize < newTuple.size())
  {
    //std::cout << "resizing tuples from " << mTupleSize << " to " << newTuple.size() << std::endl;
    mTupleSize = newTuple.size();
    for (auto& tuple : mImpl)
    {
      tuple.resize(mTupleSize);
    }
    mIndexSets.resize(mTupleSize);
    mWildcardIndexSets.resize(mTupleSize);
  }
  mImpl.push_back(newTuple);
  IndexType newIndex = mImpl.size();
  for (VariableLengthDataTuple::size_type positionInTuple = 0; positionInTuple < newTuple.size(); positionInTuple++)
  {
    UpdateIndexSet(newTuple[positionInTuple], positionInTuple, newIndex);
  }
  //std::cout << *this;
  //std::cout << "Done Insert " << newTuple << std::endl;
}

bool VariableLengthDataTupleSet::FindNormalized(const VariableLengthDataTuple& valueTuple) const
{
  //std::cout << "FindNormalized " << valueTuple << std::endl;
  if (valueTuple.size() != mTupleSize) return false;

  // step 1: for each column, build sets of matched row indices, and include these index sets to a list.
  std::list<IndexSet> resultIndexSets;
  for (VariableLengthDataTuple::size_type column = 0; column < mTupleSize; column++)
  {
    const auto& value = valueTuple[column];
    if (value.empty()) // given column value is a wildcard
      continue;
    const ValueToIndexSet& valueToIndexSet = mIndexSets[column];
    auto indexSetIter = valueToIndexSet.find(value);
    if (indexSetIter != valueToIndexSet.end())
    {
      // add found index set to list of sets to be intersected
      //std::cout << "column " << column << " value " << value << " => " << indexSetIter->second << std::endl;
      resultIndexSets.push_back(indexSetIter->second);
    }
    else
    {
      resultIndexSets.push_back(mWildcardIndexSets[column]);
    }
  }
  // step 2: build intersection of all index sets. The resulting set is the set of rows matching the value tuple.
  if (resultIndexSets.empty())
  {
    // trivial case: all value columns were wildcards
    return true;
  }
  IndexSet intersectionSet = resultIndexSets.front();
  resultIndexSets.pop_front();
  for (auto& indexSet : resultIndexSets)
  {
    IndexSet intersectionResult;
    //std::cout << intersectionSet << " X " << indexSet << "=";
    set_intersection(intersectionSet.begin(), intersectionSet.end(), indexSet.begin(), indexSet.end(), std::inserter(intersectionResult, intersectionResult.begin()));
    //std::cout << intersectionResult << std::endl;
    intersectionSet = intersectionResult;
  }
  //std::cout << "intersection set=" << intersectionSet << std::endl;
  return !intersectionSet.empty();
}


bool VariableLengthDataTupleSet::Contains(const VariableLengthDataTuple& newTuple) const
{
  for (auto& tuple : mImpl)
  {
    if (tuple == newTuple)
    {
      return true;
    }
  }
  return false;
}

std::ostream& operator<<(std::ostream& out, const VariableLengthDataTuple& vldTuple)
{
  out << "(";
  auto iter = vldTuple.begin();
  auto end = vldTuple.end();
  while (iter != end)
  {
    out << *iter;
    if (++iter != end) out << ", ";
  }
  out << ")";
  return out;
}


std::ostream& operator<<(std::ostream& out, const VariableLengthDataTupleSet::IndexSet& indexSet)
{
  out << "[";
  auto iter = indexSet.begin();
  auto end = indexSet.end();
  while (iter != end)
  {
    out << *iter;
    if (++iter != end) out << ", ";
  }
  out << "]";
  return out;
}

std::ostream& operator<<(std::ostream& out, const VariableLengthDataTupleSet& tupleSet)
{
  out << tupleSet.mImpl.size() << " tuples:" << std::endl;
  for (auto& tuple : tupleSet.mImpl)
  {
    out << tuple << std::endl;
  }
  for (VariableLengthDataTuple::size_type positionInTuple = 0; positionInTuple < tupleSet.mIndexSets.size(); positionInTuple++)
  {
    out << "IndexSet " << positionInTuple << ": ";
    const VariableLengthDataTupleSet::ValueToIndexSet& valueToIndexSet = tupleSet.mIndexSets[positionInTuple];
    for (const auto& valueToIndexSetEntry : valueToIndexSet)
    {
      const auto& value = valueToIndexSetEntry.first;
      const auto& indexSet = valueToIndexSetEntry.second;
      out << "value " << value << " => indexSet " << indexSet << std::endl;
    }
    out << "WildcardIndexSet " << positionInTuple << ": ";
    const VariableLengthDataTupleSet::IndexSet& wildcardIndexSet = tupleSet.mWildcardIndexSets[positionInTuple];
    out << wildcardIndexSet << std::endl;
  }
  return out;
}


} // namespace OpenRTI
