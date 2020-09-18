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

namespace rti1516e {

static inline size_t align(size_t offset, size_t octetBoundary)
{
  return (offset + octetBoundary - 1) & ~(octetBoundary - 1);
}

static inline size_t align(void* buffer, size_t offset, size_t octetBoundary)
{
  size_t alignedOffset = (offset + octetBoundary - 1) & ~(octetBoundary - 1);
  uint8_t *p=static_cast<uint8_t*>(buffer)+offset;
  uint8_t *end=static_cast<uint8_t*>(buffer)+alignedOffset;
  while (p != end) *p++ = 0;
  return alignedOffset;
}

static inline void align(std::vector<Octet>& buffer, size_t octetBoundary)
{
  buffer.resize(align(buffer.size(), octetBoundary), 0);
}

static inline size_t encodeIntoLE64(Octet* buffer, size_t bufferSize, size_t offset, uint64_t value)
{
  size_t alignedOffset = align(buffer, offset, 8);
  if (bufferSize < offset + 8)
  {
    throw EncoderException(L"provided buffer too small");
  }
  Octet* p = buffer + alignedOffset;
  *p++ = Octet(0xff & (value));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 24));
  *p++ = Octet(0xff & (value >> 32));
  *p++ = Octet(0xff & (value >> 40));
  *p++ = Octet(0xff & (value >> 48));
  *p++ = Octet(0xff & (value >> 56));
  return alignedOffset + 8;
}

static inline size_t encodeIntoBE64(Octet* buffer, size_t bufferSize, size_t offset, uint64_t value)
{
  size_t alignedOffset = align(buffer, offset, 8);
  if (bufferSize < offset + 8)
  {
    throw EncoderException(L"provided buffer too small");
  }
  Octet* p = buffer + alignedOffset;
  *p++ = Octet(0xff & (value >> 56));
  *p++ = Octet(0xff & (value >> 48));
  *p++ = Octet(0xff & (value >> 40));
  *p++ = Octet(0xff & (value >> 32));
  *p++ = Octet(0xff & (value >> 24));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value));
  return alignedOffset + 8;
}

static inline size_t encodeIntoLE32(Octet* buffer, size_t bufferSize, size_t offset, uint32_t value)
{
  size_t alignedOffset = align(buffer, offset, 4);
  if (bufferSize < offset + 4)
  {
    throw EncoderException(L"provided buffer too small");
  }
  Octet* p = buffer + alignedOffset;
  *p++ = Octet(0xff & (value));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 24));
  return alignedOffset + 4;
}

static inline size_t encodeIntoBE32(Octet* buffer, size_t bufferSize, size_t offset, uint32_t value)
{
  size_t alignedOffset = align(buffer, offset, 4);
  if (bufferSize < offset + 4)
  {
    throw EncoderException(L"provided buffer too small");
  }
  Octet* p = buffer + alignedOffset;
  *p++ = Octet(0xff & (value >> 24));
  *p++ = Octet(0xff & (value >> 16));
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value));
  return alignedOffset + 4;
}

static inline size_t decodeFromBE32(const Octet* buffer, size_t bufferSize, size_t offset, uint32_t& value)
{
  if (bufferSize < offset + 4)
  {
    throw EncoderException(L"provided buffer too small");
  }
  value = size_t(buffer[offset]) << 24;
  value |= size_t(buffer[offset + 1]) << 16;
  value |= size_t(buffer[offset + 2]) << 8;
  value |= size_t(buffer[offset + 3]);
  return offset + 4;
}

static inline size_t encodeIntoLE16(Octet* buffer, size_t bufferSize, size_t offset, uint16_t value)
{
  size_t alignedOffset = align(buffer, offset, 2);
  if (bufferSize < alignedOffset + 2)
  {
    throw EncoderException(L"provided buffer too small");
  }
  Octet* p = buffer + alignedOffset;
  *p++ = Octet(0xff & (value));
  *p++ = Octet(0xff & (value >> 8));
  return alignedOffset + 2;
}

static inline size_t encodeIntoBE16(Octet* buffer, size_t bufferSize, size_t offset, uint16_t value)
{
  size_t alignedOffset = align(buffer, offset, 2);
  if (bufferSize < alignedOffset + 2)
  {
    throw EncoderException(L"provided buffer too small");
  }
  Octet* p = buffer + alignedOffset;
  *p++ = Octet(0xff & (value >> 8));
  *p++ = Octet(0xff & (value));
  return alignedOffset + 2;
}

}