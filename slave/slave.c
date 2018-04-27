#include "ntddk.h"
#include "MYIOCTL.H"

NTSTATUS
DriverEntry( IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath )
{
    return STATUS_SUCCESS;
}

NTSTATUS
_declspec( dllexport )DoSomethingMeaningless(IN PIRP pIrp,
    IN PIO_STACK_LOCATION
    pIrpStack)
{
    USHORT *pw;

    /* check to see if this is our ioctl. */

    if (pIrpStack->Parameters.DeviceIoControl.IoControlCode != IOCTL_CALL_KERNEL_MODE_DLL)
        return STATUS_INVALID_PARAMETER;

    /* check to see if output buffer size is big enough. */

    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(USHORT)) {
        pIrp->IoStatus.Status = STATUS_BUFFER_TOO_SMALL;
        pIrp->IoStatus.Information = 0;
        return STATUS_BUFFER_TOO_SMALL;
    }

    /* return a value back to the user. */

    pIrp->IoStatus.Information = sizeof( USHORT );
    pw = (USHORT *)pIrp->AssociatedIrp.SystemBuffer;
    *pw = 0XAA55;
    pIrp->IoStatus.Status = STATUS_SUCCESS;

    return STATUS_SUCCESS;
}

