#include <restored_pwn/nor.h>
#include <restored_pwn/log.h>
#include <sys/mman.h>
#include <sys/errno.h>
#include <string.h>

int img3_flash_NOR_image(io_connect_t norServiceConnection, CFDataRef imageData, int isLLB) {
	restored_log("%s: flashing %s data\n", "img3_flash_NOR_image", (isLLB ? "LLB" : "NOR"));
	
	size_t imgLen = CFDataGetLength(imageData);
	void *mappedImage = mmap(NULL, imgLen, PROT_READ | PROT_WRITE, MAP_ANON | VM_FLAGS_PURGABLE, -1, 0);
	if(mappedImage == MAP_FAILED) {
		int err = errno;
		restored_log("mmap (size = %d) failed: %s\n", imgLen, strerror(err));
		return err;
	}
	
	const void *imageDataPtr = CFDataGetBytePtr(imageData);
	bcopy(imageDataPtr, mappedImage, imgLen);
	
	kern_return_t result;
	if((result = IOConnectCallStructMethod(norServiceConnection, 0, mappedImage, imgLen, NULL, 0)) != KERN_SUCCESS) {
		restored_log("IOConnectCallStructMethod failed: 0x%x\n", result);
	}
	
	munmap(mappedImage, imgLen);
	
	return result;
}