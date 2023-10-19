#include <ntifs.h>
#include <intrin.h>

DRIVER_UNLOAD DriverUnload;

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);

typedef struct _MMPTE_HIGHLOW
{
    ULONG LowPart;
    ULONG HighPart;
} MMPTE_HIGHLOW, * PMMPTE_HIGHLOW;

typedef struct _MMPTE_HARDWARE
{
    ULONGLONG Valid : 1;
    ULONGLONG Dirty1 : 1;
    ULONGLONG Owner : 1;
    ULONGLONG WriteThrough : 1;
    ULONGLONG CacheDisable : 1;
    ULONGLONG Accessed : 1;
    ULONGLONG Dirty : 1;
    ULONGLONG LargePage : 1;
    ULONGLONG Global : 1;
    ULONGLONG CopyOnWrite : 1;
    ULONGLONG Unused : 1;
    ULONGLONG Write : 1;
    ULONGLONG PageFrameNumber : 40;
    ULONGLONG ReservedForSoftware : 4;
    ULONGLONG WsleAge : 4;
    ULONGLONG WsleProtection : 3;
    ULONGLONG NoExecute : 1;
} MMPTE_HARDWARE, * PMMPTE_HARDWARE;

typedef struct _MMPTE_PROTOTYPE
{
    ULONGLONG Valid : 1;
    ULONGLONG DemandFillProto : 1;
    ULONGLONG HiberVerifyConverted : 1;
    ULONGLONG ReadOnly : 1;
    ULONGLONG SwizzleBit : 1;
    ULONGLONG Protection : 5;
    ULONGLONG Prototype : 1;
    ULONGLONG Combined : 1;
    ULONGLONG Unused1 : 4;
    LONGLONG ProtoAddress : 48;
} MMPTE_PROTOTYPE, * PMMPTE_PROTOTYPE;

typedef struct _MMPTE_SOFTWARE
{
    ULONGLONG Valid : 1;
    ULONGLONG PageFileReserved : 1;
    ULONGLONG PageFileAllocated : 1;
    ULONGLONG ColdPage : 1;
    ULONGLONG SwizzleBit : 1;
    ULONGLONG Protection : 5;
    ULONGLONG Prototype : 1;
    ULONGLONG Transition : 1;
    ULONGLONG PageFileLow : 4;
    ULONGLONG UsedPageTableEntries : 10;
    ULONGLONG ShadowStack : 1;
    ULONGLONG OnStandbyLookaside : 1;
    ULONGLONG Unused : 4;
    ULONGLONG PageFileHigh : 32;
} MMPTE_SOFTWARE, * PMMPTE_SOFTWARE;

typedef struct _MMPTE_TIMESTAMP
{
    ULONGLONG MustBeZero : 1;
    ULONGLONG Unused : 3;
    ULONGLONG SwizzleBit : 1;
    ULONGLONG Protection : 5;
    ULONGLONG Prototype : 1;
    ULONGLONG Transition : 1;
    ULONGLONG PageFileLow : 4;
    ULONGLONG Reserved : 16;
    ULONGLONG GlobalTimeStamp : 32;
} MMPTE_TIMESTAMP, * PMMPTE_TIMESTAMP;

typedef struct _MMPTE_TRANSITION
{
    ULONGLONG Valid : 1;
    ULONGLONG Write : 1;
    ULONGLONG OnStandbyLookaside : 1;
    ULONGLONG IoTracker : 1;
    ULONGLONG SwizzleBit : 1;
    ULONGLONG Protection : 5;
    ULONGLONG Prototype : 1;
    ULONGLONG Transition : 1;
    ULONGLONG PageFrameNumber : 40;
    ULONGLONG Unused : 12;
} MMPTE_TRANSITION, * PMMPTE_TRANSITION;

typedef struct _MMPTE_SUBSECTION
{
    ULONGLONG Valid : 1;
    ULONGLONG Unused0 : 2;
    ULONGLONG OnStandbyLookaside : 1;
    ULONGLONG SwizzleBit : 1;
    ULONGLONG Protection : 5;
    ULONGLONG Prototype : 1;
    ULONGLONG ColdPage : 1;
    ULONGLONG Unused2 : 3;
    ULONGLONG ExecutePrivilege : 1;
    LONGLONG SubsectionAddress : 48;
} MMPTE_SUBSECTION, * PMMPTE_SUBSECTION;

typedef struct _MMPTE_LIST
{
    ULONGLONG Valid : 1;
    ULONGLONG OneEntry : 1;
    ULONGLONG filler0 : 2;
    ULONGLONG SwizzleBit : 1;
    ULONGLONG Protection : 5;
    ULONGLONG Prototype : 1;
    ULONGLONG Transition : 1;
    ULONGLONG filler1 : 16;
    ULONGLONG NextEntry : 36;
} MMPTE_LIST, * PMMPTE_LIST;

typedef struct _MMPTE
{
    union
    {
        ULONGLONG Long;
        volatile ULONGLONG VolatileLong;
#ifndef _WIN64
        MMPTE_HIGHLOW HighLow;
#endif
        MMPTE_HARDWARE Hard;
        MMPTE_PROTOTYPE Proto;
        MMPTE_SOFTWARE Soft;
        MMPTE_TIMESTAMP TimeStamp;
        MMPTE_TRANSITION Trans;
        MMPTE_SUBSECTION Subsect;
        MMPTE_LIST List;
    } u;
} MMPTE, * PMMPTE;

/*
0: kd> uf nt!MiGetPteAddress
nt!MiGetPteAddress:
fffff806`09e64bf4 48c1e909        shr     rcx,9
fffff806`09e64bf8 48b8f8ffffff7f000000 mov rax,7FFFFFFFF8h
fffff806`09e64c02 4823c8          and     rcx,rax
fffff806`09e64c05 48b8000000008081ffff mov rax,0FFFF818000000000h
fffff806`09e64c0f 4803c1          add     rax,rcx
fffff806`09e64c12 c3              ret
0: kd> !pte 0
                                           VA 0000000000000000
PXE at FFFF81C0E0703000    PPE at FFFF81C0E0600000    PDE at FFFF81C0C0000000    PTE at FFFF818000000000
contains 8A00000101B3E867  contains 0A00000101B3F867  contains 0000000000000000
pfn 101b3e    ---DA--UW-V  pfn 101b3f    ---DA--UWEV  contains 0000000000000000
not valid

0: kd> g
pml4: FFFF81C0E0703000
auto entry index: 0x103
pte_base: FFFF818000000000,end: FFFF820000000000
pde_base: FFFF81C0C0000000,end: FFFF81C100000000
ppe_base: FFFF81C0E0600000,end: FFFF81C0E0800000
pxe_base: FFFF81C0E0703000,end: FFFF81C0E0704000
*/

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = DriverUnload;

    ULONG_PTR pte_base, pde_base, ppe_base, pxe_base;
    pte_base = pde_base = ppe_base = pxe_base = 0;
	PHYSICAL_ADDRESS pml4;
	pml4.QuadPart = __readcr3();
    // get the PXE virtual address
    PMMPTE_HARDWARE page_directory_va
        = (PMMPTE_HARDWARE)MmGetVirtualForPhysical(pml4);
    KdPrint(("pml4: %p\n", page_directory_va));
    pxe_base = (ULONG_PTR)page_directory_va;
    ULONG_PTR pxe_end = pxe_base + (1ull << 12);
    ULONG_PTR auto_entry_index = (pxe_base >> 12) & 0x1ff;
    KdPrint(("auto entry index: 0x%x\n", auto_entry_index));
    ULONG_PTR va = 0xFFFF0000'00000000;
    pte_base = va | (auto_entry_index << 39);
    ULONG_PTR pte_end = pte_base + (1ull << 39);
    pde_base = pte_base | (auto_entry_index << 30);
    ULONG_PTR pde_end = pde_base + (1ull << 30);
    ppe_base = pde_base | (auto_entry_index << 21);
    ULONG_PTR ppe_end = ppe_base + (1ull << 21);

    KdPrint(("pte_base: %p,end: %p\n", pte_base, pte_end));
    KdPrint(("pde_base: %p,end: %p\n", pde_base, pde_end));
    KdPrint(("ppe_base: %p,end: %p\n", ppe_base, ppe_end));
    KdPrint(("pxe_base: %p,end: %p\n", pxe_base, pxe_end));

    
    return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
}