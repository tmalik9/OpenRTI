/* -*-c++-*- OpenRTI - Copyright (C) 2004-2012 Mathias Froehlich
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
#include "ThreadLocal.h"

#include <vector>
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <windows.h>
#include "Referenced.h"
#include "SharedPtr.h"


namespace OpenRTI {

struct OPENRTI_LOCAL AbstractThreadLocal::_Provider : public Referenced {
  typedef std::vector<AbstractThreadLocal::_AbstractData*> ThreadLocalVector;


  _Provider() noexcept;
  _Provider(const _Provider&) = delete;
  _Provider(_Provider&&) = default;
  ~_Provider() noexcept;
  _Provider& operator=(const _Provider&) = delete;
  _Provider& operator=(_Provider&&) = default;
  static AbstractThreadLocal::_Provider& GetInstance();

  unsigned getNextIndex();

  _AbstractData* getData(unsigned index);
  void setData(unsigned index, _AbstractData* abstractThreadLocal);

  ThreadLocalVector* _tlsVector();
  DWORD _key;
  unsigned _index;
};

AbstractThreadLocal::_Provider::_Provider() noexcept
  : _key(TlsAlloc()), _index(0)
{
}

AbstractThreadLocal::_Provider::~_Provider() noexcept
{
  TlsFree(_key);
  _key = TLS_OUT_OF_INDEXES;
}

AbstractThreadLocal::_Provider& AbstractThreadLocal::_Provider::GetInstance()
{
  static AbstractThreadLocal::_Provider _instance;
  return _instance;
}


unsigned
AbstractThreadLocal::_Provider::getNextIndex()
{
  return _index++;
}

AbstractThreadLocal::_AbstractData*
AbstractThreadLocal::_Provider::getData(unsigned index)
{
  ThreadLocalVector* tlsVector = _tlsVector();
  if (!tlsVector)
    return 0;
  if (tlsVector->size() <= index)
    return 0;
  return (*tlsVector)[index];
}

void
AbstractThreadLocal::_Provider::setData(unsigned index, AbstractThreadLocal::_AbstractData* abstractThreadLocal)
{
  ThreadLocalVector* tlsVector = _tlsVector();
  if (!tlsVector)
    return;
  if (tlsVector->size() <= index)
    tlsVector->resize(index + 1, 0);
  delete (*tlsVector)[index];
  (*tlsVector)[index] = abstractThreadLocal;
}

AbstractThreadLocal::_Provider::ThreadLocalVector*
AbstractThreadLocal::_Provider::_tlsVector()
{
  ThreadLocalVector* tlsVector;
  tlsVector = static_cast<ThreadLocalVector*>(TlsGetValue(_key));
  if (tlsVector)
    return tlsVector;

  tlsVector = new ThreadLocalVector;
  TlsSetValue(_key, tlsVector);
  return tlsVector;
}

AbstractThreadLocal::_AbstractData::~_AbstractData()
{
}

AbstractThreadLocal::AbstractThreadLocal() :
  _index(~0u)
{
  _Provider& instance = _Provider::GetInstance();
  _index = instance.getNextIndex();
}

AbstractThreadLocal::~AbstractThreadLocal()
{
}

AbstractThreadLocal::_AbstractData*
AbstractThreadLocal::_get()
{
  _Provider& instance = _Provider::GetInstance();
  return instance.getData(_index);
}

void
AbstractThreadLocal::_set(AbstractThreadLocal::_AbstractData* abstractThreadLocal)
{
  _Provider& instance = _Provider::GetInstance();
  if (_index == ~0u)
    return;
  instance.setData(_index, abstractThreadLocal);
}


void AbstractThreadLocal::shutdown()
{
  //_Provider::GetInstance().reset();
}

} // namespace OpenRTI
