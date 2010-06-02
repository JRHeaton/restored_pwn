#include <IOKit/IOKitLib.h>
#include <CoreGraphics/CGGeometry.h>

kern_return_t IOMobileFramebufferOpen(io_service_t service, task_port_t owningTask, uint32_t type, io_connect_t *connect);
kern_return_t IOMobileFramebufferGetDisplaySize(io_connect_t connect, CGSize *size);