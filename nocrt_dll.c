/******************************************************************************\
* MIT No Attribution                                                           *
*                                                                              *
* Copyright 2023 Jaroslav Hensl <emulator@emulace.cz>                          *
*                                                                              *
* Permission is hereby granted, free of charge, to any person obtaining a copy *
* of this software and associated documentation files (the "Software"), to deal*
* in the Software without restriction, including without limitation the rights *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell    *
* copies of the Software, and to permit persons to whom the Software is        *
* furnished to do so.                                                          *
*                                                                              *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL      *
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER   *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,*
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS       *
* IN THE SOFTWARE.                                                             *
\******************************************************************************/

#include <windows.h>
#include "nocrt.h"

/* security cookie = buffer overrun protection */
#ifdef _WIN64
#define DEFAULT_SECURITY_COOKIE 0x00002B992DDFA232ll
#else
#define DEFAULT_SECURITY_COOKIE 0xBB40E64E
#endif

DECLSPEC_SELECTANY UINT_PTR __security_cookie = DEFAULT_SECURITY_COOKIE;
DECLSPEC_SELECTANY UINT_PTR __security_cookie_complement = ~(DEFAULT_SECURITY_COOKIE);

typedef union
{
  unsigned __int64 ft_scalar;
  FILETIME ft_struct;
} FT;

void __cdecl __security_init_cookie (void);

void __cdecl
__security_init_cookie (void)
{
  UINT_PTR cookie;
  FT systime = { 0, };
  LARGE_INTEGER perfctr;

  if (__security_cookie != DEFAULT_SECURITY_COOKIE)
    {
      __security_cookie_complement = ~__security_cookie;
      return;
    }

  GetSystemTimeAsFileTime (&systime.ft_struct);
#ifdef _WIN64
  cookie = systime.ft_scalar;
#else
  cookie = systime.ft_struct.dwLowDateTime;
  cookie ^= systime.ft_struct.dwHighDateTime;
#endif

  cookie ^= GetCurrentProcessId ();
  cookie ^= GetCurrentThreadId ();
  cookie ^= GetTickCount ();

  QueryPerformanceCounter (&perfctr);
#ifdef _WIN64
  cookie ^= perfctr.QuadPart;
#else
  cookie ^= perfctr.LowPart;
  cookie ^= perfctr.HighPart;
#endif

#ifdef _WIN64
  cookie &= 0x0000ffffffffffffll;
#endif

  if (cookie == DEFAULT_SECURITY_COOKIE)
    cookie = DEFAULT_SECURITY_COOKIE + 1;
  __security_cookie = cookie;
  __security_cookie_complement = ~cookie;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

/* DllMain caller and I/O initializer */
__declspec(noinline) WINBOOL
__DllMainCRTStartup (HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
  WINBOOL retcode;
  retcode = DllMain(hDllHandle, dwReason, lpreserved);
  return retcode ;
}

/* entry point */
WINBOOL WINAPI DllMainCRTStartup (HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
  if (dwReason == DLL_PROCESS_ATTACH)
  {
  	__security_init_cookie();
  }
  return __DllMainCRTStartup (hDllHandle, dwReason, lpreserved);
}


