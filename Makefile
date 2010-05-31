PLATFORM=/Developer/Platforms/iPhoneOS.platform
SDKVER=3.1.3
SDK=$(PLATFORM)/Developer/SDKs/iPhoneOS$(SDKVER).sdk
CC=$(PLATFORM)/Developer/usr/bin/gcc-4.2
STRIP=$(PLATFORM)/Developer/usr/bin/strip

RESTORED_PWN_SOURCES=main.c

CFLAGS=-arch armv6 -isysroot $(SDK) -I/Developer/SDKs/MacOSX10.5.sdk/System/Library/Frameworks/IOKit.framework/Headers
LDFLAGS=-framework CoreFoundation -framework IOKit

PROG=restored_external

all: $(PROG)

$(PROG):
	$(CC) $(CFLAGS) $(RESTORED_PWN_SOURCES) -o $(PROG) $(LDFLAGS)
	$(STRIP) $(PROG)
	ldid -S $(PROG)
