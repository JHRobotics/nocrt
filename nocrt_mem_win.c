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

#ifdef NOCRT_MEM

#include <windows.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "nocrt.h"
#include "nocrt_mem.h"

void *noctr_malloc(size_t size)
{
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void *noctr_realloc(void *ptr, size_t new_size)
{
	return HeapReAlloc(GetProcessHeap(), 0, ptr, new_size);
}

void *noctr_calloc(size_t num, size_t size)
{
	size_t total = num*size; 
	
  return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, total);
}

void noctr_free(void *ptr)
{
	HeapFree(GetProcessHeap(), 0, ptr);
}

#endif /* NOCRT_MEM */

