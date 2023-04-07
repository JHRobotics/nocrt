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

#ifndef __NOCRT_MEM_H__INCLUDED__
#define __NOCRT_MEM_H__INCLUDED__

#ifdef NOCRT

void *noctr_malloc(size_t size);
void *noctr_realloc(void *ptr, size_t new_size);
void *noctr_calloc(size_t num, size_t size);
void noctr_free(void *ptr);

#define malloc noctr_malloc
#define realloc noctr_realloc
#define calloc noctr_calloc
#define free noctr_free

#endif /* NOCRT */

#endif /* __NOCRT_MEM_H__INCLUDED__ */
