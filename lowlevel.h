#ifndef RESTORED_PWN_LOWLEVEL_H
#define RESTORED_PWN_LOWLEVEL_H

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

CFStringRef copy_device_imei();
CFStringRef copy_device_serial_number();
CFStringRef copy_devicetree_option(CFStringRef key);
CFStringRef copy_hardware_model();
CFStringRef copy_hardware_platform();

#endif /* RESTORED_PWN_LOWLEVEL_H */