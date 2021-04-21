/* -*-c++-*- OpenRTI - Copyright (C) 2004-2014 Mathias Froehlich
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

#ifndef OpenRTI_Referenced_h
#define OpenRTI_Referenced_h

#include "Export.h"
#include "OpenRTIConfig.h"
#include "Atomic.h"

namespace OpenRTI {

class OPENRTI_API Referenced {
  public:
    Referenced() noexcept : _refcount(0u)
    {}
    /// Do not copy reference counts. Each new object has it's own counter
    Referenced(const Referenced&) noexcept : _refcount(0u) {}
    Referenced(Referenced&&) = delete;
    virtual ~Referenced() noexcept {}
    /// Do not copy reference counts. Each new object has it's own counter
    Referenced& operator=(const Referenced&) noexcept { return *this; }
    Referenced& operator=(Referenced&&) = delete;

    static void incRef(const Referenced* ref) noexcept {
      if (ref != nullptr) ref->_refcount.incFetch(Atomic::MemoryOrderRelease);
    }
    static void getFirst(const Referenced* ref) { incRef(ref); }
    static unsigned decRef(const Referenced* ref) noexcept {
      if (ref != nullptr) return ref->_refcount.decFetch(Atomic::MemoryOrderAcqRel);
      else return ~0u;
    }
    static void release(const Referenced* ref) noexcept {
      if (ref != nullptr) ref->_refcount.decFetch(Atomic::MemoryOrderRelease);
    }
    static unsigned count(const Referenced* ref) noexcept {
      if (ref != nullptr) return ref->_refcount;
      else return 0u;
    }
    static bool shared(const Referenced* ref) noexcept {
      if (ref != nullptr) return 1u < ref->_refcount;
      else return false;
    }

    template<typename T>
    static void destruct(T* ref) { delete ref; }

  private:
    mutable Atomic _refcount;
};

} // namespace OpenRTI

#endif
