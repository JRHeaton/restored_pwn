#include <restored_pwn/registry.h>
#include <restored_pwn/log.h>
#include <sys/sysctl.h>
#include <sys/mman.h>
#include <errno.h>

io_service_t get_io_service(const char *name) {
	CFMutableDictionaryRef matching;
	io_service_t service;
	
	matching = IOServiceMatching(name);
	if(matching == NULL) {
		restored_log("unable to create matching dictionary for class '%s'", name);
		return 0;
	}
	
	while(!service) {
		CFRetain(matching);
		service = IOServiceGetMatchingService(kIOMasterPortDefault, matching);
		if(service) break;
		
		restored_log("waiting for matching IOKit service: %s\n", name);
		sleep(1);
		CFRelease(matching);
	}
	
	CFRelease(matching);
	
	return service;
}

CFStringRef copy_device_imei() {
	CFMutableDictionaryRef matching;
	io_service_t service;
	CFDataRef imeiData;
	const void *imeiDataPtr;
	CFStringRef imeiString;
	
	matching = IOServiceNameMatching("baseband");
	service = IOServiceGetMatchingService(kIOMasterPortDefault, matching);
	
	if(!service) {
		restored_log("unable to find baseband service\n");
		return NULL;
	}
	
	imeiData = IORegistryEntryCreateCFProperty(service, CFSTR("device-imei"), kCFAllocatorDefault, 0);
	if(!imeiData) {
		restored_log("unable to find device-imei property\n");
		IOObjectRelease(service);
		return NULL;
	}
	
	imeiDataPtr = CFDataGetBytePtr(imeiData);
	imeiString = CFStringCreateWithCString(kCFAllocatorDefault, imeiDataPtr, kCFStringEncodingUTF8);
	
	CFRelease(imeiData);
	IOObjectRelease(service);
	
	return imeiString;
}

CFStringRef copy_device_serial_number() {
	io_service_t service;
	CFStringRef serialNumber;
	
	service = get_io_service("IOPlatformExpertDevice");
	if(!service) {
		restored_log("unable to find IOPlatformExpertDevice service");
		return NULL;
	}
	
	serialNumber = IORegistryEntryCreateCFProperty(service, CFSTR("IOPlatformSerialNumber"), kCFAllocatorDefault, 0);
	IOObjectRelease(service);
	
	return serialNumber;
}

CFStringRef copy_devicetree_option(CFStringRef key) {
	io_registry_entry_t entry;
	CFStringRef option;
	
	entry = IORegistryEntryFromPath(kIOMasterPortDefault, "IODeviceTree:/options");
	if(!entry) {
		restored_log("unable to get registry entry for IODeviceTree:/options");
		return NULL;
	}
	
	option = IORegistryEntryCreateCFProperty(entry, key, kCFAllocatorDefault, 0);
	IOObjectRelease(entry);
	
	return option;
}

CFStringRef copy_hardware_model() {
	size_t buflen = 0x80;
	char buf[buflen];
	CFStringRef model;
	
	if(sysctlbyname("hw.model", buf, &buflen, NULL, 0) != 0) {
		restored_log("sysctlbyname for hw.model failed: %s\n", strerror(errno));
		return NULL;
	}
	
	model = CFStringCreateWithCString(kCFAllocatorDefault, buf, kCFStringEncodingUTF8);
	
	return model;
}

CFStringRef copy_hardware_platform() {
	io_service_t service;
	CFStringRef platform;
	char *platformPtr;
	
	service = get_io_service("IOPlatformExpertDevice");
	if(!service) {
		restored_log("unable to find IOPlatformExpertDevice service");
		return NULL;
	}
	
	platform= IORegistryEntryCreateCFProperty(service, CFSTR("platform-name"), kCFAllocatorDefault, 0);
	if(platform == NULL) {
		restored_log("platform-name not found in device tree");
		IOObjectRelease(service);
		return NULL;
	}
	
	platformPtr = calloc(1, CFStringGetLength(platform)+1);
	if(!CFStringGetCString(platform, platformPtr, CFStringGetLength(platform)+1, kCFStringEncodingUTF8)) {
		restored_log("unable to obtain platform-name string");
		IOObjectRelease(service);
		return NULL;
	}
	
	restored_log("platform-name = %s\n", platformPtr);
	free(platformPtr);
	
	return platform;
}