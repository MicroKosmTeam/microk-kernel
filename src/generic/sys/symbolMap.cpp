#include <sys/symbol.hpp>
const uint64_t symbolCount = 595;
const Symbol symbols[] = {
   {{0xffffffff80000000}, {"SyscallEntry"}}, 
   {{0xffffffff80000000}, {"syscall_start"}}, 
   {{0xffffffff80000040}, {"SyscallStack"}}, 
   {{0xffffffff80020040}, {"syscall_end"}}, 
   {{0xffffffff80021040}, {"EfiMain"}}, 
   {{0xffffffff80021060}, {"LimineEntry"}}, 
   {{0xffffffff800213d0}, {"__stack_chk_fail"}}, 
   {{0xffffffff80021410}, {"_Z11UnwindStacki"}}, 
   {{0xffffffff800214a0}, {"__ubsan_handle_missing_return"}}, 
   {{0xffffffff800214b0}, {"__ubsan_handle_divrem_overflow"}}, 
   {{0xffffffff800214c0}, {"__ubsan_handle_load_invalid_value"}}, 
   {{0xffffffff800214d0}, {"__ubsan_handle_shift_out_of_bounds"}}, 
   {{0xffffffff800214e0}, {"__ubsan_handle_builtin_unreachable"}}, 
   {{0xffffffff800214f0}, {"__ubsan_handle_mul_overflow"}}, 
   {{0xffffffff80021500}, {"__ubsan_handle_negate_overflow"}}, 
   {{0xffffffff80021510}, {"__ubsan_handle_out_of_bounds"}}, 
   {{0xffffffff80021550}, {"__ubsan_handle_pointer_overflow"}}, 
   {{0xffffffff80021560}, {"__ubsan_handle_add_overflow"}}, 
   {{0xffffffff80021570}, {"__ubsan_handle_sub_overflow"}}, 
   {{0xffffffff80021580}, {"__ubsan_handle_type_mismatch_v1"}}, 
   {{0xffffffff80021640}, {"_ZN4ACPI7GetXSDTEv"}}, 
   {{0xffffffff80021650}, {"_ZN4ACPI4InitEv"}}, 
   {{0xffffffff800216b0}, {"_ZN4ACPI9FindTableEPNS_9SDTHeaderEPc"}}, 
   {{0xffffffff80021720}, {"_ZN10UARTDevice4InitE11SerialPorts"}}, 
   {{0xffffffff80021820}, {"_ZN10UARTDevice5IoctlEmP13__va_list_tag"}}, 
   {{0xffffffff800219d0}, {"_ZN10UARTDevice6PutStrEPKc"}}, 
   {{0xffffffff80021a50}, {"_ZN10UARTDevice7PutCharEc"}}, 
   {{0xffffffff80021aa0}, {"_ZN10UARTDevice7GetCharEv"}}, 
   {{0xffffffff80021ae0}, {"_ZN10UARTDevice15isTransmitEmptyEv"}}, 
   {{0xffffffff80021b00}, {"_ZN10UARTDevice14serialReceivedEv"}}, 
   {{0xffffffff80021b20}, {"_Z8InitInfov"}}, 
   {{0xffffffff80021b40}, {"_Z7GetInfov"}}, 
   {{0xffffffff80021b50}, {"_Z15EarlyKernelInitv"}}, 
   {{0xffffffff80021b70}, {"_Z11PrintBannerv"}}, 
   {{0xffffffff80021c20}, {"_Z11KernelStartv"}}, 
   {{0xffffffff80021d60}, {"_Z8RestInitv"}}, 
   {{0xffffffff80021d90}, {"_ZN7BOOTMEM15BootmemIsActiveEv"}}, 
   {{0xffffffff80021da0}, {"_ZN7BOOTMEM6MallocEm"}}, 
   {{0xffffffff80021dd0}, {"_ZN7BOOTMEM4FreeEPv"}}, 
   {{0xffffffff80021de0}, {"_ZN7BOOTMEM7GetFreeEv"}}, 
   {{0xffffffff80021df0}, {"_ZN7BOOTMEM8GetTotalEv"}}, 
   {{0xffffffff80021df6}, {"_ZN7BOOTMEM17DeactivateBootmemEv"}}, 
   {{0xffffffff80021dfd}, {"_ZN7BOOTMEM6MallocEm.cold"}}, 
   {{0xffffffff80021e10}, {"_ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff80021e20}, {"_ZN4HEAP7GetFreeEv"}}, 
   {{0xffffffff80021e30}, {"_ZN4HEAP8GetTotalEv"}}, 
   {{0xffffffff80021e40}, {"_ZN13HeapSegHeader14CombineForwardEv"}}, 
   {{0xffffffff80021ea0}, {"_ZN13HeapSegHeader15CombineBackwardEv"}}, 
   {{0xffffffff80021ec0}, {"_ZN13HeapSegHeader5SplitEm"}}, 
   {{0xffffffff80021f30}, {"_ZN4HEAP14InitializeHeapEPvm"}}, 
   {{0xffffffff80022000}, {"_ZN4HEAP4FreeEPv"}}, 
   {{0xffffffff800220a0}, {"_ZN4HEAP10ExpandHeapEm"}}, 
   {{0xffffffff80022190}, {"_ZN4HEAP6MallocEm"}}, 
   {{0xffffffff80022240}, {"_ZN8SpinLock6UnlockEv"}}, 
   {{0xffffffff80022250}, {"_ZN8SpinLock8IsLockedEv"}}, 
   {{0xffffffff80022260}, {"_ZN8SpinLock4LockEv"}}, 
   {{0xffffffff800222a0}, {"_GLOBAL__sub_I__ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff800222c0}, {"_Z6memcmpPKvS0_m"}}, 
   {{0xffffffff80022300}, {"_Z6memcpyPvS_m"}}, 
   {{0xffffffff80022330}, {"_Z6Mallocm"}}, 
   {{0xffffffff80022380}, {"_Z4FreePv"}}, 
   {{0xffffffff80022390}, {"_Znwm"}}, 
   {{0xffffffff800223e0}, {"_Znam"}}, 
   {{0xffffffff80022430}, {"_ZdlPv"}}, 
   {{0xffffffff80022440}, {"_ZdlPvm"}}, 
   {{0xffffffff80022450}, {"_ZdaPv"}}, 
   {{0xffffffff80022460}, {"_ZdaPvm"}}, 
   {{0xffffffff80022470}, {"_ZN3MEM10DisplayRamEv"}}, 
   {{0xffffffff800224e0}, {"_ZN3MEM4InitEv"}}, 
   {{0xffffffff800224f0}, {"_Z6memsetPvhm"}}, 
   {{0xffffffff80022530}, {"_ZL12ReservePagesPvm"}}, 
   {{0xffffffff800225b0}, {"_ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022840}, {"_ZN3PMM8FreePageEPv"}}, 
   {{0xffffffff800228c0}, {"_ZN3PMM8LockPageEPv"}}, 
   {{0xffffffff80022940}, {"_ZN3PMM11RequestPageEv"}}, 
   {{0xffffffff80022a10}, {"_ZN3PMM12RequestPagesEm"}}, 
   {{0xffffffff80022b30}, {"_ZN3PMM9FreePagesEPvm"}}, 
   {{0xffffffff80022b80}, {"_ZN3PMM9LockPagesEPvm"}}, 
   {{0xffffffff80022bd0}, {"_ZN3PMM10GetFreeMemEv"}}, 
   {{0xffffffff80022be0}, {"_ZN3PMM10GetUsedMemEv"}}, 
   {{0xffffffff80022bf0}, {"_ZN3PMM14GetReservedMemEv"}}, 
   {{0xffffffff80022c00}, {"_GLOBAL__sub_I__ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022c20}, {"_Z6strlenPKc"}}, 
   {{0xffffffff80022c50}, {"_Z6strcpyPcPKc"}}, 
   {{0xffffffff80022c80}, {"_Z6strcmpPKcS0_"}}, 
   {{0xffffffff80022cd0}, {"_Z8is_delimcPc"}}, 
   {{0xffffffff80022d10}, {"_Z6strtokPcS_"}}, 
   {{0xffffffff80022de0}, {"_Z4itoaPcix"}}, 
   {{0xffffffff80022e80}, {"_Z4atoiPc"}}, 
   {{0xffffffff80022ec0}, {"_ZN3VMM21NewModuleVirtualSpaceEv"}}, 
   {{0xffffffff80023000}, {"_ZN3VMM21NewKernelVirtualSpaceEv"}}, 
   {{0xffffffff80023130}, {"_ZN3VMM7InitVMMEv"}}, 
   {{0xffffffff80023170}, {"_ZN3VMM15NewVirtualSpaceEv"}}, 
   {{0xffffffff80023180}, {"_ZN3VMM16LoadVirtualSpaceEPNS_12VirtualSpaceE"}}, 
   {{0xffffffff80023190}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_"}}, 
   {{0xffffffff800231a0}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_m"}}, 
   {{0xffffffff800231b0}, {"_ZN6MODULE13BufferManagerC1Ev"}}, 
   {{0xffffffff800231b0}, {"_ZN6MODULE13BufferManagerC2Ev"}}, 
   {{0xffffffff800231f0}, {"_ZN6MODULE13BufferManager7AddNodeEPNS_6BufferE"}}, 
   {{0xffffffff80023260}, {"_ZN6MODULE13BufferManager10RemoveNodeEj"}}, 
   {{0xffffffff800232d0}, {"_ZN6MODULE13BufferManager8FindNodeEjPPNS_10BufferNodeEPb"}}, 
   {{0xffffffff80023310}, {"_ZN6MODULE13BufferManager12CreateBufferEjjNS_10BufferTypeEm"}}, 
   {{0xffffffff800233c0}, {"_ZN6MODULE13BufferManager9GetBufferEj"}}, 
   {{0xffffffff800233f0}, {"_ZN6MODULE13BufferManager9MapBufferEjjjPN3VMM12VirtualSpaceEm"}}, 
   {{0xffffffff80023520}, {"_ZN6MODULE13BufferManager12DeleteBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023580}, {"_ZN6MODULE13BufferManager10LockBufferEPNS_6BufferE"}}, 
   {{0xffffffff800235b0}, {"_ZN6MODULE13BufferManager12UnlockBufferEPNS_6BufferE"}}, 
   {{0xffffffff800235d0}, {"_ZN6MODULE14ComposeMessageEPNS_7MessageEjjm"}}, 
   {{0xffffffff80023650}, {"_ZN6MODULE6ModuleC1EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff80023650}, {"_ZN6MODULE6ModuleC2EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff80023660}, {"_ZN6MODULE7ManagerC1Ev"}}, 
   {{0xffffffff80023660}, {"_ZN6MODULE7ManagerC2Ev"}}, 
   {{0xffffffff80023690}, {"_ZN6MODULE7Manager7AddNodeEjj"}}, 
   {{0xffffffff80023700}, {"_ZN6MODULE7Manager10RemoveNodeEjj"}}, 
   {{0xffffffff80023770}, {"_ZN6MODULE7Manager8FindNodeEjjPPNS_10ModuleNodeEPb"}}, 
   {{0xffffffff800237c0}, {"_ZN6MODULE7Manager14RegisterModuleEPN4PROC7ProcessEjj"}}, 
   {{0xffffffff80023890}, {"_ZN6MODULE7Manager9GetModuleEjj"}}, 
   {{0xffffffff800238d0}, {"_ZN6MODULE7Manager9GetModuleEm"}}, 
   {{0xffffffff80023920}, {"_ZN6MODULE7Manager16UnregisterModuleEjj"}}, 
   {{0xffffffff80023930}, {"_ZN6MODULE14SectionManagerC1Ev"}}, 
   {{0xffffffff80023930}, {"_ZN6MODULE14SectionManagerC2Ev"}}, 
   {{0xffffffff80023970}, {"_ZN6MODULE14SectionManager8FindNodeEPKcjjPPNS_11SectionNodeEPb"}}, 
   {{0xffffffff80023a30}, {"_ZN6MODULE14SectionManager7AddNodeEPKcjj"}}, 
   {{0xffffffff80023a90}, {"_ZN6MODULE14SectionManager10RemoveNodeEPKcjj"}}, 
   {{0xffffffff80023ae0}, {"_ZN6MODULE14SectionManager21RegisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023ba0}, {"_ZN6MODULE14SectionManager16GetSectionDriverEPKcPjS3_"}}, 
   {{0xffffffff80023c00}, {"_ZN6MODULE14SectionManager23UnregisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023c50}, {"_ZN4PROC7ProcessD1Ev"}}, 
   {{0xffffffff80023c50}, {"_ZN4PROC7ProcessD2Ev"}}, 
   {{0xffffffff80023ca0}, {"_ZN4PROC7Process9GetThreadEm"}}, 
   {{0xffffffff80023cc0}, {"_ZN4PROC7Process10RequestTIDEv"}}, 
   {{0xffffffff80023cd0}, {"_ZN4PROC7Process13DestroyThreadEPNS_6ThreadE"}}, 
   {{0xffffffff80023ce0}, {"_ZN4PROC7Process14SetThreadStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff80023d10}, {"_ZN4PROC7Process14GetThreadStateEm"}}, 
   {{0xffffffff80023d40}, {"_ZN4PROC7Process13SetMainThreadEm"}}, 
   {{0xffffffff80023d70}, {"_ZN4PROC7Process13GetMainThreadEv"}}, 
   {{0xffffffff80023d80}, {"_ZN4PROC7Process16SetMessageThreadEPv"}}, 
   {{0xffffffff80023d90}, {"_ZN4PROC7Process16GetMessageThreadEv"}}, 
   {{0xffffffff80023da0}, {"_ZN4PROC7Process11SetPriorityEh"}}, 
   {{0xffffffff80023db0}, {"_ZN4PROC7Process15SetProcessStateENS_12ProcessStateE"}}, 
   {{0xffffffff80023df0}, {"_ZN4PROC7Process6GetPIDEv"}}, 
   {{0xffffffff80023e00}, {"_ZN4PROC7Process15GetProcessStateEv"}}, 
   {{0xffffffff80023e10}, {"_ZN4PROC7Process21GetVirtualMemorySpaceEv"}}, 
   {{0xffffffff80023e20}, {"_ZN4PROC7Process14GetHighestFreeEv"}}, 
   {{0xffffffff80023e30}, {"_ZN4PROC7Process14SetHighestFreeEm"}}, 
   {{0xffffffff80023e40}, {"_ZN4PROC6ThreadC1EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80023e40}, {"_ZN4PROC6ThreadC2EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80023fc0}, {"_ZN4PROC7Process12CreateThreadEmm.part.0"}}, 
   {{0xffffffff800240b0}, {"_ZN4PROC7Process12CreateThreadEmm"}}, 
   {{0xffffffff800240d0}, {"_ZN4PROC7ProcessC1ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff800240d0}, {"_ZN4PROC7ProcessC2ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff800241c0}, {"_ZN4PROC6ThreadD1Ev"}}, 
   {{0xffffffff800241c0}, {"_ZN4PROC6ThreadD2Ev"}}, 
   {{0xffffffff800241f0}, {"_ZN4PROC6Thread8SetStateENS_12ProcessStateE"}}, 
   {{0xffffffff80024200}, {"_ZN4PROC6Thread8GetStateEv"}}, 
   {{0xffffffff80024210}, {"_ZN4PROC6Thread6GetTIDEv"}}, 
   {{0xffffffff80024220}, {"_ZN4PROC6Thread14SetInstructionEm"}}, 
   {{0xffffffff80024230}, {"_ZN4PROC6Thread8SetStackEm"}}, 
   {{0xffffffff80024240}, {"_ZN4PROC6Thread12GetStackBaseEv"}}, 
   {{0xffffffff80024250}, {"_ZN4PROC6Thread8GetStackEv"}}, 
   {{0xffffffff80024260}, {"_ZN4PROC6Thread12GetStackSizeEv"}}, 
   {{0xffffffff80024270}, {"_ZN4PROC9Scheduler7AddNodeEPNS_11ProcessNodeEPNS_7ProcessE"}}, 
   {{0xffffffff800242f0}, {"_ZN4PROC9Scheduler12PopFirstNodeEPNS_11ProcessNodeE"}}, 
   {{0xffffffff80024330}, {"_ZN4PROC9Scheduler10RemoveNodeEPNS_11ProcessNodeEm"}}, 
   {{0xffffffff800243b0}, {"_ZN4PROC9Scheduler8FindNodeEPNS_11ProcessNodeEmPS2_Pb"}}, 
   {{0xffffffff80024430}, {"_ZN4PROC9SchedulerC1Ev"}}, 
   {{0xffffffff80024430}, {"_ZN4PROC9SchedulerC2Ev"}}, 
   {{0xffffffff80024520}, {"_ZN4PROC9Scheduler10AddProcessEPNS_7ProcessE"}}, 
   {{0xffffffff80024580}, {"_ZN4PROC9Scheduler10GetProcessEmPNS_11ProcessNodeE"}}, 
   {{0xffffffff800245e0}, {"_ZN4PROC9Scheduler10GetProcessEm"}}, 
   {{0xffffffff80024690}, {"_ZN4PROC9Scheduler15SetProcessStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff800247a0}, {"_ZN4PROC9Scheduler17GetRunningProcessEv"}}, 
   {{0xffffffff800247b0}, {"_ZN4PROC9Scheduler12SwitchToTaskEPNS_7ProcessEm"}}, 
   {{0xffffffff800248d0}, {"_ZN4PROC9Scheduler20RecalculateSchedulerEv"}}, 
   {{0xffffffff80024940}, {"_ZN6BitmapixEm"}}, 
   {{0xffffffff80024970}, {"_ZN6Bitmap3GetEm"}}, 
   {{0xffffffff800249a0}, {"_ZN6Bitmap3SetEmb"}}, 
   {{0xffffffff800249f0}, {"_Z5IoctlP6Drivermz"}}, 
   {{0xffffffff80024a40}, {"_Z18LoadProgramHeadersPhmP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024d70}, {"_Z11LinkSymbolsPhmP10Elf64_Ehdr"}}, 
   {{0xffffffff80024d90}, {"_Z11LoadProcessP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024e40}, {"_Z7LoadELFPhm"}}, 
   {{0xffffffff80024eb0}, {"_ZN4FILE4OpenEPcPm"}}, 
   {{0xffffffff800250b0}, {"__cxa_guard_acquire"}}, 
   {{0xffffffff800250c0}, {"__cxa_guard_release"}}, 
   {{0xffffffff800250d0}, {"__cxa_guard_abort"}}, 
   {{0xffffffff800250e0}, {"__cxa_atexit"}}, 
   {{0xffffffff80025130}, {"__cxa_finalize"}}, 
   {{0xffffffff800251e0}, {"_Z16RequestInterruptj"}}, 
   {{0xffffffff800251f0}, {"_Z18LoadExecutableFilePhm"}}, 
   {{0xffffffff80025200}, {"_Z12TryLockMutexPb"}}, 
   {{0xffffffff80025210}, {"_Z17SpinlockLockMutexPb"}}, 
   {{0xffffffff80025220}, {"_Z11UnlockMutexPb"}}, 
   {{0xffffffff80025230}, {"_Z5PanicPKcS0_S0_j"}}, 
   {{0xffffffff80025260}, {"_Z4OopsPKcS0_S0_j"}}, 
   {{0xffffffff80025290}, {"_ZN6PRINTK6PutStrEPc"}}, 
   {{0xffffffff800252a0}, {"_ZN6PRINTK7PutCharEc"}}, 
   {{0xffffffff800252b0}, {"_ZN6PRINTK7VPrintKEPcP13__va_list_tag"}}, 
   {{0xffffffff800254a0}, {"_ZN6PRINTK6PrintKEPcz"}}, 
   {{0xffffffff800254e0}, {"_ZN6PRINTK9EarlyInitEv"}}, 
   {{0xffffffff80025530}, {"_GLOBAL__sub_I_kernelPort"}}, 
   {{0xffffffff80025540}, {"_Z12LookupSymbolPKc"}}, 
   {{0xffffffff800255b0}, {"_Z12LookupSymbolm"}}, 
   {{0xffffffff80025620}, {"_Z11AddOverridem"}}, 
   {{0xffffffff80025630}, {"_Z13CheckOverridem"}}, 
   {{0xffffffff80025640}, {"_Z11RunOverridem"}}, 
   {{0xffffffff80025650}, {"_Z24HandleSyscallDebugPrintKPKc"}}, 
   {{0xffffffff80025670}, {"_Z26HandleSyscallMemoryGetinfom"}}, 
   {{0xffffffff800256d0}, {"_Z26HandleSyscallMemoryVmallocmmm"}}, 
   {{0xffffffff800257f0}, {"_Z25HandleSyscallMemoryVmfreemm"}}, 
   {{0xffffffff800258f0}, {"_Z23HandleSyscallMemoryMmapmmmm"}}, 
   {{0xffffffff800259c0}, {"_Z24HandleSyscallMemoryUnmapmm"}}, 
   {{0xffffffff80025aa0}, {"_Z21HandleSyscallProcExecmm"}}, 
   {{0xffffffff80025bc0}, {"_Z21HandleSyscallProcForkm"}}, 
   {{0xffffffff80025bd0}, {"_Z23HandleSyscallProcReturnmm"}}, 
   {{0xffffffff80025c40}, {"_Z21HandleSyscallProcExitmm"}}, 
   {{0xffffffff80025c80}, {"_Z21HandleSyscallProcWaitm"}}, 
   {{0xffffffff80025c90}, {"_Z21HandleSyscallProcKillm"}}, 
   {{0xffffffff80025ca0}, {"_Z27HandleSyscallModuleRegistermm"}}, 
   {{0xffffffff80025d10}, {"_Z29HandleSyscallModuleUnregisterv"}}, 
   {{0xffffffff80025d80}, {"_Z31HandleSyscallModuleBufferCreatemmPj"}}, 
   {{0xffffffff80025e30}, {"_Z28HandleSyscallModuleBufferMapmj"}}, 
   {{0xffffffff80025ee0}, {"_Z30HandleSyscallModuleBufferUnmapmj"}}, 
   {{0xffffffff80025ef0}, {"_Z31HandleSyscallModuleBufferDeletej"}}, 
   {{0xffffffff80025f00}, {"_Z33HandleSyscallModuleMessageHandlerm"}}, 
   {{0xffffffff80025f60}, {"_Z30HandleSyscallModuleMessageSendjjPvm"}}, 
   {{0xffffffff800261a0}, {"_Z34HandleSyscallModuleSectionRegisterPKc"}}, 
   {{0xffffffff80026280}, {"_Z29HandleSyscallModuleSectionGetPKcPjS1_"}}, 
   {{0xffffffff80026340}, {"_Z36HandleSyscallModuleSectionUnregisterPKc"}}, 
   {{0xffffffff80026410}, {"_Z21HandleSyscallFileOpenPcPmS0_"}}, 
   {{0xffffffff800264d0}, {"HandleSyscall"}}, 
   {{0xffffffff800267f0}, {"_Z21HandleSyscallFileReadm"}}, 
   {{0xffffffff80026800}, {"_Z22HandleSyscallFileWritem"}}, 
   {{0xffffffff80026810}, {"_Z22HandleSyscallFileClosem"}}, 
   {{0xffffffff80026820}, {"_Z25HandleSyscallKernOverridem"}}, 
   {{0xffffffff80026830}, {"_ZN5TIMER13WaitForCyclesEm"}}, 
   {{0xffffffff80026840}, {"_ZN6x86_646x86CPU4InitEv"}}, 
   {{0xffffffff80026940}, {"_ZN6x86_646x86CPU9GetVendorEPc"}}, 
   {{0xffffffff80026960}, {"_ZN6x86_647TSSInitEm"}}, 
   {{0xffffffff80026a00}, {"_ZN6x86_647LoadGDTEm"}}, 
   {{0xffffffff80026a50}, {"_Z15InitializeStackPvm"}}, 
   {{0xffffffff80026a60}, {"_ZN6x86_648LoadMADTEv"}}, 
   {{0xffffffff80026bb0}, {"_ZN6x86_6411GetAPICBaseEv"}}, 
   {{0xffffffff80026bc0}, {"_ZN6x86_6411SetAPICBaseEm"}}, 
   {{0xffffffff80026be0}, {"_ZN6x86_6417WriteAPICRegisterEtj"}}, 
   {{0xffffffff80026c00}, {"_ZN6x86_6416ReadAPICRegisterEt"}}, 
   {{0xffffffff80026c20}, {"_ZN6x86_648WaitAPICEj"}}, 
   {{0xffffffff80026c40}, {"_ZN6x86_6410EnableAPICEv"}}, 
   {{0xffffffff80026d10}, {"_ZN6x86_6411SendAPICEOIEv"}}, 
   {{0xffffffff80026d30}, {"_ZN6x86_6417WriteHPETRegisterEmjj"}}, 
   {{0xffffffff80026d40}, {"_ZN6x86_6412EnableIOAPICEm"}}, 
   {{0xffffffff80026d80}, {"_ZN6x86_6418ReadIOAPICRegisterEmt"}}, 
   {{0xffffffff80026d90}, {"_ZN6x86_6419WriteIOAPICRegisterEmtm"}}, 
   {{0xffffffff80026da0}, {"_ZN6x86_647IDTInitEv"}}, 
   {{0xffffffff80026ed0}, {"exceptionHandler"}}, 
   {{0xffffffff80027000}, {"timerHandler"}}, 
   {{0xffffffff80027050}, {"spuriousHandler"}}, 
   {{0xffffffff80027080}, {"syscallHandler"}}, 
   {{0xffffffff800270c0}, {"_ZN6x86_644OutBEth"}}, 
   {{0xffffffff800270d0}, {"_ZN6x86_644OutWEtt"}}, 
   {{0xffffffff800270e0}, {"_ZN6x86_643InBEt"}}, 
   {{0xffffffff800270f0}, {"_ZN6x86_643InWEt"}}, 
   {{0xffffffff80027100}, {"_ZN6x86_646IoWaitEv"}}, 
   {{0xffffffff80027110}, {"_ZN6x86_644InitEv"}}, 
   {{0xffffffff800271d0}, {"_ZN14PageMapIndexerC1Em"}}, 
   {{0xffffffff800271d0}, {"_ZN14PageMapIndexerC2Em"}}, 
   {{0xffffffff80027210}, {"_ZN16PageTableManager9MapMemoryEPvS0_m"}}, 
   {{0xffffffff80027570}, {"_ZN16PageTableManager11UnmapMemoryEPv"}}, 
   {{0xffffffff80027660}, {"_ZN16PageTableManager18GetPhysicalAddressEPv"}}, 
   {{0xffffffff80027730}, {"_ZN16PageTableManager4ForkEPN3VMM12VirtualSpaceEb"}}, 
   {{0xffffffff80027b00}, {"_ZN16PageTableManagerC1EP9PageTable"}}, 
   {{0xffffffff80027b00}, {"_ZN16PageTableManagerC2EP9PageTable"}}, 
   {{0xffffffff80027b10}, {"_ZN16PageTableManager13GetTopAddressEv"}}, 
   {{0xffffffff80027b20}, {"_ZN18PageDirectoryEntry7SetFlagE7PT_Flagb"}}, 
   {{0xffffffff80027b50}, {"_ZN18PageDirectoryEntry7GetFlagE7PT_Flag"}}, 
   {{0xffffffff80027b60}, {"_ZN18PageDirectoryEntry10SetAddressEm"}}, 
   {{0xffffffff80027b90}, {"_ZN18PageDirectoryEntry10GetAddressEv"}}, 
   {{0xffffffff80027ba0}, {"_ZN6x86_6415NewVirtualSpaceEv"}}, 
   {{0xffffffff80027be0}, {"_ZN6x86_6416LoadVirtualSpaceEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80027c10}, {"CPUPause"}}, 
   {{0xffffffff80027c20}, {"FlushGDT"}}, 
   {{0xffffffff80027c3a}, {"FlushTSS"}}, 
   {{0xffffffff80027c50}, {"EnableSCE"}}, 
   {{0xffffffff80027c90}, {"EnableSSE"}}, 
   {{0xffffffff80027cab}, {"EnableAVX"}}, 
   {{0xffffffff80027cc0}, {"SwitchStack"}}, 
   {{0xffffffff80027d00}, {"EnterUserspace"}}, 
   {{0xffffffff80027d12}, {"UserStub"}}, 
   {{0xffffffff80027d30}, {"isr_stub_0"}}, 
   {{0xffffffff80027d71}, {"isr_stub_1"}}, 
   {{0xffffffff80027db2}, {"isr_stub_2"}}, 
   {{0xffffffff80027df3}, {"isr_stub_3"}}, 
   {{0xffffffff80027e34}, {"isr_stub_4"}}, 
   {{0xffffffff80027e75}, {"isr_stub_5"}}, 
   {{0xffffffff80027eb6}, {"isr_stub_6"}}, 
   {{0xffffffff80027ef7}, {"isr_stub_7"}}, 
   {{0xffffffff80027f38}, {"isr_stub_8"}}, 
   {{0xffffffff80027f77}, {"isr_stub_9"}}, 
   {{0xffffffff80027fb8}, {"isr_stub_10"}}, 
   {{0xffffffff80027ff7}, {"isr_stub_11"}}, 
   {{0xffffffff80028036}, {"isr_stub_12"}}, 
   {{0xffffffff80028075}, {"isr_stub_13"}}, 
   {{0xffffffff800280b4}, {"isr_stub_14"}}, 
   {{0xffffffff800280f3}, {"isr_stub_15"}}, 
   {{0xffffffff80028134}, {"isr_stub_16"}}, 
   {{0xffffffff80028175}, {"isr_stub_17"}}, 
   {{0xffffffff800281b4}, {"isr_stub_18"}}, 
   {{0xffffffff800281f5}, {"isr_stub_19"}}, 
   {{0xffffffff80028236}, {"isr_stub_20"}}, 
   {{0xffffffff80028277}, {"isr_stub_21"}}, 
   {{0xffffffff800282b8}, {"isr_stub_22"}}, 
   {{0xffffffff800282f9}, {"isr_stub_23"}}, 
   {{0xffffffff8002833a}, {"isr_stub_24"}}, 
   {{0xffffffff8002837b}, {"isr_stub_25"}}, 
   {{0xffffffff800283bc}, {"isr_stub_26"}}, 
   {{0xffffffff800283fd}, {"isr_stub_27"}}, 
   {{0xffffffff8002843e}, {"isr_stub_28"}}, 
   {{0xffffffff8002847f}, {"isr_stub_29"}}, 
   {{0xffffffff800284c0}, {"isr_stub_30"}}, 
   {{0xffffffff800284ff}, {"isr_stub_31"}}, 
   {{0xffffffff80028540}, {"isr_stub_32"}}, 
   {{0xffffffff80028581}, {"isr_stub_33"}}, 
   {{0xffffffff800285b3}, {"isr_stub_34"}}, 
   {{0xffffffff800285e5}, {"isr_stub_35"}}, 
   {{0xffffffff80028617}, {"isr_stub_36"}}, 
   {{0xffffffff80028649}, {"isr_stub_37"}}, 
   {{0xffffffff8002867b}, {"isr_stub_38"}}, 
   {{0xffffffff800286ad}, {"isr_stub_39"}}, 
   {{0xffffffff800286ee}, {"isr_stub_40"}}, 
   {{0xffffffff80028720}, {"isr_stub_41"}}, 
   {{0xffffffff80028752}, {"isr_stub_42"}}, 
   {{0xffffffff80028784}, {"isr_stub_43"}}, 
   {{0xffffffff800287b6}, {"isr_stub_44"}}, 
   {{0xffffffff800287e8}, {"isr_stub_45"}}, 
   {{0xffffffff8002881a}, {"isr_stub_46"}}, 
   {{0xffffffff8002884c}, {"isr_stub_47"}}, 
   {{0xffffffff8002887e}, {"isr_stub_48"}}, 
   {{0xffffffff800288b0}, {"isr_stub_49"}}, 
   {{0xffffffff800288e2}, {"isr_stub_50"}}, 
   {{0xffffffff80028914}, {"isr_stub_51"}}, 
   {{0xffffffff80028946}, {"isr_stub_52"}}, 
   {{0xffffffff80028978}, {"isr_stub_53"}}, 
   {{0xffffffff800289aa}, {"isr_stub_54"}}, 
   {{0xffffffff800289dc}, {"isr_stub_55"}}, 
   {{0xffffffff80028a0e}, {"isr_stub_56"}}, 
   {{0xffffffff80028a40}, {"isr_stub_57"}}, 
   {{0xffffffff80028a72}, {"isr_stub_58"}}, 
   {{0xffffffff80028aa4}, {"isr_stub_59"}}, 
   {{0xffffffff80028ad6}, {"isr_stub_60"}}, 
   {{0xffffffff80028b08}, {"isr_stub_61"}}, 
   {{0xffffffff80028b3a}, {"isr_stub_62"}}, 
   {{0xffffffff80028b6c}, {"isr_stub_63"}}, 
   {{0xffffffff80028b9e}, {"isr_stub_64"}}, 
   {{0xffffffff80028bd0}, {"isr_stub_65"}}, 
   {{0xffffffff80028c02}, {"isr_stub_66"}}, 
   {{0xffffffff80028c34}, {"isr_stub_67"}}, 
   {{0xffffffff80028c66}, {"isr_stub_68"}}, 
   {{0xffffffff80028c98}, {"isr_stub_69"}}, 
   {{0xffffffff80028cca}, {"isr_stub_70"}}, 
   {{0xffffffff80028cfc}, {"isr_stub_71"}}, 
   {{0xffffffff80028d2e}, {"isr_stub_72"}}, 
   {{0xffffffff80028d60}, {"isr_stub_73"}}, 
   {{0xffffffff80028d92}, {"isr_stub_74"}}, 
   {{0xffffffff80028dc4}, {"isr_stub_75"}}, 
   {{0xffffffff80028df6}, {"isr_stub_76"}}, 
   {{0xffffffff80028e28}, {"isr_stub_77"}}, 
   {{0xffffffff80028e5a}, {"isr_stub_78"}}, 
   {{0xffffffff80028e8c}, {"isr_stub_79"}}, 
   {{0xffffffff80028ebe}, {"isr_stub_80"}}, 
   {{0xffffffff80028ef0}, {"isr_stub_81"}}, 
   {{0xffffffff80028f22}, {"isr_stub_82"}}, 
   {{0xffffffff80028f54}, {"isr_stub_83"}}, 
   {{0xffffffff80028f86}, {"isr_stub_84"}}, 
   {{0xffffffff80028fb8}, {"isr_stub_85"}}, 
   {{0xffffffff80028fea}, {"isr_stub_86"}}, 
   {{0xffffffff8002901c}, {"isr_stub_87"}}, 
   {{0xffffffff8002904e}, {"isr_stub_88"}}, 
   {{0xffffffff80029080}, {"isr_stub_89"}}, 
   {{0xffffffff800290b2}, {"isr_stub_90"}}, 
   {{0xffffffff800290e4}, {"isr_stub_91"}}, 
   {{0xffffffff80029116}, {"isr_stub_92"}}, 
   {{0xffffffff80029148}, {"isr_stub_93"}}, 
   {{0xffffffff8002917a}, {"isr_stub_94"}}, 
   {{0xffffffff800291ac}, {"isr_stub_95"}}, 
   {{0xffffffff800291de}, {"isr_stub_96"}}, 
   {{0xffffffff80029210}, {"isr_stub_97"}}, 
   {{0xffffffff80029242}, {"isr_stub_98"}}, 
   {{0xffffffff80029274}, {"isr_stub_99"}}, 
   {{0xffffffff800292a6}, {"isr_stub_100"}}, 
   {{0xffffffff800292d8}, {"isr_stub_101"}}, 
   {{0xffffffff8002930a}, {"isr_stub_102"}}, 
   {{0xffffffff8002933c}, {"isr_stub_103"}}, 
   {{0xffffffff8002936e}, {"isr_stub_104"}}, 
   {{0xffffffff800293a0}, {"isr_stub_105"}}, 
   {{0xffffffff800293d2}, {"isr_stub_106"}}, 
   {{0xffffffff80029404}, {"isr_stub_107"}}, 
   {{0xffffffff80029436}, {"isr_stub_108"}}, 
   {{0xffffffff80029468}, {"isr_stub_109"}}, 
   {{0xffffffff8002949a}, {"isr_stub_110"}}, 
   {{0xffffffff800294cc}, {"isr_stub_111"}}, 
   {{0xffffffff800294fe}, {"isr_stub_112"}}, 
   {{0xffffffff80029530}, {"isr_stub_113"}}, 
   {{0xffffffff80029562}, {"isr_stub_114"}}, 
   {{0xffffffff80029594}, {"isr_stub_115"}}, 
   {{0xffffffff800295c6}, {"isr_stub_116"}}, 
   {{0xffffffff800295f8}, {"isr_stub_117"}}, 
   {{0xffffffff8002962a}, {"isr_stub_118"}}, 
   {{0xffffffff8002965c}, {"isr_stub_119"}}, 
   {{0xffffffff8002968e}, {"isr_stub_120"}}, 
   {{0xffffffff800296c0}, {"isr_stub_121"}}, 
   {{0xffffffff800296f2}, {"isr_stub_122"}}, 
   {{0xffffffff80029724}, {"isr_stub_123"}}, 
   {{0xffffffff80029756}, {"isr_stub_124"}}, 
   {{0xffffffff80029788}, {"isr_stub_125"}}, 
   {{0xffffffff800297ba}, {"isr_stub_126"}}, 
   {{0xffffffff800297ec}, {"isr_stub_127"}}, 
   {{0xffffffff8002981e}, {"isr_stub_128"}}, 
   {{0xffffffff80029850}, {"isr_stub_129"}}, 
   {{0xffffffff80029882}, {"isr_stub_130"}}, 
   {{0xffffffff800298b4}, {"isr_stub_131"}}, 
   {{0xffffffff800298e6}, {"isr_stub_132"}}, 
   {{0xffffffff80029918}, {"isr_stub_133"}}, 
   {{0xffffffff8002994a}, {"isr_stub_134"}}, 
   {{0xffffffff8002997c}, {"isr_stub_135"}}, 
   {{0xffffffff800299ae}, {"isr_stub_136"}}, 
   {{0xffffffff800299e0}, {"isr_stub_137"}}, 
   {{0xffffffff80029a12}, {"isr_stub_138"}}, 
   {{0xffffffff80029a44}, {"isr_stub_139"}}, 
   {{0xffffffff80029a76}, {"isr_stub_140"}}, 
   {{0xffffffff80029aa8}, {"isr_stub_141"}}, 
   {{0xffffffff80029ada}, {"isr_stub_142"}}, 
   {{0xffffffff80029b0c}, {"isr_stub_143"}}, 
   {{0xffffffff80029b3e}, {"isr_stub_144"}}, 
   {{0xffffffff80029b70}, {"isr_stub_145"}}, 
   {{0xffffffff80029ba2}, {"isr_stub_146"}}, 
   {{0xffffffff80029bd4}, {"isr_stub_147"}}, 
   {{0xffffffff80029c06}, {"isr_stub_148"}}, 
   {{0xffffffff80029c38}, {"isr_stub_149"}}, 
   {{0xffffffff80029c6a}, {"isr_stub_150"}}, 
   {{0xffffffff80029c9c}, {"isr_stub_151"}}, 
   {{0xffffffff80029cce}, {"isr_stub_152"}}, 
   {{0xffffffff80029d00}, {"isr_stub_153"}}, 
   {{0xffffffff80029d32}, {"isr_stub_154"}}, 
   {{0xffffffff80029d64}, {"isr_stub_155"}}, 
   {{0xffffffff80029d96}, {"isr_stub_156"}}, 
   {{0xffffffff80029dc8}, {"isr_stub_157"}}, 
   {{0xffffffff80029dfa}, {"isr_stub_158"}}, 
   {{0xffffffff80029e2c}, {"isr_stub_159"}}, 
   {{0xffffffff80029e5e}, {"isr_stub_160"}}, 
   {{0xffffffff80029e90}, {"isr_stub_161"}}, 
   {{0xffffffff80029ec2}, {"isr_stub_162"}}, 
   {{0xffffffff80029ef4}, {"isr_stub_163"}}, 
   {{0xffffffff80029f26}, {"isr_stub_164"}}, 
   {{0xffffffff80029f58}, {"isr_stub_165"}}, 
   {{0xffffffff80029f8a}, {"isr_stub_166"}}, 
   {{0xffffffff80029fbc}, {"isr_stub_167"}}, 
   {{0xffffffff80029fee}, {"isr_stub_168"}}, 
   {{0xffffffff8002a020}, {"isr_stub_169"}}, 
   {{0xffffffff8002a052}, {"isr_stub_170"}}, 
   {{0xffffffff8002a084}, {"isr_stub_171"}}, 
   {{0xffffffff8002a0b6}, {"isr_stub_172"}}, 
   {{0xffffffff8002a0e8}, {"isr_stub_173"}}, 
   {{0xffffffff8002a11a}, {"isr_stub_174"}}, 
   {{0xffffffff8002a14c}, {"isr_stub_175"}}, 
   {{0xffffffff8002a17e}, {"isr_stub_176"}}, 
   {{0xffffffff8002a1b0}, {"isr_stub_177"}}, 
   {{0xffffffff8002a1e2}, {"isr_stub_178"}}, 
   {{0xffffffff8002a214}, {"isr_stub_179"}}, 
   {{0xffffffff8002a246}, {"isr_stub_180"}}, 
   {{0xffffffff8002a278}, {"isr_stub_181"}}, 
   {{0xffffffff8002a2aa}, {"isr_stub_182"}}, 
   {{0xffffffff8002a2dc}, {"isr_stub_183"}}, 
   {{0xffffffff8002a30e}, {"isr_stub_184"}}, 
   {{0xffffffff8002a340}, {"isr_stub_185"}}, 
   {{0xffffffff8002a372}, {"isr_stub_186"}}, 
   {{0xffffffff8002a3a4}, {"isr_stub_187"}}, 
   {{0xffffffff8002a3d6}, {"isr_stub_188"}}, 
   {{0xffffffff8002a408}, {"isr_stub_189"}}, 
   {{0xffffffff8002a43a}, {"isr_stub_190"}}, 
   {{0xffffffff8002a46c}, {"isr_stub_191"}}, 
   {{0xffffffff8002a49e}, {"isr_stub_192"}}, 
   {{0xffffffff8002a4d0}, {"isr_stub_193"}}, 
   {{0xffffffff8002a502}, {"isr_stub_194"}}, 
   {{0xffffffff8002a534}, {"isr_stub_195"}}, 
   {{0xffffffff8002a566}, {"isr_stub_196"}}, 
   {{0xffffffff8002a598}, {"isr_stub_197"}}, 
   {{0xffffffff8002a5ca}, {"isr_stub_198"}}, 
   {{0xffffffff8002a5fc}, {"isr_stub_199"}}, 
   {{0xffffffff8002a62e}, {"isr_stub_200"}}, 
   {{0xffffffff8002a660}, {"isr_stub_201"}}, 
   {{0xffffffff8002a692}, {"isr_stub_202"}}, 
   {{0xffffffff8002a6c4}, {"isr_stub_203"}}, 
   {{0xffffffff8002a6f6}, {"isr_stub_204"}}, 
   {{0xffffffff8002a728}, {"isr_stub_205"}}, 
   {{0xffffffff8002a75a}, {"isr_stub_206"}}, 
   {{0xffffffff8002a78c}, {"isr_stub_207"}}, 
   {{0xffffffff8002a7be}, {"isr_stub_208"}}, 
   {{0xffffffff8002a7f0}, {"isr_stub_209"}}, 
   {{0xffffffff8002a822}, {"isr_stub_210"}}, 
   {{0xffffffff8002a854}, {"isr_stub_211"}}, 
   {{0xffffffff8002a886}, {"isr_stub_212"}}, 
   {{0xffffffff8002a8b8}, {"isr_stub_213"}}, 
   {{0xffffffff8002a8ea}, {"isr_stub_214"}}, 
   {{0xffffffff8002a91c}, {"isr_stub_215"}}, 
   {{0xffffffff8002a94e}, {"isr_stub_216"}}, 
   {{0xffffffff8002a980}, {"isr_stub_217"}}, 
   {{0xffffffff8002a9b2}, {"isr_stub_218"}}, 
   {{0xffffffff8002a9e4}, {"isr_stub_219"}}, 
   {{0xffffffff8002aa16}, {"isr_stub_220"}}, 
   {{0xffffffff8002aa48}, {"isr_stub_221"}}, 
   {{0xffffffff8002aa7a}, {"isr_stub_222"}}, 
   {{0xffffffff8002aaac}, {"isr_stub_223"}}, 
   {{0xffffffff8002aade}, {"isr_stub_224"}}, 
   {{0xffffffff8002ab10}, {"isr_stub_225"}}, 
   {{0xffffffff8002ab42}, {"isr_stub_226"}}, 
   {{0xffffffff8002ab74}, {"isr_stub_227"}}, 
   {{0xffffffff8002aba6}, {"isr_stub_228"}}, 
   {{0xffffffff8002abd8}, {"isr_stub_229"}}, 
   {{0xffffffff8002ac0a}, {"isr_stub_230"}}, 
   {{0xffffffff8002ac3c}, {"isr_stub_231"}}, 
   {{0xffffffff8002ac6e}, {"isr_stub_232"}}, 
   {{0xffffffff8002aca0}, {"isr_stub_233"}}, 
   {{0xffffffff8002acd2}, {"isr_stub_234"}}, 
   {{0xffffffff8002ad04}, {"isr_stub_235"}}, 
   {{0xffffffff8002ad36}, {"isr_stub_236"}}, 
   {{0xffffffff8002ad68}, {"isr_stub_237"}}, 
   {{0xffffffff8002ad9a}, {"isr_stub_238"}}, 
   {{0xffffffff8002adcc}, {"isr_stub_239"}}, 
   {{0xffffffff8002adfe}, {"isr_stub_240"}}, 
   {{0xffffffff8002ae30}, {"isr_stub_241"}}, 
   {{0xffffffff8002ae62}, {"isr_stub_242"}}, 
   {{0xffffffff8002ae94}, {"isr_stub_243"}}, 
   {{0xffffffff8002aec6}, {"isr_stub_244"}}, 
   {{0xffffffff8002aef8}, {"isr_stub_245"}}, 
   {{0xffffffff8002af2a}, {"isr_stub_246"}}, 
   {{0xffffffff8002af5c}, {"isr_stub_247"}}, 
   {{0xffffffff8002af8e}, {"isr_stub_248"}}, 
   {{0xffffffff8002afc0}, {"isr_stub_249"}}, 
   {{0xffffffff8002aff2}, {"isr_stub_250"}}, 
   {{0xffffffff8002b024}, {"isr_stub_251"}}, 
   {{0xffffffff8002b056}, {"isr_stub_252"}}, 
   {{0xffffffff8002b088}, {"isr_stub_253"}}, 
   {{0xffffffff8002b0ba}, {"isr_stub_254"}}, 
   {{0xffffffff8002b0fe}, {"isr_stub_255"}}, 
   {{0xffffffff8002b130}, {"isrStubTable"}}, 
   {{0xffffffff8002ccc8}, {"_ZTV10UARTDevice"}}, 
   {{0xffffffff8002cf88}, {"_ZTV8SpinLock"}}, 
   {{0xffffffff8002d060}, {"_ZL14memTypeStrings"}}, 
   {{0xffffffff800306e0}, {"symbols"}}, 
   {{0xffffffff80032c10}, {"symbolCount"}}, 
   {{0xffffffff800331a8}, {"_ZTV16PageTableManager"}}, 
   {{0xffffffff80035000}, {"_ZL18framebufferRequest"}}, 
   {{0xffffffff80035040}, {"_ZL11rsdpRequest"}}, 
   {{0xffffffff80035080}, {"_ZL12kaddrRequest"}}, 
   {{0xffffffff800350c0}, {"_ZL11mMapRequest"}}, 
   {{0xffffffff80035100}, {"_ZL11hhdmRequest"}}, 
   {{0xffffffff80035140}, {"_ZL13moduleRequest"}}, 
   {{0xffffffff80035180}, {"_ZL12stackRequest"}}, 
   {{0xffffffff800351c0}, {"_ZL17bootloaderRequest"}}, 
   {{0xffffffff80035200}, {"_ZL11timeRequest"}}, 
   {{0xffffffff80035240}, {"_ZL17entryPointRequest"}}, 
   {{0xffffffff80035278}, {"__stack_chk_guard"}}, 
   {{0xffffffff80035280}, {"TypeCheckKinds"}}, 
   {{0xffffffff800352d0}, {"bootmemStatus"}}, 
   {{0xffffffff800352d8}, {"_ZN4PROCL10CurrentPIDE"}}, 
   {{0xffffffff800352e0}, {"StartSyscallStack"}}, 
   {{0xffffffff80036000}, {"gdt"}}, 
   {{0xffffffff80037000}, {"xsdt"}}, 
   {{0xffffffff80037008}, {"info"}}, 
   {{0xffffffff80037010}, {"RestStack"}}, 
   {{0xffffffff80038010}, {"lastPosition"}}, 
   {{0xffffffff80038020}, {"bootmemMemory"}}, 
   {{0xffffffff80039020}, {"_ZL8HeapLock"}}, 
   {{0xffffffff80039030}, {"_ZL8totalMem"}}, 
   {{0xffffffff80039038}, {"_ZL7freeMem"}}, 
   {{0xffffffff80039040}, {"_ZL12isHeapActive"}}, 
   {{0xffffffff80039048}, {"_ZL10lastHeader"}}, 
   {{0xffffffff80039050}, {"_ZL7heapEnd"}}, 
   {{0xffffffff80039058}, {"_ZL9heapStart"}}, 
   {{0xffffffff80039060}, {"_ZL10BitmapLock"}}, 
   {{0xffffffff80039070}, {"_ZL17page_bitmap_index"}}, 
   {{0xffffffff80039080}, {"_ZL11page_bitmap"}}, 
   {{0xffffffff80039090}, {"_ZL11initialized"}}, 
   {{0xffffffff80039098}, {"_ZL11used_memory"}}, 
   {{0xffffffff800390a0}, {"_ZL15reserved_memory"}}, 
   {{0xffffffff800390a8}, {"_ZL11free_memory"}}, 
   {{0xffffffff800390b0}, {"_ZZ6strtokPcS_E1p"}}, 
   {{0xffffffff800390c0}, {"__dso_handle"}}, 
   {{0xffffffff800390c8}, {"__atexit_func_count"}}, 
   {{0xffffffff800390e0}, {"__atexit_funcs"}}, 
   {{0xffffffff80039ce0}, {"kernelPort"}}, 
   {{0xffffffff8003a000}, {"gdtPointer"}}, 
   {{0xffffffff8003b000}, {"tss"}}, 
   {{0xffffffff8003b070}, {"idtr"}}, 
   {{0xffffffff8003b080}, {"idt"}}, 
};
