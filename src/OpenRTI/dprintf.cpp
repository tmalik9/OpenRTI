

#include "DebugNew.h"
#ifndef linux
#include <Windows.h>
#endif

#include <cstdio>
#include "dprintf.h"
#include <iostream>

namespace OpenRTI {
#if defined(_WIN32)
#undef snprintf
#define snprintf _snprintf
#undef vsnprintf
#define vsnprintf _vsnprintf
#endif

OPENRTI_API bool sEnableDebugPrintf = true;
OPENRTI_API bool sDebugToConsole = true;
OPENRTI_API void DebugPrintf(__format_string const char* fmt, ...)
{
  if (sEnableDebugPrintf)
  {
#if defined(_WIN32)
    bool errorState = false;
    static const char* errorFormat = "error in format string or argument list: %s: %s";
    std::string error;
    va_list args;
    va_start(args, fmt);
    DWORD threadId = ::GetCurrentThreadId();
    DWORD processId = ::GetCurrentProcessId();
    int bufsize1 = _snprintf(nullptr, 0, "[%u:%u] ", (uint32_t)processId, (uint32_t)threadId);
    int bufsize2 = _vsnprintf(nullptr, 0, fmt, args);
    if (bufsize2 < 0)
    {
      errorState = true;
      error = strerror(errno);
      bufsize2 = _snprintf(nullptr, 0, errorFormat, fmt, error.c_str());
    }
    int bufsize = bufsize1 + bufsize2 + 1;

    char* buffer = new char[bufsize];
    _snprintf(buffer, bufsize1, "[%u:%u] ", (uint32_t)processId, (uint32_t)threadId);
    if (!errorState)
    {
      _vsnprintf(buffer + bufsize1, bufsize2 + 1, fmt, args);
    }
    else
    {
      bufsize2 = _snprintf(buffer + bufsize1, bufsize2 + 1, errorFormat, fmt, error.c_str());
    }
    va_end(args);
    //fputs(buffer, stdout);
    OutputDebugStringA(buffer);
    if (sDebugToConsole)
    {
      std::cout << buffer;
    }
    delete [] buffer;
#elif defined(__linux__)
#endif
  }
}

OPENRTI_API std::string string_vprintf(const char* fmt, va_list args)
{
#if defined(_WIN32)
  int bufsiz = _vscprintf(fmt, args) + 1;
  char* buffer = new char[bufsiz];
  _vsnprintf(buffer, bufsiz, fmt, args);
  std::string result(buffer);
  delete [] buffer;
  return result;
#elif defined(__linux__)
  int bufsiz = 124;
  int nWrittenLen = 0;

  char* buffer;
  char* buffer1;
  buffer = (char*)malloc(bufsiz);

  while( true )
  {
    // See https://sourceware.org/bugzilla/show_bug.cgi?id=4977 - that's a really nasty one:
    // a va_list must never be reused in a second call to a function, such as vsnprintf, otherwise crashes are likely
    va_list argsCopy;
    va_copy (argsCopy, args);
    nWrittenLen = vsnprintf(buffer, bufsiz, fmt, argsCopy);
    va_end(argsCopy);
    if (nWrittenLen < 0) // error
    {
      free(buffer);
      return std::string();
    }
    else if (nWrittenLen < bufsiz) // OK
    {
      std::string result(buffer);
      free(buffer);
      return result;
    }
    else // buffer too small
    {
      bufsiz = nWrittenLen + 1;

      if ((buffer1 = (char*)realloc(buffer, bufsiz)) == NULL)
      {
        free(buffer);
        return std::string();
      }
      else {
        buffer = buffer1;
      }
    }
  }

  return std::string();
#endif
}

OPENRTI_API std::string string_printf(__format_string const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  std::string result = string_vprintf(fmt, args);
  va_end(args);
  return result;
}

} // namespace OpenRTI
