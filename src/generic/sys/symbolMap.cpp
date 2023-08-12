#include <sys/symbol.hpp>
const uint64_t symbolCount = 603;
const Symbol symbols[] = {
   {{0xffffffff80000000}, {"SyscallEntry"}}, 
   {{0xffffffff80000000}, {"syscall_start"}}, 
   {{0xffffffff80000050}, {"SyscallStack"}}, 
   {{0xffffffff80020050}, {"syscall_end"}}, 
   {{0xffffffff80021050}, {"EfiMain"}}, 
   {{0xffffffff80021070}, {"LimineEntry"}}, 
   {{0xffffffff80021420}, {"__stack_chk_fail"}}, 
   {{0xffffffff80021460}, {"_Z11UnwindStacki"}}, 
   {{0xffffffff800214f0}, {"__ubsan_handle_missing_return"}}, 
   {{0xffffffff80021500}, {"__ubsan_handle_divrem_overflow"}}, 
   {{0xffffffff80021510}, {"__ubsan_handle_load_invalid_value"}}, 
   {{0xffffffff80021520}, {"__ubsan_handle_shift_out_of_bounds"}}, 
   {{0xffffffff80021530}, {"__ubsan_handle_builtin_unreachable"}}, 
   {{0xffffffff80021540}, {"__ubsan_handle_mul_overflow"}}, 
   {{0xffffffff80021550}, {"__ubsan_handle_negate_overflow"}}, 
   {{0xffffffff80021560}, {"__ubsan_handle_out_of_bounds"}}, 
   {{0xffffffff800215a0}, {"__ubsan_handle_pointer_overflow"}}, 
   {{0xffffffff800215b0}, {"__ubsan_handle_add_overflow"}}, 
   {{0xffffffff800215c0}, {"__ubsan_handle_sub_overflow"}}, 
   {{0xffffffff800215d0}, {"__ubsan_handle_type_mismatch_v1"}}, 
   {{0xffffffff80021690}, {"_ZN10UARTDevice4InitE11SerialPorts"}}, 
   {{0xffffffff80021790}, {"_ZN10UARTDevice5IoctlEmz"}}, 
   {{0xffffffff800218f0}, {"_ZN10UARTDevice6PutStrEPKc"}}, 
   {{0xffffffff80021970}, {"_ZN10UARTDevice7PutCharEc"}}, 
   {{0xffffffff800219c0}, {"_ZN10UARTDevice7GetCharEv"}}, 
   {{0xffffffff80021a00}, {"_ZN10UARTDevice15isTransmitEmptyEv"}}, 
   {{0xffffffff80021a20}, {"_ZN10UARTDevice14serialReceivedEv"}}, 
   {{0xffffffff80021a40}, {"_Z9ParseArgsv"}}, 
   {{0xffffffff80021b00}, {"_Z8InitInfov"}}, 
   {{0xffffffff80021b20}, {"_Z7GetInfov"}}, 
   {{0xffffffff80021b30}, {"_Z15EarlyKernelInitv"}}, 
   {{0xffffffff80021b50}, {"_Z11PrintBannerv"}}, 
   {{0xffffffff80021c00}, {"_Z11KernelStartv"}}, 
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
   {{0xffffffff800224f0}, {"_ZN3MEM4InitEv"}}, 
   {{0xffffffff80022500}, {"_Z6memsetPvhm"}}, 
   {{0xffffffff80022550}, {"_ZL12ReservePagesPvm"}}, 
   {{0xffffffff800225d0}, {"_ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022860}, {"_ZN3PMM8FreePageEPv"}}, 
   {{0xffffffff800228e0}, {"_ZN3PMM8LockPageEPv"}}, 
   {{0xffffffff80022960}, {"_ZN3PMM11RequestPageEv"}}, 
   {{0xffffffff80022a30}, {"_ZN3PMM12RequestPagesEm"}}, 
   {{0xffffffff80022b50}, {"_ZN3PMM9FreePagesEPvm"}}, 
   {{0xffffffff80022ba0}, {"_ZN3PMM9LockPagesEPvm"}}, 
   {{0xffffffff80022bf0}, {"_ZN3PMM10GetFreeMemEv"}}, 
   {{0xffffffff80022c00}, {"_ZN3PMM10GetUsedMemEv"}}, 
   {{0xffffffff80022c10}, {"_ZN3PMM14GetReservedMemEv"}}, 
   {{0xffffffff80022c20}, {"_GLOBAL__sub_I__ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022c40}, {"_Z6strlenPKc"}}, 
   {{0xffffffff80022c70}, {"_Z6strcpyPcPKc"}}, 
   {{0xffffffff80022ca0}, {"_Z6strcmpPKcS0_"}}, 
   {{0xffffffff80022cf0}, {"_Z8is_delimcPc"}}, 
   {{0xffffffff80022d30}, {"_Z6strtokPcS_"}}, 
   {{0xffffffff80022e00}, {"_Z4itoaPcix"}}, 
   {{0xffffffff80022ea0}, {"_Z4atoiPc"}}, 
   {{0xffffffff80022ee0}, {"_ZN3VMM21NewModuleVirtualSpaceEv"}}, 
   {{0xffffffff80023090}, {"_ZN3VMM21NewKernelVirtualSpaceEv"}}, 
   {{0xffffffff800231c0}, {"_ZN3VMM7InitVMMEv"}}, 
   {{0xffffffff80023200}, {"_ZN3VMM15NewVirtualSpaceEv"}}, 
   {{0xffffffff80023210}, {"_ZN3VMM16LoadVirtualSpaceEPNS_12VirtualSpaceE"}}, 
   {{0xffffffff80023220}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_"}}, 
   {{0xffffffff80023230}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_m"}}, 
   {{0xffffffff80023240}, {"_ZN6MODULE13BufferManagerC1Ev"}}, 
   {{0xffffffff80023240}, {"_ZN6MODULE13BufferManagerC2Ev"}}, 
   {{0xffffffff80023280}, {"_ZN6MODULE13BufferManager7AddNodeEPNS_6BufferE"}}, 
   {{0xffffffff800232f0}, {"_ZN6MODULE13BufferManager10RemoveNodeEj"}}, 
   {{0xffffffff80023360}, {"_ZN6MODULE13BufferManager8FindNodeEjPPNS_10BufferNodeEPb"}}, 
   {{0xffffffff800233a0}, {"_ZN6MODULE13BufferManager12CreateBufferEjjNS_10BufferTypeEm"}}, 
   {{0xffffffff80023450}, {"_ZN6MODULE13BufferManager9GetBufferEj"}}, 
   {{0xffffffff80023480}, {"_ZN6MODULE13BufferManager9MapBufferEjjjPN3VMM12VirtualSpaceEm"}}, 
   {{0xffffffff800235b0}, {"_ZN6MODULE13BufferManager12DeleteBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023610}, {"_ZN6MODULE13BufferManager10LockBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023640}, {"_ZN6MODULE13BufferManager12UnlockBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023660}, {"_ZN6MODULE14ComposeMessageEPNS_7MessageEjjm"}}, 
   {{0xffffffff800236e0}, {"_ZN6MODULE6ModuleC1EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff800236e0}, {"_ZN6MODULE6ModuleC2EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff800236f0}, {"_ZN6MODULE7ManagerC1Ev"}}, 
   {{0xffffffff800236f0}, {"_ZN6MODULE7ManagerC2Ev"}}, 
   {{0xffffffff80023720}, {"_ZN6MODULE7Manager7AddNodeEjj"}}, 
   {{0xffffffff80023790}, {"_ZN6MODULE7Manager10RemoveNodeEjj"}}, 
   {{0xffffffff80023800}, {"_ZN6MODULE7Manager8FindNodeEjjPPNS_10ModuleNodeEPb"}}, 
   {{0xffffffff80023850}, {"_ZN6MODULE7Manager14RegisterModuleEPN4PROC7ProcessEjj"}}, 
   {{0xffffffff80023920}, {"_ZN6MODULE7Manager9GetModuleEjj"}}, 
   {{0xffffffff80023960}, {"_ZN6MODULE7Manager9GetModuleEm"}}, 
   {{0xffffffff800239b0}, {"_ZN6MODULE7Manager16UnregisterModuleEjj"}}, 
   {{0xffffffff800239c0}, {"_ZN6MODULE14SectionManagerC1Ev"}}, 
   {{0xffffffff800239c0}, {"_ZN6MODULE14SectionManagerC2Ev"}}, 
   {{0xffffffff80023a00}, {"_ZN6MODULE14SectionManager8FindNodeEPKcjjPPNS_11SectionNodeEPb"}}, 
   {{0xffffffff80023ac0}, {"_ZN6MODULE14SectionManager7AddNodeEPKcjj"}}, 
   {{0xffffffff80023b20}, {"_ZN6MODULE14SectionManager10RemoveNodeEPKcjj"}}, 
   {{0xffffffff80023b70}, {"_ZN6MODULE14SectionManager21RegisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023c30}, {"_ZN6MODULE14SectionManager16GetSectionDriverEPKcPjS3_"}}, 
   {{0xffffffff80023c90}, {"_ZN6MODULE14SectionManager23UnregisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023ce0}, {"_ZN4PROC7ProcessD1Ev"}}, 
   {{0xffffffff80023ce0}, {"_ZN4PROC7ProcessD2Ev"}}, 
   {{0xffffffff80023d30}, {"_ZN4PROC7Process9GetThreadEm"}}, 
   {{0xffffffff80023d50}, {"_ZN4PROC7Process10RequestTIDEv"}}, 
   {{0xffffffff80023d60}, {"_ZN4PROC7Process13DestroyThreadEPNS_6ThreadE"}}, 
   {{0xffffffff80023d70}, {"_ZN4PROC7Process14SetThreadStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff80023da0}, {"_ZN4PROC7Process14GetThreadStateEm"}}, 
   {{0xffffffff80023dd0}, {"_ZN4PROC7Process13SetMainThreadEm"}}, 
   {{0xffffffff80023e00}, {"_ZN4PROC7Process13GetMainThreadEv"}}, 
   {{0xffffffff80023e10}, {"_ZN4PROC7Process16SetMessageThreadEPv"}}, 
   {{0xffffffff80023e20}, {"_ZN4PROC7Process16GetMessageThreadEv"}}, 
   {{0xffffffff80023e30}, {"_ZN4PROC7Process11SetPriorityEh"}}, 
   {{0xffffffff80023e40}, {"_ZN4PROC7Process15SetProcessStateENS_12ProcessStateE"}}, 
   {{0xffffffff80023e80}, {"_ZN4PROC7Process6GetPIDEv"}}, 
   {{0xffffffff80023e90}, {"_ZN4PROC7Process15GetProcessStateEv"}}, 
   {{0xffffffff80023ea0}, {"_ZN4PROC7Process21GetVirtualMemorySpaceEv"}}, 
   {{0xffffffff80023eb0}, {"_ZN4PROC7Process14GetHighestFreeEv"}}, 
   {{0xffffffff80023ec0}, {"_ZN4PROC7Process14SetHighestFreeEm"}}, 
   {{0xffffffff80023ed0}, {"_ZN4PROC6ThreadC1EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80023ed0}, {"_ZN4PROC6ThreadC2EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80024050}, {"_ZN4PROC7Process12CreateThreadEmm.part.0"}}, 
   {{0xffffffff80024140}, {"_ZN4PROC7Process12CreateThreadEmm"}}, 
   {{0xffffffff80024160}, {"_ZN4PROC7ProcessC1ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024160}, {"_ZN4PROC7ProcessC2ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024250}, {"_ZN4PROC6ThreadD1Ev"}}, 
   {{0xffffffff80024250}, {"_ZN4PROC6ThreadD2Ev"}}, 
   {{0xffffffff80024280}, {"_ZN4PROC6Thread8SetStateENS_12ProcessStateE"}}, 
   {{0xffffffff80024290}, {"_ZN4PROC6Thread8GetStateEv"}}, 
   {{0xffffffff800242a0}, {"_ZN4PROC6Thread6GetTIDEv"}}, 
   {{0xffffffff800242b0}, {"_ZN4PROC6Thread14SetInstructionEm"}}, 
   {{0xffffffff800242c0}, {"_ZN4PROC6Thread8SetStackEm"}}, 
   {{0xffffffff800242d0}, {"_ZN4PROC6Thread12GetStackBaseEv"}}, 
   {{0xffffffff800242e0}, {"_ZN4PROC6Thread8GetStackEv"}}, 
   {{0xffffffff800242f0}, {"_ZN4PROC6Thread12GetStackSizeEv"}}, 
   {{0xffffffff80024300}, {"_ZN4PROC9Scheduler7AddNodeEPNS_11ProcessNodeEPNS_7ProcessE"}}, 
   {{0xffffffff80024380}, {"_ZN4PROC9Scheduler12PopFirstNodeEPNS_11ProcessNodeE"}}, 
   {{0xffffffff800243c0}, {"_ZN4PROC9Scheduler10RemoveNodeEPNS_11ProcessNodeEm"}}, 
   {{0xffffffff80024440}, {"_ZN4PROC9Scheduler8FindNodeEPNS_11ProcessNodeEmPS2_Pb"}}, 
   {{0xffffffff800244c0}, {"_ZN4PROC9SchedulerC1Ev"}}, 
   {{0xffffffff800244c0}, {"_ZN4PROC9SchedulerC2Ev"}}, 
   {{0xffffffff800245b0}, {"_ZN4PROC9Scheduler10AddProcessEPNS_7ProcessE"}}, 
   {{0xffffffff80024610}, {"_ZN4PROC9Scheduler10GetProcessEmPNS_11ProcessNodeE"}}, 
   {{0xffffffff80024670}, {"_ZN4PROC9Scheduler10GetProcessEm"}}, 
   {{0xffffffff80024720}, {"_ZN4PROC9Scheduler15SetProcessStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff80024830}, {"_ZN4PROC9Scheduler17GetRunningProcessEv"}}, 
   {{0xffffffff80024840}, {"_ZN4PROC9Scheduler12SwitchToTaskEPNS_7ProcessEm"}}, 
   {{0xffffffff80024960}, {"_ZN4PROC9Scheduler20RecalculateSchedulerEv"}}, 
   {{0xffffffff800249d0}, {"_ZN6BitmapixEm"}}, 
   {{0xffffffff80024a00}, {"_ZN6Bitmap3GetEm"}}, 
   {{0xffffffff80024a30}, {"_ZN6Bitmap3SetEmb"}}, 
   {{0xffffffff80024a80}, {"_Z5IoctlP6Drivermz"}}, 
   {{0xffffffff80024ad0}, {"_Z18LoadProgramHeadersPhmP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024df0}, {"_Z11LinkSymbolsPhmP10Elf64_Ehdr"}}, 
   {{0xffffffff80024e00}, {"_Z11LoadProcessP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024eb0}, {"_Z7LoadELFPhm"}}, 
   {{0xffffffff80024f10}, {"_ZN4FILE4OpenEPcPm"}}, 
   {{0xffffffff800250a0}, {"__cxa_guard_acquire"}}, 
   {{0xffffffff800250b0}, {"__cxa_guard_release"}}, 
   {{0xffffffff800250c0}, {"__cxa_guard_abort"}}, 
   {{0xffffffff800250d0}, {"__cxa_atexit"}}, 
   {{0xffffffff80025120}, {"__cxa_finalize"}}, 
   {{0xffffffff800251d0}, {"_Z16RequestInterruptj"}}, 
   {{0xffffffff800251e0}, {"_Z18LoadExecutableFilePhm"}}, 
   {{0xffffffff80025220}, {"_Z12TryLockMutexPb"}}, 
   {{0xffffffff80025230}, {"_Z17SpinlockLockMutexPb"}}, 
   {{0xffffffff80025240}, {"_Z11UnlockMutexPb"}}, 
   {{0xffffffff80025250}, {"_Z5PanicPKcS0_S0_j"}}, 
   {{0xffffffff80025280}, {"_Z4OopsPKcS0_S0_j"}}, 
   {{0xffffffff800252b0}, {"_ZN6PRINTK11FlushBufferEv"}}, 
   {{0xffffffff800252f0}, {"_ZN6PRINTK7PutCharEc"}}, 
   {{0xffffffff800253b0}, {"_ZN6PRINTK6PutStrEPc"}}, 
   {{0xffffffff80025490}, {"_ZN6PRINTK7VPrintKEPcP13__va_list_tag"}}, 
   {{0xffffffff800256c0}, {"_ZN6PRINTK6PrintKEPcz"}}, 
   {{0xffffffff80025740}, {"_ZN6PRINTK9EarlyInitEv"}}, 
   {{0xffffffff800257b0}, {"_GLOBAL__sub_I_kernelPort"}}, 
   {{0xffffffff800257d0}, {"_Z11LoadShebangPhm"}}, 
   {{0xffffffff800257e0}, {"_Z12LookupSymbolPKc"}}, 
   {{0xffffffff80025850}, {"_Z12LookupSymbolm"}}, 
   {{0xffffffff800258c0}, {"_Z11AddOverridem"}}, 
   {{0xffffffff800258d0}, {"_Z13CheckOverridem"}}, 
   {{0xffffffff800258e0}, {"_Z11RunOverridem"}}, 
   {{0xffffffff800258f0}, {"_Z24HandleSyscallDebugPrintKPKc"}}, 
   {{0xffffffff80025910}, {"_Z26HandleSyscallMemoryGetinfom"}}, 
   {{0xffffffff80025970}, {"_Z26HandleSyscallMemoryVmallocmmm"}}, 
   {{0xffffffff80025a90}, {"_Z25HandleSyscallMemoryPallocPmm"}}, 
   {{0xffffffff80025b10}, {"_Z25HandleSyscallMemoryVmfreemm"}}, 
   {{0xffffffff80025c10}, {"_Z23HandleSyscallMemoryMmapmmmm"}}, 
   {{0xffffffff80025ce0}, {"_Z24HandleSyscallMemoryUnmapmm"}}, 
   {{0xffffffff80025dc0}, {"_Z24HandleSyscallMemoryInOutmbmPmh"}}, 
   {{0xffffffff80025ef0}, {"_Z21HandleSyscallProcExecmm"}}, 
   {{0xffffffff80026010}, {"_Z21HandleSyscallProcForkm"}}, 
   {{0xffffffff80026020}, {"_Z23HandleSyscallProcReturnmm"}}, 
   {{0xffffffff80026090}, {"_Z21HandleSyscallProcExitmm"}}, 
   {{0xffffffff800260d0}, {"_Z21HandleSyscallProcWaitm"}}, 
   {{0xffffffff800260e0}, {"_Z21HandleSyscallProcKillm"}}, 
   {{0xffffffff800260f0}, {"_Z27HandleSyscallModuleRegistermm"}}, 
   {{0xffffffff80026160}, {"_Z29HandleSyscallModuleUnregisterv"}}, 
   {{0xffffffff800261e0}, {"_Z31HandleSyscallModuleBufferCreatemmPj"}}, 
   {{0xffffffff80026290}, {"_Z28HandleSyscallModuleBufferMapmj"}}, 
   {{0xffffffff80026350}, {"_Z30HandleSyscallModuleBufferUnmapmj"}}, 
   {{0xffffffff80026360}, {"_Z31HandleSyscallModuleBufferDeletej"}}, 
   {{0xffffffff80026370}, {"_Z33HandleSyscallModuleMessageHandlerm"}}, 
   {{0xffffffff800263d0}, {"_Z30HandleSyscallModuleMessageSendjjPvm"}}, 
   {{0xffffffff80026610}, {"_Z34HandleSyscallModuleSectionRegisterPKc"}}, 
   {{0xffffffff80026700}, {"_Z29HandleSyscallModuleSectionGetPKcPjS1_"}}, 
   {{0xffffffff800267c0}, {"_Z36HandleSyscallModuleSectionUnregisterPKc"}}, 
   {{0xffffffff80026890}, {"_Z21HandleSyscallFileOpenPcPmS0_"}}, 
   {{0xffffffff80026930}, {"_Z21HandleSyscallFileReadPcmm"}}, 
   {{0xffffffff800269d0}, {"HandleSyscall"}}, 
   {{0xffffffff80026d60}, {"_Z22HandleSyscallFileWritem"}}, 
   {{0xffffffff80026d70}, {"_Z22HandleSyscallFileClosem"}}, 
   {{0xffffffff80026d80}, {"_Z25HandleSyscallKernOverridem"}}, 
   {{0xffffffff80026d90}, {"_ZN6x86_646x86CPU4InitEv"}}, 
   {{0xffffffff80026e90}, {"_ZN6x86_646x86CPU9GetVendorEPc"}}, 
   {{0xffffffff80026eb0}, {"_ZN6x86_647TSSInitEm"}}, 
   {{0xffffffff80026f50}, {"_ZN6x86_647LoadGDTEm"}}, 
   {{0xffffffff80026fa0}, {"_Z15InitializeStackPvm"}}, 
   {{0xffffffff80026fb0}, {"_ZN6x86_6411GetAPICBaseEv"}}, 
   {{0xffffffff80026fc0}, {"_ZN6x86_6411SetAPICBaseEm"}}, 
   {{0xffffffff80026fe0}, {"_ZN6x86_6417WriteAPICRegisterEtj"}}, 
   {{0xffffffff80027000}, {"_ZN6x86_6416ReadAPICRegisterEt"}}, 
   {{0xffffffff80027020}, {"_ZN6x86_648WaitAPICEj"}}, 
   {{0xffffffff80027040}, {"_ZN6x86_6410EnableAPICEv"}}, 
   {{0xffffffff80027110}, {"_ZN6x86_6411SendAPICEOIEv"}}, 
   {{0xffffffff80027130}, {"_Z20CalibrateTSCWithHPETmPm"}}, 
   {{0xffffffff80027200}, {"_ZN6x86_646IOAPIC5IoctlEmz"}}, 
   {{0xffffffff80027280}, {"_ZN6x86_646IOAPICC1Em"}}, 
   {{0xffffffff80027280}, {"_ZN6x86_646IOAPICC2Em"}}, 
   {{0xffffffff80027290}, {"_ZN6x86_646IOAPIC10ReadIOAPICEj"}}, 
   {{0xffffffff800272a0}, {"_ZN6x86_646IOAPIC11WriteIOAPICEjj"}}, 
   {{0xffffffff800272b0}, {"_ZN6x86_6410HandleMADTEP10MADTHeader"}}, 
   {{0xffffffff80027410}, {"_ZN6x86_6410HandleSRATEP9SDTHeader"}}, 
   {{0xffffffff80027460}, {"_ZN6x86_6410HandleHPETEP10HPETHeader"}}, 
   {{0xffffffff80027520}, {"_ZN6x86_6411InitDevicesEv"}}, 
   {{0xffffffff80027690}, {"_ZN6x86_643PIT5IoctlEmz"}}, 
   {{0xffffffff800276b0}, {"_ZN6x86_643PITC1Ev"}}, 
   {{0xffffffff800276b0}, {"_ZN6x86_643PITC2Ev"}}, 
   {{0xffffffff800276c0}, {"_ZN6x86_647IDTInitEv"}}, 
   {{0xffffffff80027770}, {"InterruptHandler"}}, 
   {{0xffffffff80027a10}, {"_ZN6x86_644OutBEth"}}, 
   {{0xffffffff80027a20}, {"_ZN6x86_644OutWEtt"}}, 
   {{0xffffffff80027a30}, {"_ZN6x86_643InBEt"}}, 
   {{0xffffffff80027a40}, {"_ZN6x86_643InWEt"}}, 
   {{0xffffffff80027a50}, {"_ZN6x86_646IoWaitEv"}}, 
   {{0xffffffff80027a60}, {"_ZN6x86_644InitEv"}}, 
   {{0xffffffff80027b20}, {"_ZN14PageMapIndexerC1Em"}}, 
   {{0xffffffff80027b20}, {"_ZN14PageMapIndexerC2Em"}}, 
   {{0xffffffff80027b60}, {"_ZN16PageTableManager9MapMemoryEPvS0_m"}}, 
   {{0xffffffff80027ec0}, {"_ZN16PageTableManager11UnmapMemoryEPv"}}, 
   {{0xffffffff80027fb0}, {"_ZN16PageTableManager18GetPhysicalAddressEPv"}}, 
   {{0xffffffff80028080}, {"_ZN16PageTableManager4ForkEPN3VMM12VirtualSpaceEb"}}, 
   {{0xffffffff80028450}, {"_ZN16PageTableManagerC1EP9PageTable"}}, 
   {{0xffffffff80028450}, {"_ZN16PageTableManagerC2EP9PageTable"}}, 
   {{0xffffffff80028460}, {"_ZN16PageTableManager13GetTopAddressEv"}}, 
   {{0xffffffff80028470}, {"_ZN18PageDirectoryEntry7SetFlagE7PT_Flagb"}}, 
   {{0xffffffff800284a0}, {"_ZN18PageDirectoryEntry7GetFlagE7PT_Flag"}}, 
   {{0xffffffff800284b0}, {"_ZN18PageDirectoryEntry10SetAddressEm"}}, 
   {{0xffffffff800284e0}, {"_ZN18PageDirectoryEntry10GetAddressEv"}}, 
   {{0xffffffff800284f0}, {"_ZN6x86_6415NewVirtualSpaceEv"}}, 
   {{0xffffffff80028530}, {"_ZN6x86_6416LoadVirtualSpaceEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80028560}, {"CPUPause"}}, 
   {{0xffffffff80028570}, {"FlushGDT"}}, 
   {{0xffffffff8002858a}, {"FlushTSS"}}, 
   {{0xffffffff800285a0}, {"EnableSCE"}}, 
   {{0xffffffff800285e0}, {"EnableSSE"}}, 
   {{0xffffffff800285fb}, {"EnableAVX"}}, 
   {{0xffffffff80028610}, {"SwitchStack"}}, 
   {{0xffffffff80028650}, {"EnterUserspace"}}, 
   {{0xffffffff80028660}, {"isr_stub_0"}}, 
   {{0xffffffff800286a1}, {"isr_stub_1"}}, 
   {{0xffffffff800286e2}, {"isr_stub_2"}}, 
   {{0xffffffff80028723}, {"isr_stub_3"}}, 
   {{0xffffffff80028764}, {"isr_stub_4"}}, 
   {{0xffffffff800287a5}, {"isr_stub_5"}}, 
   {{0xffffffff800287e6}, {"isr_stub_6"}}, 
   {{0xffffffff80028827}, {"isr_stub_7"}}, 
   {{0xffffffff80028868}, {"isr_stub_8"}}, 
   {{0xffffffff800288a7}, {"isr_stub_9"}}, 
   {{0xffffffff800288e8}, {"isr_stub_10"}}, 
   {{0xffffffff80028927}, {"isr_stub_11"}}, 
   {{0xffffffff80028966}, {"isr_stub_12"}}, 
   {{0xffffffff800289a5}, {"isr_stub_13"}}, 
   {{0xffffffff800289e4}, {"isr_stub_14"}}, 
   {{0xffffffff80028a23}, {"isr_stub_15"}}, 
   {{0xffffffff80028a64}, {"isr_stub_16"}}, 
   {{0xffffffff80028aa5}, {"isr_stub_17"}}, 
   {{0xffffffff80028ae4}, {"isr_stub_18"}}, 
   {{0xffffffff80028b25}, {"isr_stub_19"}}, 
   {{0xffffffff80028b66}, {"isr_stub_20"}}, 
   {{0xffffffff80028ba7}, {"isr_stub_21"}}, 
   {{0xffffffff80028be8}, {"isr_stub_22"}}, 
   {{0xffffffff80028c29}, {"isr_stub_23"}}, 
   {{0xffffffff80028c6a}, {"isr_stub_24"}}, 
   {{0xffffffff80028cab}, {"isr_stub_25"}}, 
   {{0xffffffff80028cec}, {"isr_stub_26"}}, 
   {{0xffffffff80028d2d}, {"isr_stub_27"}}, 
   {{0xffffffff80028d6e}, {"isr_stub_28"}}, 
   {{0xffffffff80028daf}, {"isr_stub_29"}}, 
   {{0xffffffff80028df0}, {"isr_stub_30"}}, 
   {{0xffffffff80028e2f}, {"isr_stub_31"}}, 
   {{0xffffffff80028e70}, {"isr_stub_32"}}, 
   {{0xffffffff80028eb1}, {"isr_stub_33"}}, 
   {{0xffffffff80028ef2}, {"isr_stub_34"}}, 
   {{0xffffffff80028f33}, {"isr_stub_35"}}, 
   {{0xffffffff80028f74}, {"isr_stub_36"}}, 
   {{0xffffffff80028fb5}, {"isr_stub_37"}}, 
   {{0xffffffff80028ff6}, {"isr_stub_38"}}, 
   {{0xffffffff80029037}, {"isr_stub_39"}}, 
   {{0xffffffff80029078}, {"isr_stub_40"}}, 
   {{0xffffffff800290b9}, {"isr_stub_41"}}, 
   {{0xffffffff800290fa}, {"isr_stub_42"}}, 
   {{0xffffffff8002913b}, {"isr_stub_43"}}, 
   {{0xffffffff8002917c}, {"isr_stub_44"}}, 
   {{0xffffffff800291bd}, {"isr_stub_45"}}, 
   {{0xffffffff800291fe}, {"isr_stub_46"}}, 
   {{0xffffffff8002923f}, {"isr_stub_47"}}, 
   {{0xffffffff80029280}, {"isr_stub_48"}}, 
   {{0xffffffff800292c1}, {"isr_stub_49"}}, 
   {{0xffffffff80029302}, {"isr_stub_50"}}, 
   {{0xffffffff80029343}, {"isr_stub_51"}}, 
   {{0xffffffff80029384}, {"isr_stub_52"}}, 
   {{0xffffffff800293c5}, {"isr_stub_53"}}, 
   {{0xffffffff80029406}, {"isr_stub_54"}}, 
   {{0xffffffff80029447}, {"isr_stub_55"}}, 
   {{0xffffffff80029488}, {"isr_stub_56"}}, 
   {{0xffffffff800294c9}, {"isr_stub_57"}}, 
   {{0xffffffff8002950a}, {"isr_stub_58"}}, 
   {{0xffffffff8002954b}, {"isr_stub_59"}}, 
   {{0xffffffff8002958c}, {"isr_stub_60"}}, 
   {{0xffffffff800295cd}, {"isr_stub_61"}}, 
   {{0xffffffff8002960e}, {"isr_stub_62"}}, 
   {{0xffffffff8002964f}, {"isr_stub_63"}}, 
   {{0xffffffff80029690}, {"isr_stub_64"}}, 
   {{0xffffffff800296d1}, {"isr_stub_65"}}, 
   {{0xffffffff80029712}, {"isr_stub_66"}}, 
   {{0xffffffff80029753}, {"isr_stub_67"}}, 
   {{0xffffffff80029794}, {"isr_stub_68"}}, 
   {{0xffffffff800297d5}, {"isr_stub_69"}}, 
   {{0xffffffff80029816}, {"isr_stub_70"}}, 
   {{0xffffffff80029857}, {"isr_stub_71"}}, 
   {{0xffffffff80029898}, {"isr_stub_72"}}, 
   {{0xffffffff800298d9}, {"isr_stub_73"}}, 
   {{0xffffffff8002991a}, {"isr_stub_74"}}, 
   {{0xffffffff8002995b}, {"isr_stub_75"}}, 
   {{0xffffffff8002999c}, {"isr_stub_76"}}, 
   {{0xffffffff800299dd}, {"isr_stub_77"}}, 
   {{0xffffffff80029a1e}, {"isr_stub_78"}}, 
   {{0xffffffff80029a5f}, {"isr_stub_79"}}, 
   {{0xffffffff80029aa0}, {"isr_stub_80"}}, 
   {{0xffffffff80029ae1}, {"isr_stub_81"}}, 
   {{0xffffffff80029b22}, {"isr_stub_82"}}, 
   {{0xffffffff80029b63}, {"isr_stub_83"}}, 
   {{0xffffffff80029ba4}, {"isr_stub_84"}}, 
   {{0xffffffff80029be5}, {"isr_stub_85"}}, 
   {{0xffffffff80029c26}, {"isr_stub_86"}}, 
   {{0xffffffff80029c67}, {"isr_stub_87"}}, 
   {{0xffffffff80029ca8}, {"isr_stub_88"}}, 
   {{0xffffffff80029ce9}, {"isr_stub_89"}}, 
   {{0xffffffff80029d2a}, {"isr_stub_90"}}, 
   {{0xffffffff80029d6b}, {"isr_stub_91"}}, 
   {{0xffffffff80029dac}, {"isr_stub_92"}}, 
   {{0xffffffff80029ded}, {"isr_stub_93"}}, 
   {{0xffffffff80029e2e}, {"isr_stub_94"}}, 
   {{0xffffffff80029e6f}, {"isr_stub_95"}}, 
   {{0xffffffff80029eb0}, {"isr_stub_96"}}, 
   {{0xffffffff80029ef1}, {"isr_stub_97"}}, 
   {{0xffffffff80029f32}, {"isr_stub_98"}}, 
   {{0xffffffff80029f73}, {"isr_stub_99"}}, 
   {{0xffffffff80029fb4}, {"isr_stub_100"}}, 
   {{0xffffffff80029ff5}, {"isr_stub_101"}}, 
   {{0xffffffff8002a036}, {"isr_stub_102"}}, 
   {{0xffffffff8002a077}, {"isr_stub_103"}}, 
   {{0xffffffff8002a0b8}, {"isr_stub_104"}}, 
   {{0xffffffff8002a0f9}, {"isr_stub_105"}}, 
   {{0xffffffff8002a13a}, {"isr_stub_106"}}, 
   {{0xffffffff8002a17b}, {"isr_stub_107"}}, 
   {{0xffffffff8002a1bc}, {"isr_stub_108"}}, 
   {{0xffffffff8002a1fd}, {"isr_stub_109"}}, 
   {{0xffffffff8002a23e}, {"isr_stub_110"}}, 
   {{0xffffffff8002a27f}, {"isr_stub_111"}}, 
   {{0xffffffff8002a2c0}, {"isr_stub_112"}}, 
   {{0xffffffff8002a301}, {"isr_stub_113"}}, 
   {{0xffffffff8002a342}, {"isr_stub_114"}}, 
   {{0xffffffff8002a383}, {"isr_stub_115"}}, 
   {{0xffffffff8002a3c4}, {"isr_stub_116"}}, 
   {{0xffffffff8002a405}, {"isr_stub_117"}}, 
   {{0xffffffff8002a446}, {"isr_stub_118"}}, 
   {{0xffffffff8002a487}, {"isr_stub_119"}}, 
   {{0xffffffff8002a4c8}, {"isr_stub_120"}}, 
   {{0xffffffff8002a509}, {"isr_stub_121"}}, 
   {{0xffffffff8002a54a}, {"isr_stub_122"}}, 
   {{0xffffffff8002a58b}, {"isr_stub_123"}}, 
   {{0xffffffff8002a5cc}, {"isr_stub_124"}}, 
   {{0xffffffff8002a60d}, {"isr_stub_125"}}, 
   {{0xffffffff8002a64e}, {"isr_stub_126"}}, 
   {{0xffffffff8002a68f}, {"isr_stub_127"}}, 
   {{0xffffffff8002a6d0}, {"isr_stub_128"}}, 
   {{0xffffffff8002a714}, {"isr_stub_129"}}, 
   {{0xffffffff8002a758}, {"isr_stub_130"}}, 
   {{0xffffffff8002a79c}, {"isr_stub_131"}}, 
   {{0xffffffff8002a7e0}, {"isr_stub_132"}}, 
   {{0xffffffff8002a824}, {"isr_stub_133"}}, 
   {{0xffffffff8002a868}, {"isr_stub_134"}}, 
   {{0xffffffff8002a8ac}, {"isr_stub_135"}}, 
   {{0xffffffff8002a8f0}, {"isr_stub_136"}}, 
   {{0xffffffff8002a934}, {"isr_stub_137"}}, 
   {{0xffffffff8002a978}, {"isr_stub_138"}}, 
   {{0xffffffff8002a9bc}, {"isr_stub_139"}}, 
   {{0xffffffff8002aa00}, {"isr_stub_140"}}, 
   {{0xffffffff8002aa44}, {"isr_stub_141"}}, 
   {{0xffffffff8002aa88}, {"isr_stub_142"}}, 
   {{0xffffffff8002aacc}, {"isr_stub_143"}}, 
   {{0xffffffff8002ab10}, {"isr_stub_144"}}, 
   {{0xffffffff8002ab54}, {"isr_stub_145"}}, 
   {{0xffffffff8002ab98}, {"isr_stub_146"}}, 
   {{0xffffffff8002abdc}, {"isr_stub_147"}}, 
   {{0xffffffff8002ac20}, {"isr_stub_148"}}, 
   {{0xffffffff8002ac64}, {"isr_stub_149"}}, 
   {{0xffffffff8002aca8}, {"isr_stub_150"}}, 
   {{0xffffffff8002acec}, {"isr_stub_151"}}, 
   {{0xffffffff8002ad30}, {"isr_stub_152"}}, 
   {{0xffffffff8002ad74}, {"isr_stub_153"}}, 
   {{0xffffffff8002adb8}, {"isr_stub_154"}}, 
   {{0xffffffff8002adfc}, {"isr_stub_155"}}, 
   {{0xffffffff8002ae40}, {"isr_stub_156"}}, 
   {{0xffffffff8002ae84}, {"isr_stub_157"}}, 
   {{0xffffffff8002aec8}, {"isr_stub_158"}}, 
   {{0xffffffff8002af0c}, {"isr_stub_159"}}, 
   {{0xffffffff8002af50}, {"isr_stub_160"}}, 
   {{0xffffffff8002af94}, {"isr_stub_161"}}, 
   {{0xffffffff8002afd8}, {"isr_stub_162"}}, 
   {{0xffffffff8002b01c}, {"isr_stub_163"}}, 
   {{0xffffffff8002b060}, {"isr_stub_164"}}, 
   {{0xffffffff8002b0a4}, {"isr_stub_165"}}, 
   {{0xffffffff8002b0e8}, {"isr_stub_166"}}, 
   {{0xffffffff8002b12c}, {"isr_stub_167"}}, 
   {{0xffffffff8002b170}, {"isr_stub_168"}}, 
   {{0xffffffff8002b1b4}, {"isr_stub_169"}}, 
   {{0xffffffff8002b1f8}, {"isr_stub_170"}}, 
   {{0xffffffff8002b23c}, {"isr_stub_171"}}, 
   {{0xffffffff8002b280}, {"isr_stub_172"}}, 
   {{0xffffffff8002b2c4}, {"isr_stub_173"}}, 
   {{0xffffffff8002b308}, {"isr_stub_174"}}, 
   {{0xffffffff8002b34c}, {"isr_stub_175"}}, 
   {{0xffffffff8002b390}, {"isr_stub_176"}}, 
   {{0xffffffff8002b3d4}, {"isr_stub_177"}}, 
   {{0xffffffff8002b418}, {"isr_stub_178"}}, 
   {{0xffffffff8002b45c}, {"isr_stub_179"}}, 
   {{0xffffffff8002b4a0}, {"isr_stub_180"}}, 
   {{0xffffffff8002b4e4}, {"isr_stub_181"}}, 
   {{0xffffffff8002b528}, {"isr_stub_182"}}, 
   {{0xffffffff8002b56c}, {"isr_stub_183"}}, 
   {{0xffffffff8002b5b0}, {"isr_stub_184"}}, 
   {{0xffffffff8002b5f4}, {"isr_stub_185"}}, 
   {{0xffffffff8002b638}, {"isr_stub_186"}}, 
   {{0xffffffff8002b67c}, {"isr_stub_187"}}, 
   {{0xffffffff8002b6c0}, {"isr_stub_188"}}, 
   {{0xffffffff8002b704}, {"isr_stub_189"}}, 
   {{0xffffffff8002b748}, {"isr_stub_190"}}, 
   {{0xffffffff8002b78c}, {"isr_stub_191"}}, 
   {{0xffffffff8002b7d0}, {"isr_stub_192"}}, 
   {{0xffffffff8002b814}, {"isr_stub_193"}}, 
   {{0xffffffff8002b858}, {"isr_stub_194"}}, 
   {{0xffffffff8002b89c}, {"isr_stub_195"}}, 
   {{0xffffffff8002b8e0}, {"isr_stub_196"}}, 
   {{0xffffffff8002b924}, {"isr_stub_197"}}, 
   {{0xffffffff8002b968}, {"isr_stub_198"}}, 
   {{0xffffffff8002b9ac}, {"isr_stub_199"}}, 
   {{0xffffffff8002b9f0}, {"isr_stub_200"}}, 
   {{0xffffffff8002ba34}, {"isr_stub_201"}}, 
   {{0xffffffff8002ba78}, {"isr_stub_202"}}, 
   {{0xffffffff8002babc}, {"isr_stub_203"}}, 
   {{0xffffffff8002bb00}, {"isr_stub_204"}}, 
   {{0xffffffff8002bb44}, {"isr_stub_205"}}, 
   {{0xffffffff8002bb88}, {"isr_stub_206"}}, 
   {{0xffffffff8002bbcc}, {"isr_stub_207"}}, 
   {{0xffffffff8002bc10}, {"isr_stub_208"}}, 
   {{0xffffffff8002bc54}, {"isr_stub_209"}}, 
   {{0xffffffff8002bc98}, {"isr_stub_210"}}, 
   {{0xffffffff8002bcdc}, {"isr_stub_211"}}, 
   {{0xffffffff8002bd20}, {"isr_stub_212"}}, 
   {{0xffffffff8002bd64}, {"isr_stub_213"}}, 
   {{0xffffffff8002bda8}, {"isr_stub_214"}}, 
   {{0xffffffff8002bdec}, {"isr_stub_215"}}, 
   {{0xffffffff8002be30}, {"isr_stub_216"}}, 
   {{0xffffffff8002be74}, {"isr_stub_217"}}, 
   {{0xffffffff8002beb8}, {"isr_stub_218"}}, 
   {{0xffffffff8002befc}, {"isr_stub_219"}}, 
   {{0xffffffff8002bf40}, {"isr_stub_220"}}, 
   {{0xffffffff8002bf84}, {"isr_stub_221"}}, 
   {{0xffffffff8002bfc8}, {"isr_stub_222"}}, 
   {{0xffffffff8002c00c}, {"isr_stub_223"}}, 
   {{0xffffffff8002c050}, {"isr_stub_224"}}, 
   {{0xffffffff8002c094}, {"isr_stub_225"}}, 
   {{0xffffffff8002c0d8}, {"isr_stub_226"}}, 
   {{0xffffffff8002c11c}, {"isr_stub_227"}}, 
   {{0xffffffff8002c160}, {"isr_stub_228"}}, 
   {{0xffffffff8002c1a4}, {"isr_stub_229"}}, 
   {{0xffffffff8002c1e8}, {"isr_stub_230"}}, 
   {{0xffffffff8002c22c}, {"isr_stub_231"}}, 
   {{0xffffffff8002c270}, {"isr_stub_232"}}, 
   {{0xffffffff8002c2b4}, {"isr_stub_233"}}, 
   {{0xffffffff8002c2f8}, {"isr_stub_234"}}, 
   {{0xffffffff8002c33c}, {"isr_stub_235"}}, 
   {{0xffffffff8002c380}, {"isr_stub_236"}}, 
   {{0xffffffff8002c3c4}, {"isr_stub_237"}}, 
   {{0xffffffff8002c408}, {"isr_stub_238"}}, 
   {{0xffffffff8002c44c}, {"isr_stub_239"}}, 
   {{0xffffffff8002c490}, {"isr_stub_240"}}, 
   {{0xffffffff8002c4d4}, {"isr_stub_241"}}, 
   {{0xffffffff8002c518}, {"isr_stub_242"}}, 
   {{0xffffffff8002c55c}, {"isr_stub_243"}}, 
   {{0xffffffff8002c5a0}, {"isr_stub_244"}}, 
   {{0xffffffff8002c5e4}, {"isr_stub_245"}}, 
   {{0xffffffff8002c628}, {"isr_stub_246"}}, 
   {{0xffffffff8002c66c}, {"isr_stub_247"}}, 
   {{0xffffffff8002c6b0}, {"isr_stub_248"}}, 
   {{0xffffffff8002c6f4}, {"isr_stub_249"}}, 
   {{0xffffffff8002c738}, {"isr_stub_250"}}, 
   {{0xffffffff8002c77c}, {"isr_stub_251"}}, 
   {{0xffffffff8002c7c0}, {"isr_stub_252"}}, 
   {{0xffffffff8002c804}, {"isr_stub_253"}}, 
   {{0xffffffff8002c848}, {"isr_stub_254"}}, 
   {{0xffffffff8002c88c}, {"isr_stub_255"}}, 
   {{0xffffffff8002c8d0}, {"isrStubTable"}}, 
   {{0xffffffff8002e408}, {"_ZTV10UARTDevice"}}, 
   {{0xffffffff8002e6f0}, {"_ZTV8SpinLock"}}, 
   {{0xffffffff8002e7c0}, {"_ZL14memTypeStrings"}}, 
   {{0xffffffff80031f40}, {"symbols"}}, 
   {{0xffffffff800344f0}, {"symbolCount"}}, 
   {{0xffffffff80034628}, {"_ZTVN6x86_646IOAPICE"}}, 
   {{0xffffffff80034a88}, {"_ZTVN6x86_643PITE"}}, 
   {{0xffffffff80034e68}, {"_ZTV16PageTableManager"}}, 
   {{0xffffffff80036000}, {"_ZL17kernelFileRequest"}}, 
   {{0xffffffff80036040}, {"_ZL18framebufferRequest"}}, 
   {{0xffffffff80036080}, {"_ZL11rsdpRequest"}}, 
   {{0xffffffff800360c0}, {"_ZL12kaddrRequest"}}, 
   {{0xffffffff80036100}, {"_ZL11mMapRequest"}}, 
   {{0xffffffff80036140}, {"_ZL11hhdmRequest"}}, 
   {{0xffffffff80036180}, {"_ZL13moduleRequest"}}, 
   {{0xffffffff800361c0}, {"_ZL12stackRequest"}}, 
   {{0xffffffff80036200}, {"_ZL17entryPointRequest"}}, 
   {{0xffffffff80036238}, {"__stack_chk_guard"}}, 
   {{0xffffffff80036240}, {"TypeCheckKinds"}}, 
   {{0xffffffff80036290}, {"bootmemStatus"}}, 
   {{0xffffffff80036298}, {"_ZN4PROCL10CurrentPIDE"}}, 
   {{0xffffffff800362a0}, {"StartSyscallStack"}}, 
   {{0xffffffff80037000}, {"gdt"}}, 
   {{0xffffffff80038000}, {"info"}}, 
   {{0xffffffff80038010}, {"RestStack"}}, 
   {{0xffffffff80039010}, {"lastPosition"}}, 
   {{0xffffffff80039020}, {"bootmemMemory"}}, 
   {{0xffffffff8003a020}, {"_ZL8HeapLock"}}, 
   {{0xffffffff8003a030}, {"_ZL8totalMem"}}, 
   {{0xffffffff8003a038}, {"_ZL7freeMem"}}, 
   {{0xffffffff8003a040}, {"_ZL12isHeapActive"}}, 
   {{0xffffffff8003a048}, {"_ZL10lastHeader"}}, 
   {{0xffffffff8003a050}, {"_ZL7heapEnd"}}, 
   {{0xffffffff8003a058}, {"_ZL9heapStart"}}, 
   {{0xffffffff8003a060}, {"_ZL10BitmapLock"}}, 
   {{0xffffffff8003a070}, {"_ZL17page_bitmap_index"}}, 
   {{0xffffffff8003a080}, {"_ZL11page_bitmap"}}, 
   {{0xffffffff8003a090}, {"_ZL11initialized"}}, 
   {{0xffffffff8003a098}, {"_ZL11used_memory"}}, 
   {{0xffffffff8003a0a0}, {"_ZL15reserved_memory"}}, 
   {{0xffffffff8003a0a8}, {"_ZL11free_memory"}}, 
   {{0xffffffff8003a0b0}, {"_ZZ6strtokPcS_E1p"}}, 
   {{0xffffffff8003a0c0}, {"__dso_handle"}}, 
   {{0xffffffff8003a0c8}, {"__atexit_func_count"}}, 
   {{0xffffffff8003a0e0}, {"__atexit_funcs"}}, 
   {{0xffffffff8003ace0}, {"kernelPort"}}, 
   {{0xffffffff8003ace8}, {"_ZN6PRINTKL16TerminalPositionE"}}, 
   {{0xffffffff8003ad00}, {"_ZN6PRINTKL14TerminalColumnE"}}, 
   {{0xffffffff8003ae00}, {"_ZN6PRINTKL14PrintKSpinlockE"}}, 
   {{0xffffffff8003b000}, {"gdtPointer"}}, 
   {{0xffffffff8003c000}, {"tss"}}, 
   {{0xffffffff8003c070}, {"idtr"}}, 
   {{0xffffffff8003c080}, {"idt"}}, 
};
