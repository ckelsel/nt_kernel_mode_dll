#include "windows.h"
#include "winioctl.h"
#include "stdio.h"
#include "stdlib.h"
#include "myioctl.h"

int main ( void )
{
    HANDLE hDriver;
    DWORD cbReturned = 0x0;
    WORD iobuf = 0x0;

    // Try to open the device

    if ((hDriver = CreateFile("\\\.\\MASTER",
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL )) != INVALID_HANDLE_VALUE )

            printf ("\nRetrieved valid handle for MASTER driver\n");

        else
    {
        printf ("Can't get a handle to MASTER driver\n");
        return 0;
    }

    // iobuf initialized to zero, so we better get something different after call.

    if ( DeviceIoControl (hDriver,
                        (DWORD) IOCTL_CALL_KERNEL_MODE_DLL,
                        &iobuf,
                        (DWORD)(2*(sizeof (WORD))),
                        &iobuf,
                        (DWORD)(2*(sizeof (WORD))),
                        &cbReturned,
                        (LPVOID)NULL) )
    {

        if (iobuf == 0xAA55) {
            printf("DeviceIoControl worked and returned the correct value.\n");
        } else {
            printf("DeviceIoControl worked but reurned the wrong value %x\n", iobuf);
        }
    } else {
        printf("DeviceIoControl Failed\n");
    }

    CloseHandle(hDriver);

    return 1;
}

