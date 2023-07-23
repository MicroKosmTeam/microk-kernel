#include <sys/symbol.hpp>
const uint64_t symbolCount = 598;
const Symbol symbols[] = {
   {{0xffffffff80000000}, {"SyscallEntry"}}, 
   {{0xffffffff80000000}, {"syscall_start"}}, 
   {{0xffffffff80000050}, {"SyscallStack"}}, 
   {{0xffffffff80020050}, {"syscall_end"}}, 
   {{0xffffffff80021050}, {"EfiMain"}}, 
   {{0xffffffff80021070}, {"LimineEntry"}}, 
   {{0xffffffff80021400}, {"__stack_chk_fail"}}, 
   {{0xffffffff80021440}, {"_Z11UnwindStacki"}}, 
   {{0xffffffff800214d0}, {"__ubsan_handle_missing_return"}}, 
   {{0xffffffff800214e0}, {"__ubsan_handle_divrem_overflow"}}, 
   {{0xffffffff800214f0}, {"__ubsan_handle_load_invalid_value"}}, 
   {{0xffffffff80021500}, {"__ubsan_handle_shift_out_of_bounds"}}, 
   {{0xffffffff80021510}, {"__ubsan_handle_builtin_unreachable"}}, 
   {{0xffffffff80021520}, {"__ubsan_handle_mul_overflow"}}, 
   {{0xffffffff80021530}, {"__ubsan_handle_negate_overflow"}}, 
   {{0xffffffff80021540}, {"__ubsan_handle_out_of_bounds"}}, 
   {{0xffffffff80021580}, {"__ubsan_handle_pointer_overflow"}}, 
   {{0xffffffff80021590}, {"__ubsan_handle_add_overflow"}}, 
   {{0xffffffff800215a0}, {"__ubsan_handle_sub_overflow"}}, 
   {{0xffffffff800215b0}, {"__ubsan_handle_type_mismatch_v1"}}, 
   {{0xffffffff80021670}, {"_ZN4ACPI7GetXSDTEv"}}, 
   {{0xffffffff80021680}, {"_ZN4ACPI4InitEv"}}, 
   {{0xffffffff800216e0}, {"_ZN4ACPI9FindTableEPNS_9SDTHeaderEPc"}}, 
   {{0xffffffff80021750}, {"_ZN10UARTDevice4InitE11SerialPorts"}}, 
   {{0xffffffff80021850}, {"_ZN10UARTDevice5IoctlEmP13__va_list_tag"}}, 
   {{0xffffffff80021a00}, {"_ZN10UARTDevice6PutStrEPKc"}}, 
   {{0xffffffff80021a80}, {"_ZN10UARTDevice7PutCharEc"}}, 
   {{0xffffffff80021ad0}, {"_ZN10UARTDevice7GetCharEv"}}, 
   {{0xffffffff80021b10}, {"_ZN10UARTDevice15isTransmitEmptyEv"}}, 
   {{0xffffffff80021b30}, {"_ZN10UARTDevice14serialReceivedEv"}}, 
   {{0xffffffff80021b50}, {"_Z8InitInfov"}}, 
   {{0xffffffff80021b70}, {"_Z7GetInfov"}}, 
   {{0xffffffff80021b80}, {"_Z15EarlyKernelInitv"}}, 
   {{0xffffffff80021ba0}, {"_Z11PrintBannerv"}}, 
   {{0xffffffff80021c50}, {"_Z11KernelStartv"}}, 
   {{0xffffffff80021da0}, {"_Z8RestInitv"}}, 
   {{0xffffffff80021dd0}, {"_ZN7BOOTMEM15BootmemIsActiveEv"}}, 
   {{0xffffffff80021de0}, {"_ZN7BOOTMEM6MallocEm"}}, 
   {{0xffffffff80021e10}, {"_ZN7BOOTMEM4FreeEPv"}}, 
   {{0xffffffff80021e20}, {"_ZN7BOOTMEM7GetFreeEv"}}, 
   {{0xffffffff80021e30}, {"_ZN7BOOTMEM8GetTotalEv"}}, 
   {{0xffffffff80021e36}, {"_ZN7BOOTMEM17DeactivateBootmemEv"}}, 
   {{0xffffffff80021e3d}, {"_ZN7BOOTMEM6MallocEm.cold"}}, 
   {{0xffffffff80021e50}, {"_ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff80021e60}, {"_ZN4HEAP7GetFreeEv"}}, 
   {{0xffffffff80021e70}, {"_ZN4HEAP8GetTotalEv"}}, 
   {{0xffffffff80021e80}, {"_ZN13HeapSegHeader14CombineForwardEv"}}, 
   {{0xffffffff80021ee0}, {"_ZN13HeapSegHeader15CombineBackwardEv"}}, 
   {{0xffffffff80021f00}, {"_ZN13HeapSegHeader5SplitEm"}}, 
   {{0xffffffff80021f70}, {"_ZN4HEAP14InitializeHeapEPvm"}}, 
   {{0xffffffff80022040}, {"_ZN4HEAP4FreeEPv"}}, 
   {{0xffffffff800220e0}, {"_ZN4HEAP10ExpandHeapEm"}}, 
   {{0xffffffff800221d0}, {"_ZN4HEAP6MallocEm"}}, 
   {{0xffffffff80022280}, {"_ZN8SpinLock6UnlockEv"}}, 
   {{0xffffffff80022290}, {"_ZN8SpinLock8IsLockedEv"}}, 
   {{0xffffffff800222a0}, {"_ZN8SpinLock4LockEv"}}, 
   {{0xffffffff800222e0}, {"_GLOBAL__sub_I__ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff80022300}, {"_Z6memcmpPKvS0_m"}}, 
   {{0xffffffff80022340}, {"_Z6memcpyPvS_m"}}, 
   {{0xffffffff80022370}, {"_Z6Mallocm"}}, 
   {{0xffffffff800223c0}, {"_Z4FreePv"}}, 
   {{0xffffffff800223d0}, {"_Znwm"}}, 
   {{0xffffffff80022420}, {"_Znam"}}, 
   {{0xffffffff80022470}, {"_ZdlPv"}}, 
   {{0xffffffff80022480}, {"_ZdlPvm"}}, 
   {{0xffffffff80022490}, {"_ZdaPv"}}, 
   {{0xffffffff800224a0}, {"_ZdaPvm"}}, 
   {{0xffffffff800224b0}, {"_ZN3MEM10DisplayRamEv"}}, 
   {{0xffffffff80022530}, {"_ZN3MEM4InitEv"}}, 
   {{0xffffffff80022540}, {"_Z6memsetPvhm"}}, 
   {{0xffffffff80022580}, {"_ZL12ReservePagesPvm"}}, 
   {{0xffffffff80022600}, {"_ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022890}, {"_ZN3PMM8FreePageEPv"}}, 
   {{0xffffffff80022910}, {"_ZN3PMM8LockPageEPv"}}, 
   {{0xffffffff80022990}, {"_ZN3PMM11RequestPageEv"}}, 
   {{0xffffffff80022a60}, {"_ZN3PMM12RequestPagesEm"}}, 
   {{0xffffffff80022b80}, {"_ZN3PMM9FreePagesEPvm"}}, 
   {{0xffffffff80022bd0}, {"_ZN3PMM9LockPagesEPvm"}}, 
   {{0xffffffff80022c20}, {"_ZN3PMM10GetFreeMemEv"}}, 
   {{0xffffffff80022c30}, {"_ZN3PMM10GetUsedMemEv"}}, 
   {{0xffffffff80022c40}, {"_ZN3PMM14GetReservedMemEv"}}, 
   {{0xffffffff80022c50}, {"_GLOBAL__sub_I__ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022c70}, {"_Z6strlenPKc"}}, 
   {{0xffffffff80022ca0}, {"_Z6strcpyPcPKc"}}, 
   {{0xffffffff80022cd0}, {"_Z6strcmpPKcS0_"}}, 
   {{0xffffffff80022d20}, {"_Z8is_delimcPc"}}, 
   {{0xffffffff80022d60}, {"_Z6strtokPcS_"}}, 
   {{0xffffffff80022e30}, {"_Z4itoaPcix"}}, 
   {{0xffffffff80022ed0}, {"_Z4atoiPc"}}, 
   {{0xffffffff80022f10}, {"_ZN3VMM21NewModuleVirtualSpaceEv"}}, 
   {{0xffffffff800230c0}, {"_ZN3VMM21NewKernelVirtualSpaceEv"}}, 
   {{0xffffffff800231f0}, {"_ZN3VMM7InitVMMEv"}}, 
   {{0xffffffff80023230}, {"_ZN3VMM15NewVirtualSpaceEv"}}, 
   {{0xffffffff80023240}, {"_ZN3VMM16LoadVirtualSpaceEPNS_12VirtualSpaceE"}}, 
   {{0xffffffff80023250}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_"}}, 
   {{0xffffffff80023260}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_m"}}, 
   {{0xffffffff80023270}, {"_ZN6MODULE13BufferManagerC1Ev"}}, 
   {{0xffffffff80023270}, {"_ZN6MODULE13BufferManagerC2Ev"}}, 
   {{0xffffffff800232b0}, {"_ZN6MODULE13BufferManager7AddNodeEPNS_6BufferE"}}, 
   {{0xffffffff80023320}, {"_ZN6MODULE13BufferManager10RemoveNodeEj"}}, 
   {{0xffffffff80023390}, {"_ZN6MODULE13BufferManager8FindNodeEjPPNS_10BufferNodeEPb"}}, 
   {{0xffffffff800233d0}, {"_ZN6MODULE13BufferManager12CreateBufferEjjNS_10BufferTypeEm"}}, 
   {{0xffffffff80023480}, {"_ZN6MODULE13BufferManager9GetBufferEj"}}, 
   {{0xffffffff800234b0}, {"_ZN6MODULE13BufferManager9MapBufferEjjjPN3VMM12VirtualSpaceEm"}}, 
   {{0xffffffff800235e0}, {"_ZN6MODULE13BufferManager12DeleteBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023640}, {"_ZN6MODULE13BufferManager10LockBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023670}, {"_ZN6MODULE13BufferManager12UnlockBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023690}, {"_ZN6MODULE14ComposeMessageEPNS_7MessageEjjm"}}, 
   {{0xffffffff80023710}, {"_ZN6MODULE6ModuleC1EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff80023710}, {"_ZN6MODULE6ModuleC2EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff80023720}, {"_ZN6MODULE7ManagerC1Ev"}}, 
   {{0xffffffff80023720}, {"_ZN6MODULE7ManagerC2Ev"}}, 
   {{0xffffffff80023750}, {"_ZN6MODULE7Manager7AddNodeEjj"}}, 
   {{0xffffffff800237c0}, {"_ZN6MODULE7Manager10RemoveNodeEjj"}}, 
   {{0xffffffff80023830}, {"_ZN6MODULE7Manager8FindNodeEjjPPNS_10ModuleNodeEPb"}}, 
   {{0xffffffff80023880}, {"_ZN6MODULE7Manager14RegisterModuleEPN4PROC7ProcessEjj"}}, 
   {{0xffffffff80023950}, {"_ZN6MODULE7Manager9GetModuleEjj"}}, 
   {{0xffffffff80023990}, {"_ZN6MODULE7Manager9GetModuleEm"}}, 
   {{0xffffffff800239e0}, {"_ZN6MODULE7Manager16UnregisterModuleEjj"}}, 
   {{0xffffffff800239f0}, {"_ZN6MODULE14SectionManagerC1Ev"}}, 
   {{0xffffffff800239f0}, {"_ZN6MODULE14SectionManagerC2Ev"}}, 
   {{0xffffffff80023a30}, {"_ZN6MODULE14SectionManager8FindNodeEPKcjjPPNS_11SectionNodeEPb"}}, 
   {{0xffffffff80023af0}, {"_ZN6MODULE14SectionManager7AddNodeEPKcjj"}}, 
   {{0xffffffff80023b50}, {"_ZN6MODULE14SectionManager10RemoveNodeEPKcjj"}}, 
   {{0xffffffff80023ba0}, {"_ZN6MODULE14SectionManager21RegisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023c60}, {"_ZN6MODULE14SectionManager16GetSectionDriverEPKcPjS3_"}}, 
   {{0xffffffff80023cc0}, {"_ZN6MODULE14SectionManager23UnregisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023d10}, {"_ZN4PROC7ProcessD1Ev"}}, 
   {{0xffffffff80023d10}, {"_ZN4PROC7ProcessD2Ev"}}, 
   {{0xffffffff80023d60}, {"_ZN4PROC7Process9GetThreadEm"}}, 
   {{0xffffffff80023d80}, {"_ZN4PROC7Process10RequestTIDEv"}}, 
   {{0xffffffff80023d90}, {"_ZN4PROC7Process13DestroyThreadEPNS_6ThreadE"}}, 
   {{0xffffffff80023da0}, {"_ZN4PROC7Process14SetThreadStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff80023dd0}, {"_ZN4PROC7Process14GetThreadStateEm"}}, 
   {{0xffffffff80023e00}, {"_ZN4PROC7Process13SetMainThreadEm"}}, 
   {{0xffffffff80023e30}, {"_ZN4PROC7Process13GetMainThreadEv"}}, 
   {{0xffffffff80023e40}, {"_ZN4PROC7Process16SetMessageThreadEPv"}}, 
   {{0xffffffff80023e50}, {"_ZN4PROC7Process16GetMessageThreadEv"}}, 
   {{0xffffffff80023e60}, {"_ZN4PROC7Process11SetPriorityEh"}}, 
   {{0xffffffff80023e70}, {"_ZN4PROC7Process15SetProcessStateENS_12ProcessStateE"}}, 
   {{0xffffffff80023eb0}, {"_ZN4PROC7Process6GetPIDEv"}}, 
   {{0xffffffff80023ec0}, {"_ZN4PROC7Process15GetProcessStateEv"}}, 
   {{0xffffffff80023ed0}, {"_ZN4PROC7Process21GetVirtualMemorySpaceEv"}}, 
   {{0xffffffff80023ee0}, {"_ZN4PROC7Process14GetHighestFreeEv"}}, 
   {{0xffffffff80023ef0}, {"_ZN4PROC7Process14SetHighestFreeEm"}}, 
   {{0xffffffff80023f00}, {"_ZN4PROC6ThreadC1EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80023f00}, {"_ZN4PROC6ThreadC2EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80024080}, {"_ZN4PROC7Process12CreateThreadEmm.part.0"}}, 
   {{0xffffffff80024170}, {"_ZN4PROC7Process12CreateThreadEmm"}}, 
   {{0xffffffff80024190}, {"_ZN4PROC7ProcessC1ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024190}, {"_ZN4PROC7ProcessC2ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024280}, {"_ZN4PROC6ThreadD1Ev"}}, 
   {{0xffffffff80024280}, {"_ZN4PROC6ThreadD2Ev"}}, 
   {{0xffffffff800242b0}, {"_ZN4PROC6Thread8SetStateENS_12ProcessStateE"}}, 
   {{0xffffffff800242c0}, {"_ZN4PROC6Thread8GetStateEv"}}, 
   {{0xffffffff800242d0}, {"_ZN4PROC6Thread6GetTIDEv"}}, 
   {{0xffffffff800242e0}, {"_ZN4PROC6Thread14SetInstructionEm"}}, 
   {{0xffffffff800242f0}, {"_ZN4PROC6Thread8SetStackEm"}}, 
   {{0xffffffff80024300}, {"_ZN4PROC6Thread12GetStackBaseEv"}}, 
   {{0xffffffff80024310}, {"_ZN4PROC6Thread8GetStackEv"}}, 
   {{0xffffffff80024320}, {"_ZN4PROC6Thread12GetStackSizeEv"}}, 
   {{0xffffffff80024330}, {"_ZN4PROC9Scheduler7AddNodeEPNS_11ProcessNodeEPNS_7ProcessE"}}, 
   {{0xffffffff800243b0}, {"_ZN4PROC9Scheduler12PopFirstNodeEPNS_11ProcessNodeE"}}, 
   {{0xffffffff800243f0}, {"_ZN4PROC9Scheduler10RemoveNodeEPNS_11ProcessNodeEm"}}, 
   {{0xffffffff80024470}, {"_ZN4PROC9Scheduler8FindNodeEPNS_11ProcessNodeEmPS2_Pb"}}, 
   {{0xffffffff800244f0}, {"_ZN4PROC9SchedulerC1Ev"}}, 
   {{0xffffffff800244f0}, {"_ZN4PROC9SchedulerC2Ev"}}, 
   {{0xffffffff800245e0}, {"_ZN4PROC9Scheduler10AddProcessEPNS_7ProcessE"}}, 
   {{0xffffffff80024640}, {"_ZN4PROC9Scheduler10GetProcessEmPNS_11ProcessNodeE"}}, 
   {{0xffffffff800246a0}, {"_ZN4PROC9Scheduler10GetProcessEm"}}, 
   {{0xffffffff80024750}, {"_ZN4PROC9Scheduler15SetProcessStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff80024860}, {"_ZN4PROC9Scheduler17GetRunningProcessEv"}}, 
   {{0xffffffff80024870}, {"_ZN4PROC9Scheduler12SwitchToTaskEPNS_7ProcessEm"}}, 
   {{0xffffffff80024990}, {"_ZN4PROC9Scheduler20RecalculateSchedulerEv"}}, 
   {{0xffffffff80024a00}, {"_ZN6BitmapixEm"}}, 
   {{0xffffffff80024a30}, {"_ZN6Bitmap3GetEm"}}, 
   {{0xffffffff80024a60}, {"_ZN6Bitmap3SetEmb"}}, 
   {{0xffffffff80024ab0}, {"_Z5IoctlP6Drivermz"}}, 
   {{0xffffffff80024b00}, {"_Z18LoadProgramHeadersPhmP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024e30}, {"_Z11LinkSymbolsPhmP10Elf64_Ehdr"}}, 
   {{0xffffffff80024e50}, {"_Z11LoadProcessP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024f00}, {"_Z7LoadELFPhm"}}, 
   {{0xffffffff80024f70}, {"_ZN4FILE4OpenEPcPm"}}, 
   {{0xffffffff80025100}, {"__cxa_guard_acquire"}}, 
   {{0xffffffff80025110}, {"__cxa_guard_release"}}, 
   {{0xffffffff80025120}, {"__cxa_guard_abort"}}, 
   {{0xffffffff80025130}, {"__cxa_atexit"}}, 
   {{0xffffffff80025180}, {"__cxa_finalize"}}, 
   {{0xffffffff80025230}, {"_Z16RequestInterruptj"}}, 
   {{0xffffffff80025240}, {"_Z18LoadExecutableFilePhm"}}, 
   {{0xffffffff80025250}, {"_Z12TryLockMutexPb"}}, 
   {{0xffffffff80025260}, {"_Z17SpinlockLockMutexPb"}}, 
   {{0xffffffff80025270}, {"_Z11UnlockMutexPb"}}, 
   {{0xffffffff80025280}, {"_Z5PanicPKcS0_S0_j"}}, 
   {{0xffffffff800252b0}, {"_Z4OopsPKcS0_S0_j"}}, 
   {{0xffffffff800252e0}, {"_ZN6PRINTK11FlushBufferEv"}}, 
   {{0xffffffff80025320}, {"_ZN6PRINTK7PutCharEc"}}, 
   {{0xffffffff80025400}, {"_ZN6PRINTK6PutStrEPc"}}, 
   {{0xffffffff800254f0}, {"_ZN6PRINTK7VPrintKEPcP13__va_list_tag"}}, 
   {{0xffffffff80025720}, {"_ZN6PRINTK6PrintKEPcz"}}, 
   {{0xffffffff800257a0}, {"_ZN6PRINTK9EarlyInitEv"}}, 
   {{0xffffffff80025800}, {"_GLOBAL__sub_I_kernelPort"}}, 
   {{0xffffffff80025820}, {"_Z12LookupSymbolPKc"}}, 
   {{0xffffffff80025890}, {"_Z12LookupSymbolm"}}, 
   {{0xffffffff80025900}, {"_Z11AddOverridem"}}, 
   {{0xffffffff80025910}, {"_Z13CheckOverridem"}}, 
   {{0xffffffff80025920}, {"_Z11RunOverridem"}}, 
   {{0xffffffff80025930}, {"_Z24HandleSyscallDebugPrintKPKc"}}, 
   {{0xffffffff80025950}, {"_Z26HandleSyscallMemoryGetinfom"}}, 
   {{0xffffffff800259b0}, {"_Z26HandleSyscallMemoryVmallocmmm"}}, 
   {{0xffffffff80025ad0}, {"_Z25HandleSyscallMemoryVmfreemm"}}, 
   {{0xffffffff80025bd0}, {"_Z23HandleSyscallMemoryMmapmmmm"}}, 
   {{0xffffffff80025ca0}, {"_Z24HandleSyscallMemoryUnmapmm"}}, 
   {{0xffffffff80025d80}, {"_Z21HandleSyscallProcExecmm"}}, 
   {{0xffffffff80025ea0}, {"_Z21HandleSyscallProcForkm"}}, 
   {{0xffffffff80025eb0}, {"_Z23HandleSyscallProcReturnmm"}}, 
   {{0xffffffff80025f20}, {"_Z21HandleSyscallProcExitmm"}}, 
   {{0xffffffff80025f60}, {"_Z21HandleSyscallProcWaitm"}}, 
   {{0xffffffff80025f70}, {"_Z21HandleSyscallProcKillm"}}, 
   {{0xffffffff80025f80}, {"_Z27HandleSyscallModuleRegistermm"}}, 
   {{0xffffffff80025ff0}, {"_Z29HandleSyscallModuleUnregisterv"}}, 
   {{0xffffffff80026060}, {"_Z31HandleSyscallModuleBufferCreatemmPj"}}, 
   {{0xffffffff80026110}, {"_Z28HandleSyscallModuleBufferMapmj"}}, 
   {{0xffffffff800261c0}, {"_Z30HandleSyscallModuleBufferUnmapmj"}}, 
   {{0xffffffff800261d0}, {"_Z31HandleSyscallModuleBufferDeletej"}}, 
   {{0xffffffff800261e0}, {"_Z33HandleSyscallModuleMessageHandlerm"}}, 
   {{0xffffffff80026240}, {"_Z30HandleSyscallModuleMessageSendjjPvm"}}, 
   {{0xffffffff80026480}, {"_Z34HandleSyscallModuleSectionRegisterPKc"}}, 
   {{0xffffffff80026560}, {"_Z29HandleSyscallModuleSectionGetPKcPjS1_"}}, 
   {{0xffffffff80026620}, {"_Z36HandleSyscallModuleSectionUnregisterPKc"}}, 
   {{0xffffffff800266f0}, {"_Z21HandleSyscallFileOpenPcPmS0_"}}, 
   {{0xffffffff80026790}, {"_Z21HandleSyscallFileReadPcmm"}}, 
   {{0xffffffff80026830}, {"HandleSyscall"}}, 
   {{0xffffffff80026b70}, {"_Z22HandleSyscallFileWritem"}}, 
   {{0xffffffff80026b80}, {"_Z22HandleSyscallFileClosem"}}, 
   {{0xffffffff80026b90}, {"_Z25HandleSyscallKernOverridem"}}, 
   {{0xffffffff80026ba0}, {"_ZN5TIMER13WaitForCyclesEm"}}, 
   {{0xffffffff80026bb0}, {"_ZN6x86_646x86CPU4InitEv"}}, 
   {{0xffffffff80026cb0}, {"_ZN6x86_646x86CPU9GetVendorEPc"}}, 
   {{0xffffffff80026cd0}, {"_ZN6x86_647TSSInitEm"}}, 
   {{0xffffffff80026d70}, {"_ZN6x86_647LoadGDTEm"}}, 
   {{0xffffffff80026dc0}, {"_Z15InitializeStackPvm"}}, 
   {{0xffffffff80026dd0}, {"_ZN6x86_648LoadMADTEv"}}, 
   {{0xffffffff80026f20}, {"_ZN6x86_6411GetAPICBaseEv"}}, 
   {{0xffffffff80026f30}, {"_ZN6x86_6411SetAPICBaseEm"}}, 
   {{0xffffffff80026f50}, {"_ZN6x86_6417WriteAPICRegisterEtj"}}, 
   {{0xffffffff80026f70}, {"_ZN6x86_6416ReadAPICRegisterEt"}}, 
   {{0xffffffff80026f90}, {"_ZN6x86_648WaitAPICEj"}}, 
   {{0xffffffff80026fb0}, {"_ZN6x86_6410EnableAPICEv"}}, 
   {{0xffffffff80027080}, {"_ZN6x86_6411SendAPICEOIEv"}}, 
   {{0xffffffff800270a0}, {"_ZN6x86_6417WriteHPETRegisterEmjj"}}, 
   {{0xffffffff800270b0}, {"_ZN6x86_6412EnableIOAPICEm"}}, 
   {{0xffffffff800270f0}, {"_ZN6x86_6418ReadIOAPICRegisterEmt"}}, 
   {{0xffffffff80027100}, {"_ZN6x86_6419WriteIOAPICRegisterEmtm"}}, 
   {{0xffffffff80027110}, {"_ZN6x86_647IDTInitEv"}}, 
   {{0xffffffff80027240}, {"exceptionHandler"}}, 
   {{0xffffffff80027370}, {"timerHandler"}}, 
   {{0xffffffff800273c0}, {"spuriousHandler"}}, 
   {{0xffffffff800273f0}, {"syscallHandler"}}, 
   {{0xffffffff80027430}, {"_ZN6x86_644OutBEth"}}, 
   {{0xffffffff80027440}, {"_ZN6x86_644OutWEtt"}}, 
   {{0xffffffff80027450}, {"_ZN6x86_643InBEt"}}, 
   {{0xffffffff80027460}, {"_ZN6x86_643InWEt"}}, 
   {{0xffffffff80027470}, {"_ZN6x86_646IoWaitEv"}}, 
   {{0xffffffff80027480}, {"_ZN6x86_644InitEv"}}, 
   {{0xffffffff80027540}, {"_ZN14PageMapIndexerC1Em"}}, 
   {{0xffffffff80027540}, {"_ZN14PageMapIndexerC2Em"}}, 
   {{0xffffffff80027580}, {"_ZN16PageTableManager9MapMemoryEPvS0_m"}}, 
   {{0xffffffff800278e0}, {"_ZN16PageTableManager11UnmapMemoryEPv"}}, 
   {{0xffffffff800279d0}, {"_ZN16PageTableManager18GetPhysicalAddressEPv"}}, 
   {{0xffffffff80027aa0}, {"_ZN16PageTableManager4ForkEPN3VMM12VirtualSpaceEb"}}, 
   {{0xffffffff80027e70}, {"_ZN16PageTableManagerC1EP9PageTable"}}, 
   {{0xffffffff80027e70}, {"_ZN16PageTableManagerC2EP9PageTable"}}, 
   {{0xffffffff80027e80}, {"_ZN16PageTableManager13GetTopAddressEv"}}, 
   {{0xffffffff80027e90}, {"_ZN18PageDirectoryEntry7SetFlagE7PT_Flagb"}}, 
   {{0xffffffff80027ec0}, {"_ZN18PageDirectoryEntry7GetFlagE7PT_Flag"}}, 
   {{0xffffffff80027ed0}, {"_ZN18PageDirectoryEntry10SetAddressEm"}}, 
   {{0xffffffff80027f00}, {"_ZN18PageDirectoryEntry10GetAddressEv"}}, 
   {{0xffffffff80027f10}, {"_ZN6x86_6415NewVirtualSpaceEv"}}, 
   {{0xffffffff80027f50}, {"_ZN6x86_6416LoadVirtualSpaceEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80027f80}, {"CPUPause"}}, 
   {{0xffffffff80027f90}, {"FlushGDT"}}, 
   {{0xffffffff80027faa}, {"FlushTSS"}}, 
   {{0xffffffff80027fc0}, {"EnableSCE"}}, 
   {{0xffffffff80028000}, {"EnableSSE"}}, 
   {{0xffffffff8002801b}, {"EnableAVX"}}, 
   {{0xffffffff80028030}, {"SwitchStack"}}, 
   {{0xffffffff80028070}, {"EnterUserspace"}}, 
   {{0xffffffff80028082}, {"UserStub"}}, 
   {{0xffffffff800280a0}, {"isr_stub_0"}}, 
   {{0xffffffff800280e1}, {"isr_stub_1"}}, 
   {{0xffffffff80028122}, {"isr_stub_2"}}, 
   {{0xffffffff80028163}, {"isr_stub_3"}}, 
   {{0xffffffff800281a4}, {"isr_stub_4"}}, 
   {{0xffffffff800281e5}, {"isr_stub_5"}}, 
   {{0xffffffff80028226}, {"isr_stub_6"}}, 
   {{0xffffffff80028267}, {"isr_stub_7"}}, 
   {{0xffffffff800282a8}, {"isr_stub_8"}}, 
   {{0xffffffff800282e7}, {"isr_stub_9"}}, 
   {{0xffffffff80028328}, {"isr_stub_10"}}, 
   {{0xffffffff80028367}, {"isr_stub_11"}}, 
   {{0xffffffff800283a6}, {"isr_stub_12"}}, 
   {{0xffffffff800283e5}, {"isr_stub_13"}}, 
   {{0xffffffff80028424}, {"isr_stub_14"}}, 
   {{0xffffffff80028463}, {"isr_stub_15"}}, 
   {{0xffffffff800284a4}, {"isr_stub_16"}}, 
   {{0xffffffff800284e5}, {"isr_stub_17"}}, 
   {{0xffffffff80028524}, {"isr_stub_18"}}, 
   {{0xffffffff80028565}, {"isr_stub_19"}}, 
   {{0xffffffff800285a6}, {"isr_stub_20"}}, 
   {{0xffffffff800285e7}, {"isr_stub_21"}}, 
   {{0xffffffff80028628}, {"isr_stub_22"}}, 
   {{0xffffffff80028669}, {"isr_stub_23"}}, 
   {{0xffffffff800286aa}, {"isr_stub_24"}}, 
   {{0xffffffff800286eb}, {"isr_stub_25"}}, 
   {{0xffffffff8002872c}, {"isr_stub_26"}}, 
   {{0xffffffff8002876d}, {"isr_stub_27"}}, 
   {{0xffffffff800287ae}, {"isr_stub_28"}}, 
   {{0xffffffff800287ef}, {"isr_stub_29"}}, 
   {{0xffffffff80028830}, {"isr_stub_30"}}, 
   {{0xffffffff8002886f}, {"isr_stub_31"}}, 
   {{0xffffffff800288b0}, {"isr_stub_32"}}, 
   {{0xffffffff800288f1}, {"isr_stub_33"}}, 
   {{0xffffffff80028923}, {"isr_stub_34"}}, 
   {{0xffffffff80028955}, {"isr_stub_35"}}, 
   {{0xffffffff80028987}, {"isr_stub_36"}}, 
   {{0xffffffff800289b9}, {"isr_stub_37"}}, 
   {{0xffffffff800289eb}, {"isr_stub_38"}}, 
   {{0xffffffff80028a1d}, {"isr_stub_39"}}, 
   {{0xffffffff80028a5e}, {"isr_stub_40"}}, 
   {{0xffffffff80028a90}, {"isr_stub_41"}}, 
   {{0xffffffff80028ac2}, {"isr_stub_42"}}, 
   {{0xffffffff80028af4}, {"isr_stub_43"}}, 
   {{0xffffffff80028b26}, {"isr_stub_44"}}, 
   {{0xffffffff80028b58}, {"isr_stub_45"}}, 
   {{0xffffffff80028b8a}, {"isr_stub_46"}}, 
   {{0xffffffff80028bbc}, {"isr_stub_47"}}, 
   {{0xffffffff80028bee}, {"isr_stub_48"}}, 
   {{0xffffffff80028c20}, {"isr_stub_49"}}, 
   {{0xffffffff80028c52}, {"isr_stub_50"}}, 
   {{0xffffffff80028c84}, {"isr_stub_51"}}, 
   {{0xffffffff80028cb6}, {"isr_stub_52"}}, 
   {{0xffffffff80028ce8}, {"isr_stub_53"}}, 
   {{0xffffffff80028d1a}, {"isr_stub_54"}}, 
   {{0xffffffff80028d4c}, {"isr_stub_55"}}, 
   {{0xffffffff80028d7e}, {"isr_stub_56"}}, 
   {{0xffffffff80028db0}, {"isr_stub_57"}}, 
   {{0xffffffff80028de2}, {"isr_stub_58"}}, 
   {{0xffffffff80028e14}, {"isr_stub_59"}}, 
   {{0xffffffff80028e46}, {"isr_stub_60"}}, 
   {{0xffffffff80028e78}, {"isr_stub_61"}}, 
   {{0xffffffff80028eaa}, {"isr_stub_62"}}, 
   {{0xffffffff80028edc}, {"isr_stub_63"}}, 
   {{0xffffffff80028f0e}, {"isr_stub_64"}}, 
   {{0xffffffff80028f40}, {"isr_stub_65"}}, 
   {{0xffffffff80028f72}, {"isr_stub_66"}}, 
   {{0xffffffff80028fa4}, {"isr_stub_67"}}, 
   {{0xffffffff80028fd6}, {"isr_stub_68"}}, 
   {{0xffffffff80029008}, {"isr_stub_69"}}, 
   {{0xffffffff8002903a}, {"isr_stub_70"}}, 
   {{0xffffffff8002906c}, {"isr_stub_71"}}, 
   {{0xffffffff8002909e}, {"isr_stub_72"}}, 
   {{0xffffffff800290d0}, {"isr_stub_73"}}, 
   {{0xffffffff80029102}, {"isr_stub_74"}}, 
   {{0xffffffff80029134}, {"isr_stub_75"}}, 
   {{0xffffffff80029166}, {"isr_stub_76"}}, 
   {{0xffffffff80029198}, {"isr_stub_77"}}, 
   {{0xffffffff800291ca}, {"isr_stub_78"}}, 
   {{0xffffffff800291fc}, {"isr_stub_79"}}, 
   {{0xffffffff8002922e}, {"isr_stub_80"}}, 
   {{0xffffffff80029260}, {"isr_stub_81"}}, 
   {{0xffffffff80029292}, {"isr_stub_82"}}, 
   {{0xffffffff800292c4}, {"isr_stub_83"}}, 
   {{0xffffffff800292f6}, {"isr_stub_84"}}, 
   {{0xffffffff80029328}, {"isr_stub_85"}}, 
   {{0xffffffff8002935a}, {"isr_stub_86"}}, 
   {{0xffffffff8002938c}, {"isr_stub_87"}}, 
   {{0xffffffff800293be}, {"isr_stub_88"}}, 
   {{0xffffffff800293f0}, {"isr_stub_89"}}, 
   {{0xffffffff80029422}, {"isr_stub_90"}}, 
   {{0xffffffff80029454}, {"isr_stub_91"}}, 
   {{0xffffffff80029486}, {"isr_stub_92"}}, 
   {{0xffffffff800294b8}, {"isr_stub_93"}}, 
   {{0xffffffff800294ea}, {"isr_stub_94"}}, 
   {{0xffffffff8002951c}, {"isr_stub_95"}}, 
   {{0xffffffff8002954e}, {"isr_stub_96"}}, 
   {{0xffffffff80029580}, {"isr_stub_97"}}, 
   {{0xffffffff800295b2}, {"isr_stub_98"}}, 
   {{0xffffffff800295e4}, {"isr_stub_99"}}, 
   {{0xffffffff80029616}, {"isr_stub_100"}}, 
   {{0xffffffff80029648}, {"isr_stub_101"}}, 
   {{0xffffffff8002967a}, {"isr_stub_102"}}, 
   {{0xffffffff800296ac}, {"isr_stub_103"}}, 
   {{0xffffffff800296de}, {"isr_stub_104"}}, 
   {{0xffffffff80029710}, {"isr_stub_105"}}, 
   {{0xffffffff80029742}, {"isr_stub_106"}}, 
   {{0xffffffff80029774}, {"isr_stub_107"}}, 
   {{0xffffffff800297a6}, {"isr_stub_108"}}, 
   {{0xffffffff800297d8}, {"isr_stub_109"}}, 
   {{0xffffffff8002980a}, {"isr_stub_110"}}, 
   {{0xffffffff8002983c}, {"isr_stub_111"}}, 
   {{0xffffffff8002986e}, {"isr_stub_112"}}, 
   {{0xffffffff800298a0}, {"isr_stub_113"}}, 
   {{0xffffffff800298d2}, {"isr_stub_114"}}, 
   {{0xffffffff80029904}, {"isr_stub_115"}}, 
   {{0xffffffff80029936}, {"isr_stub_116"}}, 
   {{0xffffffff80029968}, {"isr_stub_117"}}, 
   {{0xffffffff8002999a}, {"isr_stub_118"}}, 
   {{0xffffffff800299cc}, {"isr_stub_119"}}, 
   {{0xffffffff800299fe}, {"isr_stub_120"}}, 
   {{0xffffffff80029a30}, {"isr_stub_121"}}, 
   {{0xffffffff80029a62}, {"isr_stub_122"}}, 
   {{0xffffffff80029a94}, {"isr_stub_123"}}, 
   {{0xffffffff80029ac6}, {"isr_stub_124"}}, 
   {{0xffffffff80029af8}, {"isr_stub_125"}}, 
   {{0xffffffff80029b2a}, {"isr_stub_126"}}, 
   {{0xffffffff80029b5c}, {"isr_stub_127"}}, 
   {{0xffffffff80029b8e}, {"isr_stub_128"}}, 
   {{0xffffffff80029bc0}, {"isr_stub_129"}}, 
   {{0xffffffff80029bf2}, {"isr_stub_130"}}, 
   {{0xffffffff80029c24}, {"isr_stub_131"}}, 
   {{0xffffffff80029c56}, {"isr_stub_132"}}, 
   {{0xffffffff80029c88}, {"isr_stub_133"}}, 
   {{0xffffffff80029cba}, {"isr_stub_134"}}, 
   {{0xffffffff80029cec}, {"isr_stub_135"}}, 
   {{0xffffffff80029d1e}, {"isr_stub_136"}}, 
   {{0xffffffff80029d50}, {"isr_stub_137"}}, 
   {{0xffffffff80029d82}, {"isr_stub_138"}}, 
   {{0xffffffff80029db4}, {"isr_stub_139"}}, 
   {{0xffffffff80029de6}, {"isr_stub_140"}}, 
   {{0xffffffff80029e18}, {"isr_stub_141"}}, 
   {{0xffffffff80029e4a}, {"isr_stub_142"}}, 
   {{0xffffffff80029e7c}, {"isr_stub_143"}}, 
   {{0xffffffff80029eae}, {"isr_stub_144"}}, 
   {{0xffffffff80029ee0}, {"isr_stub_145"}}, 
   {{0xffffffff80029f12}, {"isr_stub_146"}}, 
   {{0xffffffff80029f44}, {"isr_stub_147"}}, 
   {{0xffffffff80029f76}, {"isr_stub_148"}}, 
   {{0xffffffff80029fa8}, {"isr_stub_149"}}, 
   {{0xffffffff80029fda}, {"isr_stub_150"}}, 
   {{0xffffffff8002a00c}, {"isr_stub_151"}}, 
   {{0xffffffff8002a03e}, {"isr_stub_152"}}, 
   {{0xffffffff8002a070}, {"isr_stub_153"}}, 
   {{0xffffffff8002a0a2}, {"isr_stub_154"}}, 
   {{0xffffffff8002a0d4}, {"isr_stub_155"}}, 
   {{0xffffffff8002a106}, {"isr_stub_156"}}, 
   {{0xffffffff8002a138}, {"isr_stub_157"}}, 
   {{0xffffffff8002a16a}, {"isr_stub_158"}}, 
   {{0xffffffff8002a19c}, {"isr_stub_159"}}, 
   {{0xffffffff8002a1ce}, {"isr_stub_160"}}, 
   {{0xffffffff8002a200}, {"isr_stub_161"}}, 
   {{0xffffffff8002a232}, {"isr_stub_162"}}, 
   {{0xffffffff8002a264}, {"isr_stub_163"}}, 
   {{0xffffffff8002a296}, {"isr_stub_164"}}, 
   {{0xffffffff8002a2c8}, {"isr_stub_165"}}, 
   {{0xffffffff8002a2fa}, {"isr_stub_166"}}, 
   {{0xffffffff8002a32c}, {"isr_stub_167"}}, 
   {{0xffffffff8002a35e}, {"isr_stub_168"}}, 
   {{0xffffffff8002a390}, {"isr_stub_169"}}, 
   {{0xffffffff8002a3c2}, {"isr_stub_170"}}, 
   {{0xffffffff8002a3f4}, {"isr_stub_171"}}, 
   {{0xffffffff8002a426}, {"isr_stub_172"}}, 
   {{0xffffffff8002a458}, {"isr_stub_173"}}, 
   {{0xffffffff8002a48a}, {"isr_stub_174"}}, 
   {{0xffffffff8002a4bc}, {"isr_stub_175"}}, 
   {{0xffffffff8002a4ee}, {"isr_stub_176"}}, 
   {{0xffffffff8002a520}, {"isr_stub_177"}}, 
   {{0xffffffff8002a552}, {"isr_stub_178"}}, 
   {{0xffffffff8002a584}, {"isr_stub_179"}}, 
   {{0xffffffff8002a5b6}, {"isr_stub_180"}}, 
   {{0xffffffff8002a5e8}, {"isr_stub_181"}}, 
   {{0xffffffff8002a61a}, {"isr_stub_182"}}, 
   {{0xffffffff8002a64c}, {"isr_stub_183"}}, 
   {{0xffffffff8002a67e}, {"isr_stub_184"}}, 
   {{0xffffffff8002a6b0}, {"isr_stub_185"}}, 
   {{0xffffffff8002a6e2}, {"isr_stub_186"}}, 
   {{0xffffffff8002a714}, {"isr_stub_187"}}, 
   {{0xffffffff8002a746}, {"isr_stub_188"}}, 
   {{0xffffffff8002a778}, {"isr_stub_189"}}, 
   {{0xffffffff8002a7aa}, {"isr_stub_190"}}, 
   {{0xffffffff8002a7dc}, {"isr_stub_191"}}, 
   {{0xffffffff8002a80e}, {"isr_stub_192"}}, 
   {{0xffffffff8002a840}, {"isr_stub_193"}}, 
   {{0xffffffff8002a872}, {"isr_stub_194"}}, 
   {{0xffffffff8002a8a4}, {"isr_stub_195"}}, 
   {{0xffffffff8002a8d6}, {"isr_stub_196"}}, 
   {{0xffffffff8002a908}, {"isr_stub_197"}}, 
   {{0xffffffff8002a93a}, {"isr_stub_198"}}, 
   {{0xffffffff8002a96c}, {"isr_stub_199"}}, 
   {{0xffffffff8002a99e}, {"isr_stub_200"}}, 
   {{0xffffffff8002a9d0}, {"isr_stub_201"}}, 
   {{0xffffffff8002aa02}, {"isr_stub_202"}}, 
   {{0xffffffff8002aa34}, {"isr_stub_203"}}, 
   {{0xffffffff8002aa66}, {"isr_stub_204"}}, 
   {{0xffffffff8002aa98}, {"isr_stub_205"}}, 
   {{0xffffffff8002aaca}, {"isr_stub_206"}}, 
   {{0xffffffff8002aafc}, {"isr_stub_207"}}, 
   {{0xffffffff8002ab2e}, {"isr_stub_208"}}, 
   {{0xffffffff8002ab60}, {"isr_stub_209"}}, 
   {{0xffffffff8002ab92}, {"isr_stub_210"}}, 
   {{0xffffffff8002abc4}, {"isr_stub_211"}}, 
   {{0xffffffff8002abf6}, {"isr_stub_212"}}, 
   {{0xffffffff8002ac28}, {"isr_stub_213"}}, 
   {{0xffffffff8002ac5a}, {"isr_stub_214"}}, 
   {{0xffffffff8002ac8c}, {"isr_stub_215"}}, 
   {{0xffffffff8002acbe}, {"isr_stub_216"}}, 
   {{0xffffffff8002acf0}, {"isr_stub_217"}}, 
   {{0xffffffff8002ad22}, {"isr_stub_218"}}, 
   {{0xffffffff8002ad54}, {"isr_stub_219"}}, 
   {{0xffffffff8002ad86}, {"isr_stub_220"}}, 
   {{0xffffffff8002adb8}, {"isr_stub_221"}}, 
   {{0xffffffff8002adea}, {"isr_stub_222"}}, 
   {{0xffffffff8002ae1c}, {"isr_stub_223"}}, 
   {{0xffffffff8002ae4e}, {"isr_stub_224"}}, 
   {{0xffffffff8002ae80}, {"isr_stub_225"}}, 
   {{0xffffffff8002aeb2}, {"isr_stub_226"}}, 
   {{0xffffffff8002aee4}, {"isr_stub_227"}}, 
   {{0xffffffff8002af16}, {"isr_stub_228"}}, 
   {{0xffffffff8002af48}, {"isr_stub_229"}}, 
   {{0xffffffff8002af7a}, {"isr_stub_230"}}, 
   {{0xffffffff8002afac}, {"isr_stub_231"}}, 
   {{0xffffffff8002afde}, {"isr_stub_232"}}, 
   {{0xffffffff8002b010}, {"isr_stub_233"}}, 
   {{0xffffffff8002b042}, {"isr_stub_234"}}, 
   {{0xffffffff8002b074}, {"isr_stub_235"}}, 
   {{0xffffffff8002b0a6}, {"isr_stub_236"}}, 
   {{0xffffffff8002b0d8}, {"isr_stub_237"}}, 
   {{0xffffffff8002b10a}, {"isr_stub_238"}}, 
   {{0xffffffff8002b13c}, {"isr_stub_239"}}, 
   {{0xffffffff8002b16e}, {"isr_stub_240"}}, 
   {{0xffffffff8002b1a0}, {"isr_stub_241"}}, 
   {{0xffffffff8002b1d2}, {"isr_stub_242"}}, 
   {{0xffffffff8002b204}, {"isr_stub_243"}}, 
   {{0xffffffff8002b236}, {"isr_stub_244"}}, 
   {{0xffffffff8002b268}, {"isr_stub_245"}}, 
   {{0xffffffff8002b29a}, {"isr_stub_246"}}, 
   {{0xffffffff8002b2cc}, {"isr_stub_247"}}, 
   {{0xffffffff8002b2fe}, {"isr_stub_248"}}, 
   {{0xffffffff8002b330}, {"isr_stub_249"}}, 
   {{0xffffffff8002b362}, {"isr_stub_250"}}, 
   {{0xffffffff8002b394}, {"isr_stub_251"}}, 
   {{0xffffffff8002b3c6}, {"isr_stub_252"}}, 
   {{0xffffffff8002b3f8}, {"isr_stub_253"}}, 
   {{0xffffffff8002b42a}, {"isr_stub_254"}}, 
   {{0xffffffff8002b46e}, {"isr_stub_255"}}, 
   {{0xffffffff8002b4a0}, {"isrStubTable"}}, 
   {{0xffffffff8002d000}, {"_ZTV10UARTDevice"}}, 
   {{0xffffffff8002d2e8}, {"_ZTV8SpinLock"}}, 
   {{0xffffffff8002d3c0}, {"_ZL14memTypeStrings"}}, 
   {{0xffffffff80030aa0}, {"symbols"}}, 
   {{0xffffffff80033000}, {"symbolCount"}}, 
   {{0xffffffff80033598}, {"_ZTV16PageTableManager"}}, 
   {{0xffffffff80035000}, {"_ZL17kernelFileRequest"}}, 
   {{0xffffffff80035040}, {"_ZL18framebufferRequest"}}, 
   {{0xffffffff80035080}, {"_ZL11rsdpRequest"}}, 
   {{0xffffffff800350c0}, {"_ZL12kaddrRequest"}}, 
   {{0xffffffff80035100}, {"_ZL11mMapRequest"}}, 
   {{0xffffffff80035140}, {"_ZL11hhdmRequest"}}, 
   {{0xffffffff80035180}, {"_ZL13moduleRequest"}}, 
   {{0xffffffff800351c0}, {"_ZL12stackRequest"}}, 
   {{0xffffffff80035200}, {"_ZL17entryPointRequest"}}, 
   {{0xffffffff80035238}, {"__stack_chk_guard"}}, 
   {{0xffffffff80035240}, {"TypeCheckKinds"}}, 
   {{0xffffffff80035290}, {"bootmemStatus"}}, 
   {{0xffffffff80035298}, {"_ZN4PROCL10CurrentPIDE"}}, 
   {{0xffffffff800352a0}, {"StartSyscallStack"}}, 
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
   {{0xffffffff80039ce8}, {"_ZN6PRINTKL16TerminalPositionE"}}, 
   {{0xffffffff80039d00}, {"_ZN6PRINTKL14TerminalColumnE"}}, 
   {{0xffffffff80039e00}, {"_ZN6PRINTKL14PrintKSpinlockE"}}, 
   {{0xffffffff8003a000}, {"gdtPointer"}}, 
   {{0xffffffff8003b000}, {"tss"}}, 
   {{0xffffffff8003b070}, {"idtr"}}, 
   {{0xffffffff8003b080}, {"idt"}}, 
};
