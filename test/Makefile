CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -O2 -I..
OBJ_SUFFIX = .o
LDFLAGS=-static
LIBS=-luser32 -lkernel32 -Wl,-subsystem,console

DEPS = Makefile

TESTS := hello.exe test1.exe test2.exe test3.exe test4.exe test5.exe test6.exe test7.exe

NOCRT=1

all: $(TESTS)

.PHONY: all clean

SOURCES := 

ifdef NOCRT
	SOURCES += ../nocrt.c ../nocrt_exe.c ../nocrt_file_win.c ../nocrt_mem_win.c ../nocrt_math.c
	LDFLAGS += -nostdlib -nodefaultlibs -lgcc
	CFLAGS  += -DNOCRT -DNOCRT_FILE -DNOCRT_FLOAT -DNOCRT_MEM -ffreestanding -nostdlib
endif

OBJS := $(SOURCES:.c=.c$(OBJ_SUFFIX))

%.c.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.exe: %.c.o $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) $(LDFLAGS) -o $@ $(LIBS)

clean:
	-$(RM) $(OBJS)
	-$(RM) $(TESTS)
