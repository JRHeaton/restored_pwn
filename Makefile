PLATFORM=/Developer/Platforms/iPhoneOS.platform
SDKVER=3.1.3
SDK=$(PLATFORM)/Developer/SDKs/iPhoneOS$(SDKVER).sdk
CC=$(PLATFORM)/Developer/usr/bin/gcc-4.2
STRIP=$(PLATFORM)/Developer/usr/bin/strip

RESTORED_PWN_SOURCES=main.c log.c lowlevel.c framebuffer.c

CFLAGS=-arch armv6 -isysroot $(SDK)
LDFLAGS=-framework CoreFoundation -framework IOKit

PROG=restored_external

all: clean $(PROG)

$(PROG):
	$(CC) $(CFLAGS) $(RESTORED_PWN_SOURCES) -o $(PROG) $(LDFLAGS)
	$(STRIP) $(PROG)
	ldid -S $(PROG)

link:
	sudo ln -s /Developer/SDKs/MacOSX10.5.sdk/System/Library/Frameworks/IOKit.framework/Headers $(SDK)/usr/include/IOKit

clean:
	rm -f $(PROG)