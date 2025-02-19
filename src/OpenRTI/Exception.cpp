/* -*-c++-*- OpenRTI - Copyright (C) 2009-2012 Mathias Froehlich
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
#include "Exception.h"

#include <string>
#include <sstream>

#include "Export.h"
#include "StringUtils.h"
#include "LogStream.h"

#pragma warning(disable: 4100)

namespace OpenRTI {

Exception::Exception(const char* type, const char* reason) :
  _reason(asciiToUtf8(reason))
{
  Log(Assert, Info) << "Fired exception: type = " << type << ", reason = " << _reason << std::endl;
}

Exception::Exception(const char* type, const std::string& reason) :
  _reason(reason)
{
  Log(Assert, Info) << "Fired exception: type = " << type << ", reason = " << _reason << std::endl;
}

Exception::~Exception() noexcept
{
}

const char*
Exception::what() const
{
  return _reason.c_str();
}

const std::string&
Exception::getReason() const
{
  return _reason;
}


RTIinternalError::RTIinternalError(const char* reason) :
  Exception("RTIinternalError", reason)
{
}

RTIinternalError::RTIinternalError(const std::string& reason) :
  Exception("RTIinternalError", reason)
{
}

RTIinternalError::~RTIinternalError() noexcept
{
}

void
RTIinternalError::assertMessage(const char* file, unsigned line, const char* reason)
{
  std::stringstream stream;
  stream << "Assertion in " << file << " at line " << line;
  if (reason) {
    stream << ": " << reason;
  }

  throw RTIinternalError(stream.str());
}

} // namespace OpenRTI
