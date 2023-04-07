# NOCRT - simple replacement of some parts of C runtime library

This code was created because sometimes C runtime is broken, outdated, missing or is too large and occupied too much valuable space in device ROM. Some of function are bit complex (for example `printf` and all its variants), so I decided to copy out my individual implementations of same function to separate library. Usage is now limited to Windows and GCC (MinGW), other platforms (AVR, Watcom) may come later.

**This is NOT complete reimplementation C runtime! Only small subset of frequently used functions. **


## Usage

Place nocrt.h AFTER standard headers:

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nocrt.h>

....
```

By using define (or better `-D` on command line), set subset of functions which do you want to use:
- `NOCRT`: basic subset (platform independent)
- `NOCRT_FLOAT`: working with floats (for example `%f` in printf), require some assembly (for X86 in `nocrt_math.c`)
- `NOCRT_MEM`: malloc and similar functions, depends on platform (for WINAPI is in `nocrt_mem_win.c`)
- `NOCRT_FILE`: working with files (and standard input/output), depends on platform (for WINAPI is in `nocrt_file_win.c`)

Add `-ffreestanding` to *CFLAGS*

Add `-nostdlib` `-nodefaultlibs` `-lgcc` to *LDFLAGS*

After all, you still need some startup code, for Windows EXE is in `nocrt_exe.c` and for Windows DLL is in `nocrt_dll.c`

## Hello World for WIN32

Console applications, `hello.c`:

```
#include <stdio.h>
#include <stdlib.h>

#include "nocrt.h"

int main()
{
	printf("Hello World!");
	
	return 0;
}
```

Compile files as:

```
gcc -std=c99 -Wall -Wextra -O2 -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib -c -o nocrt.c.o nocrt.c
gcc -std=c99 -Wall -Wextra -O2 -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib -c -o nocrt_math.c.o nocrt_math.c
gcc -std=c99 -Wall -Wextra -O2 -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib -c -o nocrt_file_win.c.o nocrt_file_win.c
gcc -std=c99 -Wall -Wextra -O2 -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib -c -o nocrt_mem_win.c.o nocrt_mem_win.c
gcc -std=c99 -Wall -Wextra -O2 -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib -c -o nocrt_exe.c.o nocrt_exe.c
gcc -std=c99 -Wall -Wextra -O2 -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib \
  hello.c.o nocrt.c.o nocrt_exe.c.o nocrt_file_win.c.o nocrt_mem_win.c.o nocrt_math.c.o \
  -static -nostdlib -nodefaultlibs -lgcc -o hello.exe -luser32 -lkernel32 -Wl,-subsystem,console
```

Windows application (without console), `hello_win.c`:

```
#include <windows.h>

#include "nocrt.h"

int main()
{
	MessageBox(NULL, "Hello World!", "NOCRT example", MB_ICONINFORMATION);
	return 0;
}
```

Compile is simitar but link as:
```
gcc -std=c99 -Wall -Wextra -O2 -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib \
  hello_win.c.o nocrt.c.o nocrt_exe.c.o nocrt_file_win.c.o nocrt_mem_win.c.o nocrt_math.c.o \
  -static -nostdlib -nodefaultlibs -lgcc -o hello.exe -luser32 -lkernel32 -Wl,-subsystem,windows
```

## Binary replacement

This project can't replace `libmsvcrt.a` from MinGW. If need write or port C++ program for Windows 9x (95 + 98 + Me) using newer MinGW, you've got 2 options:
1) Use my fixed `libpthread.a`: https://github.com/JHRobotics/pthread9x (missing parts of old MSVCRT are included)
   You still need some `msvcrt.dll` (included with Internet Explorer 4), but your program should now run on Vanila 98 and Me. On 95 you must install IE or separated Visual C redistributable, anyway is still possible, that your program still won't work on 95, because in newer MinGW are some SSE instructions hardcoded in libraries. This can be solved with the force (https://github.com/JHRobotics/simd95) or using older MinGW (I'm using 4.8.5).
2) Replace msvcrt.dll to one from Visual Studio Redistributable 7.0 (7.0, not 7.1!), it still works on 9x and is full compatible with `libmsvcrt.a` from MinGW.

## Licence

All code in this repository is under Public Domain. You are free to include it to your project if you find it useful!
