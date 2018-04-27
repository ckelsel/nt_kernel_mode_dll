#include "ntddk.h"
#include "myioctl.h"

typedef struct _FOO_DEVICE_EXTENSION {
    ULONG Information;
} FOO_DEVICE_EXTENSION, *PFOO_DEVICE_EXTENSION;

// Function declarations

NTSTATUS
_declspec( dllimport )DoSomethingMeaningless(IN PIRP pIrp, IN PIO_STACK_LOCATION pIrpStack );

NTSTATUS
MasterDispatchIoctl( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp )
{
    NTSTATUS                                  status;
    PIO_STACK_LOCATION          irpSp;

    // Init to default settings- we only expect 1 type of
    // IOCTL to roll through here, all others an error.

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    // Get a pointer to the current location in the Irp. This is where
    // the function codes and parameters are located.

    irpSp = IoGetCurrentIrpStackLocation( Irp );

    switch (irpSp->Parameters.DeviceIoControl.IoControlCode) {
        case IOCTL_CALL_KERNEL_MODE_DLL:
            if ( NT_SUCCESS( DoSomethingMeaningless( Irp , irpSp) ) ) {
                status = STATUS_SUCCESS;
            } else {
                status = STATUS_INVALID_PARAMETER;
            }
            break;

        default:
            Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            break;
    }

    // DON'T get cute and try to use the status field of
    // the irp in the return status. That IRP IS GONE as
    // soon as you call IoCompleteRequest.

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    // We never have pending operation so always return the status code.

    return status;

}

NTSTATUS
DriverEntry( IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath )
{
    UNICODE_STRING nameString, linkString;
    PDEVICE_OBJECT deviceObject;
    NTSTATUS status;

    //slave.sys needs to start before master.sys
    // Create the device object.

    RtlInitUnicodeString( &nameString, L"\\Device\\MASTER");
    status = IoCreateDevice( DriverObject,
            sizeof(FOO_DEVICE_EXTENSION),
             &nameString,
            FILE_DEVICE_UNKNOWN,
            0,
            FALSE,
            &deviceObject );

    if (!NT_SUCCESS( status ))
        return status;

    // Create the symbolic link.

    RtlInitUnicodeString( &linkString, L"\\DosDevices\\MASTER");
    status = IoCreateSymbolicLink (&linkString, &nameString);

    if (!NT_SUCCESS( status )) {
        IoDeleteDevice (DriverObject->DeviceObject);
        return status;
    }

    // Initialize the driver object with this device driver's entry points.

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MasterDispatchIoctl;
    return STATUS_SUCCESS;
}

