PLATFORM=/Developer/Platforms/iPhoneOS.platform
SDKVER=3.1.3
SDK=$(PLATFORM)/Developer/SDKs/iPhoneOS$(SDKVER).sdk
CC=$(PLATFORM)/Developer/usr/bin/gcc-4.2
STRIP=$(PLATFORM)/Developer/usr/bin/strip

RESTORED_PWN_SOURCES=main.c log.c framebuffer.c nor.c registry.c 

CFLAGS=-arch armv6 -I./include/ -isysroot $(SDK)
LDFLAGS=-F$(SDK)/System/Library/PrivateFrameworks \
	-framework CoreFoundation \
	-framework IOKit \
	-framework IOMobileFramebuffer \
	-framework CoreGraphics \

PROG=restored_external

all: clean $(PROG)

$(PROG):
	$(CC) $(CFLAGS) $(RESTORED_PWN_SOURCES) -o $(PROG) $(LDFLAGS)
	$(STRIP) $(PROG)
	ldid -S $(PROG)

clean:
	rm -f $(PROG)