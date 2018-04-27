#ifndef MY_IOCTL_H_
#define MY_IOCTL_H_
//CTL_CODE macro defined in devioctl.h in DDK\inc.
//custom device types (non Microsoft) in range 32768 to 65535
//custom user defined function codes in range 2048 to 4095
#define FILE_DEVICE_FOO 65534
#define IOCTL_CALL_KERNEL_MODE_DLL CTL_CODE(FILE_DEVICE_FOO, 2049, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif // MY_IOCTL_H_

