/* -*-c++-*- OpenRTI - Copyright (C) 2011-2012 Mathias Froehlich
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
namespace OpenRTI {

static inline size_t encodeIntoLE64(Octet* buffer, size_t bufferSize, size_t offset, uint64_t value)
{
  if (bufferSize < offset + 8)
  {
    throw EncoderException("provided buffer too small");
  }
  Octet* p = buffer + offset;
  *p++ = Octet(0xff & (value));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 24));
  *p++ = Octet(0xff & (value >> 32));
  *p++ = Octet(0xff & (value >> 40));
  *p++ = Octet(0xff & (value >> 48));
  *p++ = Octet(0xff & (value >> 56));
  return offset + 8;
}

static inline size_t encodeIntoBE64(Octet* buffer, size_t bufferSize, size_t offset, uint64_t value)
{
  if (bufferSize < offset + 8)
  {
    throw EncoderException("provided buffer too small");
  }
  Octet* p = buffer + offset;
  *p++ = Octet(0xff & (value >> 56));
  *p++ = Octet(0xff & (value >> 48));
  *p++ = Octet(0xff & (value >> 40));
  *p++ = Octet(0xff & (value >> 32));
  *p++ = Octet(0xff & (value >> 24));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value));
  return offset + 8;
}

static inline size_t encodeIntoLE32(Octet* buffer, size_t bufferSize, size_t offset, uint32_t value)
{
  if (bufferSize < offset + 4)
  {
    throw EncoderException("provided buffer too small");
  }
  Octet* p = buffer + offset;
  *p++ = Octet(0xff & (value));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 24));
  return offset + 4;
}

static inline size_t encodeIntoBE32(Octet* buffer, size_t bufferSize, size_t offset, uint32_t value)
{
  if (bufferSize < offset + 4)
  {
    throw EncoderException("provided buffer too small");
  }
  Octet* p = buffer + offset;
  *p++ = Octet(0xff & (value >> 24));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value));
  return offset + 4;
}

static inline size_t decodeFromBE32(const Octet* buffer, size_t bufferSize, size_t offset, uint32_t& value)
{
  if (bufferSize < offset + 4)
  {
    throw EncoderException("provided buffer too small");
  }
  value = size_t(buffer[offset]) << 24;
  value |= size_t(buffer[offset + 1]) << 16;
  value |= size_t(buffer[offset + 2]) << 8;
  value |= size_t(buffer[offset + 3]);
  return offset + 4;
}

static inline size_t encodeIntoLE16(Octet* buffer, size_t bufferSize, size_t offset, uint16_t value)
{
  if (bufferSize < offset + 2)
  {
    throw EncoderException("provided buffer too small");
  }
  Octet* p = buffer + offset;
  *p++ = Octet(0xff & (value));
  *p++ = Octet(0xff & (value >> 8));
  return offset + 2;
}

static inline size_t encodeIntoBE16(Octet* buffer, size_t bufferSize, size_t offset, uint16_t value)
{
  if (bufferSize < offset + 2)
  {
    throw EncoderException("provided buffer too small");
  }
  Octet* p = buffer + offset;
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value));
  return offset + 2;
}

}