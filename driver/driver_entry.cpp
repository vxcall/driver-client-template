#include "driver_entry.h"
#include <ntddk.h>

VOID driverUnload(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
}

NTSTATUS DriverCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = driverUnload;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverCreateClose;
    UNICODE_STRING deviceName = RTL_CONSTANT_STRING(deviceNameW);

    PDEVICE_OBJECT DeviceObject;
    NTSTATUS status = IoCreateDevice(
            DriverObject,
            0,
            &deviceName,
            FILE_DEVICE_UNKNOWN,
            0,
            FALSE,
            &DeviceObject);

    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to create device object (0x%08X)\n", status));
        return status;
    }
    UNICODE_STRING symLink = RTL_CONSTANT_STRING(symNameW);
    status = IoCreateSymbolicLink(&symLink, &deviceName);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to create symbolic link (0x%08X)\n", status));
        IoDeleteDevice(DeviceObject); // undo every step
        return status;
    }
    return STATUS_SUCCESS;
}

NTSTATUS DriverCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS; // The value this request should be completed with.
    Irp->IoStatus.Information = 0;         // polymophic member. different things in different request. Create and Close are fine with 0.
    IoCompleteRequest(Irp, IO_NO_INCREMENT); // IO_NO_INCREMENT means zero.
    return STATUS_SUCCESS;
}
