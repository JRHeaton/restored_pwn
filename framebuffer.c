#include <restored_pwn/framebuffer.h>
#include <IOMobileFramebuffer/IOMobileFramebuffer.h>
#include <IOKit/IOKitLib.h>
#include <string.h>

void *framebuffer_open() {
	CFMutableDictionaryRef matching;
	io_iterator_t iterator;
	io_service_t service;
	kern_return_t result;
	io_connect_t connect;
	io_name_t fb_name;
	
	matching = IOServiceMatching("IOMobileFramebuffer");
	if(!IOServiceGetMatchingServices(kIOMasterPortDefault, matching, &iterator)) {
		restored_log("unable to get IOMobileFramebuffer service\n");
		
		// ***FILL IN RELEASE CRAP
		return NULL;
	}
	
	while(service = IOIteratorNext(iterator)) {
		if((result = IOObjectGetClass(service, fb_name)) != KERN_SUCCESS) {
			restored_log("unable to get name for service: 0x%x\n", result);
			IOObjectRelease(service);
			continue;
		}
		
		if(strstr(fb_name, "CLCD") == NULL) {
			IOObjectRelease(service);
			continue;
		}
		
		restored_log("found suitable IOMobileFramebuffer: %s\n", fb_name);
		
		if((result = IOMobileFramebufferOpen(service, mach_task_self(), 0, &connect)) != KERN_SUCCESS) {
			restored_log("unable to open framebuffer: 0x%x\n", result);
			// *** OTHER BS PERHAPS
		}
		
		
	}
	
	if(!service) {
		restored_log("unable to get display service\n");
		// ***BAIL OUT
	}
	
	restored_log("found suitable framebuffer: %s\n", fb_name);
	IOObjectRelease(iterator);
	
	if((result = IOMobileFramebufferOpen(service, mach_task_self(), 0, &connect)) != KERN_SUCCESS) {
		restored_log("unable to open framebuffer: 0x%x\n", result);
		// ***BAIL OUT
	}
	
	
}