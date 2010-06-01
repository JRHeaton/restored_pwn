#include <restored_pwn/framebuffer.h>
#include <IOKit/IOKitLib.h>
#include <string.h>

void *framebuffer_open() {
	CFMutableDictionaryRef matching;
	io_iterator_t iterator;
	io_service_t service;
	kern_return_t result;
	io_connect_t connect;
	
	matching = IOServiceMatching("IOMobileFramebuffer");
	if(!IOServiceGetMatchingServices(kIOMasterPortDefault, matching, &iterator)) {
		restored_log("unable to get IOMobileFramebuffer service\n");
		
		// ***FILL IN RELEASE CRAP
		return NULL;
	}
	
	while(service = IOIteratorNext(iterator)) {
		io_name_t class_name;
		if((result = IOObjectGetClass(service, class_name)) != KERN_SUCCESS) {
			restored_log("unable to get name for service: 0x%x\n", result);
			IOObjectRelease(service);
			continue;
		}
		
		if(strstr(class_name, "CLCD") == NULL) {
			IOObjectRelease(service);
			continue;
		}
		
		restored_log("found suitable IOMobileFramebuffer: %s\n", class_name);
		
		if((result = IOMobileFramebufferOpen(service, mach_task_self(), 0, &connect)) != KERN_SUCCESS) {
			restored_log("unable to open framebuffer: 0x%x\n", result);
			// *** OTHER BS PERHAPS
		}
		
		
	}
}