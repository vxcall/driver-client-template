#include "driver_entry.h"
#include <ntddk.h>

VOID driver_unload(PDRIVER_OBJECT driver_object) {
    UNREFERENCED_PARAMETER(driver_object);
}

NTSTATUS driver_create_close(PDEVICE_OBJECT device_object, PIRP irp);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {
    UNREFERENCED_PARAMETER(registry_path);
    driver_object->DriverUnload = driver_unload;
    driver_object->MajorFunction[IRP_MJ_CREATE] = driver_create_close;
    driver_object->MajorFunction[IRP_MJ_CLOSE] = driver_create_close;
    UNICODE_STRING device_name = RTL_CONSTANT_STRING(device_name_w);

    PDEVICE_OBJECT device_object;
    NTSTATUS status = IoCreateDevice(
            driver_object,
            0,
            &device_name,
            FILE_DEVICE_UNKNOWN,
            0,
            FALSE,
            &device_object);

    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to create device object (0x%08X)\n", status));
        return status;
    }

    UNICODE_STRING sym_link = RTL_CONSTANT_STRING(sym_name_w);
    status = IoCreateSymbolicLink(&sym_link, &device_name);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Failed to create symbolic link (0x%08X)\n", status));
        IoDeleteDevice(device_object); // undo every step
        return status;
    }
    return STATUS_SUCCESS;
}

NTSTATUS driver_create_close(PDEVICE_OBJECT device_object, PIRP irp) {
    UNREFERENCED_PARAMETER(device_object);
    irp->IoStatus.Status = STATUS_SUCCESS; // The value this request should be completed with.
    irp->IoStatus.Information = 0;         // polymophic member. different things in different request. Create and Close are fine with 0.
    IoCompleteRequest(irp, IO_NO_INCREMENT); // IO_NO_INCREMENT means zero.
    return STATUS_SUCCESS;
}
