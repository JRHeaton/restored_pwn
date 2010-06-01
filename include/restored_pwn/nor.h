#ifndef RESTORED_PWN_NOR_H
#define RESTORED_PWN_NOR_H

#include <IOKit/IOKitLib.h>

int img3_flash_NOR_image(io_connect_t norServiceConnection, CFDataRef imageData, int isLLB);

#endif /* RESTORED_PWN_NOR_H */