#include <sys/symbol.hpp>
const uint64_t symbolCount = 600;
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
   {{0xffffffff80021690}, {"_ZN4ACPI7GetXSDTEv"}}, 
   {{0xffffffff800216a0}, {"_ZN4ACPI4InitEv"}}, 
   {{0xffffffff80021700}, {"_ZN4ACPI9FindTableEPNS_9SDTHeaderEPc"}}, 
   {{0xffffffff80021770}, {"_ZN10UARTDevice4InitE11SerialPorts"}}, 
   {{0xffffffff80021870}, {"_ZN10UARTDevice5IoctlEmP13__va_list_tag"}}, 
   {{0xffffffff80021a20}, {"_ZN10UARTDevice6PutStrEPKc"}}, 
   {{0xffffffff80021aa0}, {"_ZN10UARTDevice7PutCharEc"}}, 
   {{0xffffffff80021af0}, {"_ZN10UARTDevice7GetCharEv"}}, 
   {{0xffffffff80021b30}, {"_ZN10UARTDevice15isTransmitEmptyEv"}}, 
   {{0xffffffff80021b50}, {"_ZN10UARTDevice14serialReceivedEv"}}, 
   {{0xffffffff80021b70}, {"_Z9ParseArgsv"}}, 
   {{0xffffffff80021c30}, {"_Z8InitInfov"}}, 
   {{0xffffffff80021c50}, {"_Z7GetInfov"}}, 
   {{0xffffffff80021c60}, {"_Z15EarlyKernelInitv"}}, 
   {{0xffffffff80021c80}, {"_Z11PrintBannerv"}}, 
   {{0xffffffff80021d30}, {"_Z11KernelStartv"}}, 
   {{0xffffffff80021e90}, {"_Z8RestInitv"}}, 
   {{0xffffffff80021ec0}, {"_ZN7BOOTMEM15BootmemIsActiveEv"}}, 
   {{0xffffffff80021ed0}, {"_ZN7BOOTMEM6MallocEm"}}, 
   {{0xffffffff80021f00}, {"_ZN7BOOTMEM4FreeEPv"}}, 
   {{0xffffffff80021f10}, {"_ZN7BOOTMEM7GetFreeEv"}}, 
   {{0xffffffff80021f20}, {"_ZN7BOOTMEM8GetTotalEv"}}, 
   {{0xffffffff80021f26}, {"_ZN7BOOTMEM17DeactivateBootmemEv"}}, 
   {{0xffffffff80021f2d}, {"_ZN7BOOTMEM6MallocEm.cold"}}, 
   {{0xffffffff80021f40}, {"_ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff80021f50}, {"_ZN4HEAP7GetFreeEv"}}, 
   {{0xffffffff80021f60}, {"_ZN4HEAP8GetTotalEv"}}, 
   {{0xffffffff80021f70}, {"_ZN13HeapSegHeader14CombineForwardEv"}}, 
   {{0xffffffff80021fd0}, {"_ZN13HeapSegHeader15CombineBackwardEv"}}, 
   {{0xffffffff80021ff0}, {"_ZN13HeapSegHeader5SplitEm"}}, 
   {{0xffffffff80022060}, {"_ZN4HEAP14InitializeHeapEPvm"}}, 
   {{0xffffffff80022130}, {"_ZN4HEAP4FreeEPv"}}, 
   {{0xffffffff800221d0}, {"_ZN4HEAP10ExpandHeapEm"}}, 
   {{0xffffffff800222c0}, {"_ZN4HEAP6MallocEm"}}, 
   {{0xffffffff80022370}, {"_ZN8SpinLock6UnlockEv"}}, 
   {{0xffffffff80022380}, {"_ZN8SpinLock8IsLockedEv"}}, 
   {{0xffffffff80022390}, {"_ZN8SpinLock4LockEv"}}, 
   {{0xffffffff800223d0}, {"_GLOBAL__sub_I__ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff800223f0}, {"_Z6memcmpPKvS0_m"}}, 
   {{0xffffffff80022430}, {"_Z6memcpyPvS_m"}}, 
   {{0xffffffff80022460}, {"_Z6Mallocm"}}, 
   {{0xffffffff800224b0}, {"_Z4FreePv"}}, 
   {{0xffffffff800224c0}, {"_Znwm"}}, 
   {{0xffffffff80022510}, {"_Znam"}}, 
   {{0xffffffff80022560}, {"_ZdlPv"}}, 
   {{0xffffffff80022570}, {"_ZdlPvm"}}, 
   {{0xffffffff80022580}, {"_ZdaPv"}}, 
   {{0xffffffff80022590}, {"_ZdaPvm"}}, 
   {{0xffffffff800225a0}, {"_ZN3MEM10DisplayRamEv"}}, 
   {{0xffffffff80022620}, {"_ZN3MEM4InitEv"}}, 
   {{0xffffffff80022630}, {"_Z6memsetPvhm"}}, 
   {{0xffffffff80022680}, {"_ZL12ReservePagesPvm"}}, 
   {{0xffffffff80022700}, {"_ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022990}, {"_ZN3PMM8FreePageEPv"}}, 
   {{0xffffffff80022a10}, {"_ZN3PMM8LockPageEPv"}}, 
   {{0xffffffff80022a90}, {"_ZN3PMM11RequestPageEv"}}, 
   {{0xffffffff80022b60}, {"_ZN3PMM12RequestPagesEm"}}, 
   {{0xffffffff80022c80}, {"_ZN3PMM9FreePagesEPvm"}}, 
   {{0xffffffff80022cd0}, {"_ZN3PMM9LockPagesEPvm"}}, 
   {{0xffffffff80022d20}, {"_ZN3PMM10GetFreeMemEv"}}, 
   {{0xffffffff80022d30}, {"_ZN3PMM10GetUsedMemEv"}}, 
   {{0xffffffff80022d40}, {"_ZN3PMM14GetReservedMemEv"}}, 
   {{0xffffffff80022d50}, {"_GLOBAL__sub_I__ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022d70}, {"_Z6strlenPKc"}}, 
   {{0xffffffff80022da0}, {"_Z6strcpyPcPKc"}}, 
   {{0xffffffff80022dd0}, {"_Z6strcmpPKcS0_"}}, 
   {{0xffffffff80022e20}, {"_Z8is_delimcPc"}}, 
   {{0xffffffff80022e60}, {"_Z6strtokPcS_"}}, 
   {{0xffffffff80022f30}, {"_Z4itoaPcix"}}, 
   {{0xffffffff80022fd0}, {"_Z4atoiPc"}}, 
   {{0xffffffff80023010}, {"_ZN3VMM21NewModuleVirtualSpaceEv"}}, 
   {{0xffffffff800231c0}, {"_ZN3VMM21NewKernelVirtualSpaceEv"}}, 
   {{0xffffffff800232f0}, {"_ZN3VMM7InitVMMEv"}}, 
   {{0xffffffff80023330}, {"_ZN3VMM15NewVirtualSpaceEv"}}, 
   {{0xffffffff80023340}, {"_ZN3VMM16LoadVirtualSpaceEPNS_12VirtualSpaceE"}}, 
   {{0xffffffff80023350}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_"}}, 
   {{0xffffffff80023360}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_m"}}, 
   {{0xffffffff80023370}, {"_ZN6MODULE13BufferManagerC1Ev"}}, 
   {{0xffffffff80023370}, {"_ZN6MODULE13BufferManagerC2Ev"}}, 
   {{0xffffffff800233b0}, {"_ZN6MODULE13BufferManager7AddNodeEPNS_6BufferE"}}, 
   {{0xffffffff80023420}, {"_ZN6MODULE13BufferManager10RemoveNodeEj"}}, 
   {{0xffffffff80023490}, {"_ZN6MODULE13BufferManager8FindNodeEjPPNS_10BufferNodeEPb"}}, 
   {{0xffffffff800234d0}, {"_ZN6MODULE13BufferManager12CreateBufferEjjNS_10BufferTypeEm"}}, 
   {{0xffffffff80023580}, {"_ZN6MODULE13BufferManager9GetBufferEj"}}, 
   {{0xffffffff800235b0}, {"_ZN6MODULE13BufferManager9MapBufferEjjjPN3VMM12VirtualSpaceEm"}}, 
   {{0xffffffff800236e0}, {"_ZN6MODULE13BufferManager12DeleteBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023740}, {"_ZN6MODULE13BufferManager10LockBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023770}, {"_ZN6MODULE13BufferManager12UnlockBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023790}, {"_ZN6MODULE14ComposeMessageEPNS_7MessageEjjm"}}, 
   {{0xffffffff80023810}, {"_ZN6MODULE6ModuleC1EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff80023810}, {"_ZN6MODULE6ModuleC2EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff80023820}, {"_ZN6MODULE7ManagerC1Ev"}}, 
   {{0xffffffff80023820}, {"_ZN6MODULE7ManagerC2Ev"}}, 
   {{0xffffffff80023850}, {"_ZN6MODULE7Manager7AddNodeEjj"}}, 
   {{0xffffffff800238c0}, {"_ZN6MODULE7Manager10RemoveNodeEjj"}}, 
   {{0xffffffff80023930}, {"_ZN6MODULE7Manager8FindNodeEjjPPNS_10ModuleNodeEPb"}}, 
   {{0xffffffff80023980}, {"_ZN6MODULE7Manager14RegisterModuleEPN4PROC7ProcessEjj"}}, 
   {{0xffffffff80023a50}, {"_ZN6MODULE7Manager9GetModuleEjj"}}, 
   {{0xffffffff80023a90}, {"_ZN6MODULE7Manager9GetModuleEm"}}, 
   {{0xffffffff80023ae0}, {"_ZN6MODULE7Manager16UnregisterModuleEjj"}}, 
   {{0xffffffff80023af0}, {"_ZN6MODULE14SectionManagerC1Ev"}}, 
   {{0xffffffff80023af0}, {"_ZN6MODULE14SectionManagerC2Ev"}}, 
   {{0xffffffff80023b30}, {"_ZN6MODULE14SectionManager8FindNodeEPKcjjPPNS_11SectionNodeEPb"}}, 
   {{0xffffffff80023bf0}, {"_ZN6MODULE14SectionManager7AddNodeEPKcjj"}}, 
   {{0xffffffff80023c50}, {"_ZN6MODULE14SectionManager10RemoveNodeEPKcjj"}}, 
   {{0xffffffff80023ca0}, {"_ZN6MODULE14SectionManager21RegisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023d60}, {"_ZN6MODULE14SectionManager16GetSectionDriverEPKcPjS3_"}}, 
   {{0xffffffff80023dc0}, {"_ZN6MODULE14SectionManager23UnregisterSectionDriverEPKcjj"}}, 
   {{0xffffffff80023e10}, {"_ZN4PROC7ProcessD1Ev"}}, 
   {{0xffffffff80023e10}, {"_ZN4PROC7ProcessD2Ev"}}, 
   {{0xffffffff80023e60}, {"_ZN4PROC7Process9GetThreadEm"}}, 
   {{0xffffffff80023e80}, {"_ZN4PROC7Process10RequestTIDEv"}}, 
   {{0xffffffff80023e90}, {"_ZN4PROC7Process13DestroyThreadEPNS_6ThreadE"}}, 
   {{0xffffffff80023ea0}, {"_ZN4PROC7Process14SetThreadStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff80023ed0}, {"_ZN4PROC7Process14GetThreadStateEm"}}, 
   {{0xffffffff80023f00}, {"_ZN4PROC7Process13SetMainThreadEm"}}, 
   {{0xffffffff80023f30}, {"_ZN4PROC7Process13GetMainThreadEv"}}, 
   {{0xffffffff80023f40}, {"_ZN4PROC7Process16SetMessageThreadEPv"}}, 
   {{0xffffffff80023f50}, {"_ZN4PROC7Process16GetMessageThreadEv"}}, 
   {{0xffffffff80023f60}, {"_ZN4PROC7Process11SetPriorityEh"}}, 
   {{0xffffffff80023f70}, {"_ZN4PROC7Process15SetProcessStateENS_12ProcessStateE"}}, 
   {{0xffffffff80023fb0}, {"_ZN4PROC7Process6GetPIDEv"}}, 
   {{0xffffffff80023fc0}, {"_ZN4PROC7Process15GetProcessStateEv"}}, 
   {{0xffffffff80023fd0}, {"_ZN4PROC7Process21GetVirtualMemorySpaceEv"}}, 
   {{0xffffffff80023fe0}, {"_ZN4PROC7Process14GetHighestFreeEv"}}, 
   {{0xffffffff80023ff0}, {"_ZN4PROC7Process14SetHighestFreeEm"}}, 
   {{0xffffffff80024000}, {"_ZN4PROC6ThreadC1EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80024000}, {"_ZN4PROC6ThreadC2EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff80024180}, {"_ZN4PROC7Process12CreateThreadEmm.part.0"}}, 
   {{0xffffffff80024270}, {"_ZN4PROC7Process12CreateThreadEmm"}}, 
   {{0xffffffff80024290}, {"_ZN4PROC7ProcessC1ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024290}, {"_ZN4PROC7ProcessC2ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024380}, {"_ZN4PROC6ThreadD1Ev"}}, 
   {{0xffffffff80024380}, {"_ZN4PROC6ThreadD2Ev"}}, 
   {{0xffffffff800243b0}, {"_ZN4PROC6Thread8SetStateENS_12ProcessStateE"}}, 
   {{0xffffffff800243c0}, {"_ZN4PROC6Thread8GetStateEv"}}, 
   {{0xffffffff800243d0}, {"_ZN4PROC6Thread6GetTIDEv"}}, 
   {{0xffffffff800243e0}, {"_ZN4PROC6Thread14SetInstructionEm"}}, 
   {{0xffffffff800243f0}, {"_ZN4PROC6Thread8SetStackEm"}}, 
   {{0xffffffff80024400}, {"_ZN4PROC6Thread12GetStackBaseEv"}}, 
   {{0xffffffff80024410}, {"_ZN4PROC6Thread8GetStackEv"}}, 
   {{0xffffffff80024420}, {"_ZN4PROC6Thread12GetStackSizeEv"}}, 
   {{0xffffffff80024430}, {"_ZN4PROC9Scheduler7AddNodeEPNS_11ProcessNodeEPNS_7ProcessE"}}, 
   {{0xffffffff800244b0}, {"_ZN4PROC9Scheduler12PopFirstNodeEPNS_11ProcessNodeE"}}, 
   {{0xffffffff800244f0}, {"_ZN4PROC9Scheduler10RemoveNodeEPNS_11ProcessNodeEm"}}, 
   {{0xffffffff80024570}, {"_ZN4PROC9Scheduler8FindNodeEPNS_11ProcessNodeEmPS2_Pb"}}, 
   {{0xffffffff800245f0}, {"_ZN4PROC9SchedulerC1Ev"}}, 
   {{0xffffffff800245f0}, {"_ZN4PROC9SchedulerC2Ev"}}, 
   {{0xffffffff800246e0}, {"_ZN4PROC9Scheduler10AddProcessEPNS_7ProcessE"}}, 
   {{0xffffffff80024740}, {"_ZN4PROC9Scheduler10GetProcessEmPNS_11ProcessNodeE"}}, 
   {{0xffffffff800247a0}, {"_ZN4PROC9Scheduler10GetProcessEm"}}, 
   {{0xffffffff80024850}, {"_ZN4PROC9Scheduler15SetProcessStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff80024960}, {"_ZN4PROC9Scheduler17GetRunningProcessEv"}}, 
   {{0xffffffff80024970}, {"_ZN4PROC9Scheduler12SwitchToTaskEPNS_7ProcessEm"}}, 
   {{0xffffffff80024a90}, {"_ZN4PROC9Scheduler20RecalculateSchedulerEv"}}, 
   {{0xffffffff80024b00}, {"_ZN6BitmapixEm"}}, 
   {{0xffffffff80024b30}, {"_ZN6Bitmap3GetEm"}}, 
   {{0xffffffff80024b60}, {"_ZN6Bitmap3SetEmb"}}, 
   {{0xffffffff80024bb0}, {"_Z5IoctlP6Drivermz"}}, 
   {{0xffffffff80024c00}, {"_Z18LoadProgramHeadersPhmP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024f30}, {"_Z11LinkSymbolsPhmP10Elf64_Ehdr"}}, 
   {{0xffffffff80024f50}, {"_Z11LoadProcessP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80025000}, {"_Z7LoadELFPhm"}}, 
   {{0xffffffff80025070}, {"_ZN4FILE4OpenEPcPm"}}, 
   {{0xffffffff80025200}, {"__cxa_guard_acquire"}}, 
   {{0xffffffff80025210}, {"__cxa_guard_release"}}, 
   {{0xffffffff80025220}, {"__cxa_guard_abort"}}, 
   {{0xffffffff80025230}, {"__cxa_atexit"}}, 
   {{0xffffffff80025280}, {"__cxa_finalize"}}, 
   {{0xffffffff80025330}, {"_Z16RequestInterruptj"}}, 
   {{0xffffffff80025340}, {"_Z18LoadExecutableFilePhm"}}, 
   {{0xffffffff80025350}, {"_Z12TryLockMutexPb"}}, 
   {{0xffffffff80025360}, {"_Z17SpinlockLockMutexPb"}}, 
   {{0xffffffff80025370}, {"_Z11UnlockMutexPb"}}, 
   {{0xffffffff80025380}, {"_Z5PanicPKcS0_S0_j"}}, 
   {{0xffffffff800253b0}, {"_Z4OopsPKcS0_S0_j"}}, 
   {{0xffffffff800253e0}, {"_ZN6PRINTK11FlushBufferEv"}}, 
   {{0xffffffff80025420}, {"_ZN6PRINTK7PutCharEc"}}, 
   {{0xffffffff80025500}, {"_ZN6PRINTK6PutStrEPc"}}, 
   {{0xffffffff800255f0}, {"_ZN6PRINTK7VPrintKEPcP13__va_list_tag"}}, 
   {{0xffffffff80025820}, {"_ZN6PRINTK6PrintKEPcz"}}, 
   {{0xffffffff800258a0}, {"_ZN6PRINTK9EarlyInitEv"}}, 
   {{0xffffffff80025910}, {"_GLOBAL__sub_I_kernelPort"}}, 
   {{0xffffffff80025930}, {"_Z12LookupSymbolPKc"}}, 
   {{0xffffffff800259a0}, {"_Z12LookupSymbolm"}}, 
   {{0xffffffff80025a10}, {"_Z11AddOverridem"}}, 
   {{0xffffffff80025a20}, {"_Z13CheckOverridem"}}, 
   {{0xffffffff80025a30}, {"_Z11RunOverridem"}}, 
   {{0xffffffff80025a40}, {"_Z24HandleSyscallDebugPrintKPKc"}}, 
   {{0xffffffff80025a60}, {"_Z26HandleSyscallMemoryGetinfom"}}, 
   {{0xffffffff80025ac0}, {"_Z26HandleSyscallMemoryVmallocmmm"}}, 
   {{0xffffffff80025be0}, {"_Z25HandleSyscallMemoryVmfreemm"}}, 
   {{0xffffffff80025ce0}, {"_Z23HandleSyscallMemoryMmapmmmm"}}, 
   {{0xffffffff80025db0}, {"_Z24HandleSyscallMemoryUnmapmm"}}, 
   {{0xffffffff80025e90}, {"_Z24HandleSyscallMemoryInOutmbmPmh"}}, 
   {{0xffffffff80025fc0}, {"_Z21HandleSyscallProcExecmm"}}, 
   {{0xffffffff800260e0}, {"_Z21HandleSyscallProcForkm"}}, 
   {{0xffffffff800260f0}, {"_Z23HandleSyscallProcReturnmm"}}, 
   {{0xffffffff80026160}, {"_Z21HandleSyscallProcExitmm"}}, 
   {{0xffffffff800261a0}, {"_Z21HandleSyscallProcWaitm"}}, 
   {{0xffffffff800261b0}, {"_Z21HandleSyscallProcKillm"}}, 
   {{0xffffffff800261c0}, {"_Z27HandleSyscallModuleRegistermm"}}, 
   {{0xffffffff80026230}, {"_Z29HandleSyscallModuleUnregisterv"}}, 
   {{0xffffffff800262b0}, {"_Z31HandleSyscallModuleBufferCreatemmPj"}}, 
   {{0xffffffff80026360}, {"_Z28HandleSyscallModuleBufferMapmj"}}, 
   {{0xffffffff80026420}, {"_Z30HandleSyscallModuleBufferUnmapmj"}}, 
   {{0xffffffff80026430}, {"_Z31HandleSyscallModuleBufferDeletej"}}, 
   {{0xffffffff80026440}, {"_Z33HandleSyscallModuleMessageHandlerm"}}, 
   {{0xffffffff800264a0}, {"_Z30HandleSyscallModuleMessageSendjjPvm"}}, 
   {{0xffffffff800266e0}, {"_Z34HandleSyscallModuleSectionRegisterPKc"}}, 
   {{0xffffffff800267d0}, {"_Z29HandleSyscallModuleSectionGetPKcPjS1_"}}, 
   {{0xffffffff80026890}, {"_Z36HandleSyscallModuleSectionUnregisterPKc"}}, 
   {{0xffffffff80026960}, {"_Z21HandleSyscallFileOpenPcPmS0_"}}, 
   {{0xffffffff80026a00}, {"_Z21HandleSyscallFileReadPcmm"}}, 
   {{0xffffffff80026aa0}, {"HandleSyscall"}}, 
   {{0xffffffff80026e00}, {"_Z22HandleSyscallFileWritem"}}, 
   {{0xffffffff80026e10}, {"_Z22HandleSyscallFileClosem"}}, 
   {{0xffffffff80026e20}, {"_Z25HandleSyscallKernOverridem"}}, 
   {{0xffffffff80026e30}, {"_ZN5TIMER13WaitForCyclesEm"}}, 
   {{0xffffffff80026e40}, {"_ZN6x86_646x86CPU4InitEv"}}, 
   {{0xffffffff80026f40}, {"_ZN6x86_646x86CPU9GetVendorEPc"}}, 
   {{0xffffffff80026f60}, {"_ZN6x86_647TSSInitEm"}}, 
   {{0xffffffff80027000}, {"_ZN6x86_647LoadGDTEm"}}, 
   {{0xffffffff80027050}, {"_Z15InitializeStackPvm"}}, 
   {{0xffffffff80027060}, {"_ZN6x86_648LoadMADTEv"}}, 
   {{0xffffffff800271b0}, {"_ZN6x86_6411GetAPICBaseEv"}}, 
   {{0xffffffff800271c0}, {"_ZN6x86_6411SetAPICBaseEm"}}, 
   {{0xffffffff800271e0}, {"_ZN6x86_6417WriteAPICRegisterEtj"}}, 
   {{0xffffffff80027200}, {"_ZN6x86_6416ReadAPICRegisterEt"}}, 
   {{0xffffffff80027220}, {"_ZN6x86_648WaitAPICEj"}}, 
   {{0xffffffff80027240}, {"_ZN6x86_6410EnableAPICEv"}}, 
   {{0xffffffff80027310}, {"_ZN6x86_6411SendAPICEOIEv"}}, 
   {{0xffffffff80027330}, {"_ZN6x86_6417WriteHPETRegisterEmjj"}}, 
   {{0xffffffff80027340}, {"_ZN6x86_6412EnableIOAPICEm"}}, 
   {{0xffffffff80027380}, {"_ZN6x86_6418ReadIOAPICRegisterEmt"}}, 
   {{0xffffffff80027390}, {"_ZN6x86_6419WriteIOAPICRegisterEmtm"}}, 
   {{0xffffffff800273a0}, {"_ZN6x86_647IDTInitEv"}}, 
   {{0xffffffff800274d0}, {"exceptionHandler"}}, 
   {{0xffffffff80027600}, {"timerHandler"}}, 
   {{0xffffffff80027650}, {"spuriousHandler"}}, 
   {{0xffffffff80027680}, {"syscallHandler"}}, 
   {{0xffffffff800276c0}, {"_ZN6x86_644OutBEth"}}, 
   {{0xffffffff800276d0}, {"_ZN6x86_644OutWEtt"}}, 
   {{0xffffffff800276e0}, {"_ZN6x86_643InBEt"}}, 
   {{0xffffffff800276f0}, {"_ZN6x86_643InWEt"}}, 
   {{0xffffffff80027700}, {"_ZN6x86_646IoWaitEv"}}, 
   {{0xffffffff80027710}, {"_ZN6x86_644InitEv"}}, 
   {{0xffffffff800277d0}, {"_ZN14PageMapIndexerC1Em"}}, 
   {{0xffffffff800277d0}, {"_ZN14PageMapIndexerC2Em"}}, 
   {{0xffffffff80027810}, {"_ZN16PageTableManager9MapMemoryEPvS0_m"}}, 
   {{0xffffffff80027b70}, {"_ZN16PageTableManager11UnmapMemoryEPv"}}, 
   {{0xffffffff80027c60}, {"_ZN16PageTableManager18GetPhysicalAddressEPv"}}, 
   {{0xffffffff80027d30}, {"_ZN16PageTableManager4ForkEPN3VMM12VirtualSpaceEb"}}, 
   {{0xffffffff80028100}, {"_ZN16PageTableManagerC1EP9PageTable"}}, 
   {{0xffffffff80028100}, {"_ZN16PageTableManagerC2EP9PageTable"}}, 
   {{0xffffffff80028110}, {"_ZN16PageTableManager13GetTopAddressEv"}}, 
   {{0xffffffff80028120}, {"_ZN18PageDirectoryEntry7SetFlagE7PT_Flagb"}}, 
   {{0xffffffff80028150}, {"_ZN18PageDirectoryEntry7GetFlagE7PT_Flag"}}, 
   {{0xffffffff80028160}, {"_ZN18PageDirectoryEntry10SetAddressEm"}}, 
   {{0xffffffff80028190}, {"_ZN18PageDirectoryEntry10GetAddressEv"}}, 
   {{0xffffffff800281a0}, {"_ZN6x86_6415NewVirtualSpaceEv"}}, 
   {{0xffffffff800281e0}, {"_ZN6x86_6416LoadVirtualSpaceEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80028210}, {"CPUPause"}}, 
   {{0xffffffff80028220}, {"FlushGDT"}}, 
   {{0xffffffff8002823a}, {"FlushTSS"}}, 
   {{0xffffffff80028250}, {"EnableSCE"}}, 
   {{0xffffffff80028290}, {"EnableSSE"}}, 
   {{0xffffffff800282ab}, {"EnableAVX"}}, 
   {{0xffffffff800282c0}, {"SwitchStack"}}, 
   {{0xffffffff80028300}, {"EnterUserspace"}}, 
   {{0xffffffff80028312}, {"UserStub"}}, 
   {{0xffffffff80028330}, {"isr_stub_0"}}, 
   {{0xffffffff80028371}, {"isr_stub_1"}}, 
   {{0xffffffff800283b2}, {"isr_stub_2"}}, 
   {{0xffffffff800283f3}, {"isr_stub_3"}}, 
   {{0xffffffff80028434}, {"isr_stub_4"}}, 
   {{0xffffffff80028475}, {"isr_stub_5"}}, 
   {{0xffffffff800284b6}, {"isr_stub_6"}}, 
   {{0xffffffff800284f7}, {"isr_stub_7"}}, 
   {{0xffffffff80028538}, {"isr_stub_8"}}, 
   {{0xffffffff80028577}, {"isr_stub_9"}}, 
   {{0xffffffff800285b8}, {"isr_stub_10"}}, 
   {{0xffffffff800285f7}, {"isr_stub_11"}}, 
   {{0xffffffff80028636}, {"isr_stub_12"}}, 
   {{0xffffffff80028675}, {"isr_stub_13"}}, 
   {{0xffffffff800286b4}, {"isr_stub_14"}}, 
   {{0xffffffff800286f3}, {"isr_stub_15"}}, 
   {{0xffffffff80028734}, {"isr_stub_16"}}, 
   {{0xffffffff80028775}, {"isr_stub_17"}}, 
   {{0xffffffff800287b4}, {"isr_stub_18"}}, 
   {{0xffffffff800287f5}, {"isr_stub_19"}}, 
   {{0xffffffff80028836}, {"isr_stub_20"}}, 
   {{0xffffffff80028877}, {"isr_stub_21"}}, 
   {{0xffffffff800288b8}, {"isr_stub_22"}}, 
   {{0xffffffff800288f9}, {"isr_stub_23"}}, 
   {{0xffffffff8002893a}, {"isr_stub_24"}}, 
   {{0xffffffff8002897b}, {"isr_stub_25"}}, 
   {{0xffffffff800289bc}, {"isr_stub_26"}}, 
   {{0xffffffff800289fd}, {"isr_stub_27"}}, 
   {{0xffffffff80028a3e}, {"isr_stub_28"}}, 
   {{0xffffffff80028a7f}, {"isr_stub_29"}}, 
   {{0xffffffff80028ac0}, {"isr_stub_30"}}, 
   {{0xffffffff80028aff}, {"isr_stub_31"}}, 
   {{0xffffffff80028b40}, {"isr_stub_32"}}, 
   {{0xffffffff80028b81}, {"isr_stub_33"}}, 
   {{0xffffffff80028bb3}, {"isr_stub_34"}}, 
   {{0xffffffff80028be5}, {"isr_stub_35"}}, 
   {{0xffffffff80028c17}, {"isr_stub_36"}}, 
   {{0xffffffff80028c49}, {"isr_stub_37"}}, 
   {{0xffffffff80028c7b}, {"isr_stub_38"}}, 
   {{0xffffffff80028cad}, {"isr_stub_39"}}, 
   {{0xffffffff80028cee}, {"isr_stub_40"}}, 
   {{0xffffffff80028d20}, {"isr_stub_41"}}, 
   {{0xffffffff80028d52}, {"isr_stub_42"}}, 
   {{0xffffffff80028d84}, {"isr_stub_43"}}, 
   {{0xffffffff80028db6}, {"isr_stub_44"}}, 
   {{0xffffffff80028de8}, {"isr_stub_45"}}, 
   {{0xffffffff80028e1a}, {"isr_stub_46"}}, 
   {{0xffffffff80028e4c}, {"isr_stub_47"}}, 
   {{0xffffffff80028e7e}, {"isr_stub_48"}}, 
   {{0xffffffff80028eb0}, {"isr_stub_49"}}, 
   {{0xffffffff80028ee2}, {"isr_stub_50"}}, 
   {{0xffffffff80028f14}, {"isr_stub_51"}}, 
   {{0xffffffff80028f46}, {"isr_stub_52"}}, 
   {{0xffffffff80028f78}, {"isr_stub_53"}}, 
   {{0xffffffff80028faa}, {"isr_stub_54"}}, 
   {{0xffffffff80028fdc}, {"isr_stub_55"}}, 
   {{0xffffffff8002900e}, {"isr_stub_56"}}, 
   {{0xffffffff80029040}, {"isr_stub_57"}}, 
   {{0xffffffff80029072}, {"isr_stub_58"}}, 
   {{0xffffffff800290a4}, {"isr_stub_59"}}, 
   {{0xffffffff800290d6}, {"isr_stub_60"}}, 
   {{0xffffffff80029108}, {"isr_stub_61"}}, 
   {{0xffffffff8002913a}, {"isr_stub_62"}}, 
   {{0xffffffff8002916c}, {"isr_stub_63"}}, 
   {{0xffffffff8002919e}, {"isr_stub_64"}}, 
   {{0xffffffff800291d0}, {"isr_stub_65"}}, 
   {{0xffffffff80029202}, {"isr_stub_66"}}, 
   {{0xffffffff80029234}, {"isr_stub_67"}}, 
   {{0xffffffff80029266}, {"isr_stub_68"}}, 
   {{0xffffffff80029298}, {"isr_stub_69"}}, 
   {{0xffffffff800292ca}, {"isr_stub_70"}}, 
   {{0xffffffff800292fc}, {"isr_stub_71"}}, 
   {{0xffffffff8002932e}, {"isr_stub_72"}}, 
   {{0xffffffff80029360}, {"isr_stub_73"}}, 
   {{0xffffffff80029392}, {"isr_stub_74"}}, 
   {{0xffffffff800293c4}, {"isr_stub_75"}}, 
   {{0xffffffff800293f6}, {"isr_stub_76"}}, 
   {{0xffffffff80029428}, {"isr_stub_77"}}, 
   {{0xffffffff8002945a}, {"isr_stub_78"}}, 
   {{0xffffffff8002948c}, {"isr_stub_79"}}, 
   {{0xffffffff800294be}, {"isr_stub_80"}}, 
   {{0xffffffff800294f0}, {"isr_stub_81"}}, 
   {{0xffffffff80029522}, {"isr_stub_82"}}, 
   {{0xffffffff80029554}, {"isr_stub_83"}}, 
   {{0xffffffff80029586}, {"isr_stub_84"}}, 
   {{0xffffffff800295b8}, {"isr_stub_85"}}, 
   {{0xffffffff800295ea}, {"isr_stub_86"}}, 
   {{0xffffffff8002961c}, {"isr_stub_87"}}, 
   {{0xffffffff8002964e}, {"isr_stub_88"}}, 
   {{0xffffffff80029680}, {"isr_stub_89"}}, 
   {{0xffffffff800296b2}, {"isr_stub_90"}}, 
   {{0xffffffff800296e4}, {"isr_stub_91"}}, 
   {{0xffffffff80029716}, {"isr_stub_92"}}, 
   {{0xffffffff80029748}, {"isr_stub_93"}}, 
   {{0xffffffff8002977a}, {"isr_stub_94"}}, 
   {{0xffffffff800297ac}, {"isr_stub_95"}}, 
   {{0xffffffff800297de}, {"isr_stub_96"}}, 
   {{0xffffffff80029810}, {"isr_stub_97"}}, 
   {{0xffffffff80029842}, {"isr_stub_98"}}, 
   {{0xffffffff80029874}, {"isr_stub_99"}}, 
   {{0xffffffff800298a6}, {"isr_stub_100"}}, 
   {{0xffffffff800298d8}, {"isr_stub_101"}}, 
   {{0xffffffff8002990a}, {"isr_stub_102"}}, 
   {{0xffffffff8002993c}, {"isr_stub_103"}}, 
   {{0xffffffff8002996e}, {"isr_stub_104"}}, 
   {{0xffffffff800299a0}, {"isr_stub_105"}}, 
   {{0xffffffff800299d2}, {"isr_stub_106"}}, 
   {{0xffffffff80029a04}, {"isr_stub_107"}}, 
   {{0xffffffff80029a36}, {"isr_stub_108"}}, 
   {{0xffffffff80029a68}, {"isr_stub_109"}}, 
   {{0xffffffff80029a9a}, {"isr_stub_110"}}, 
   {{0xffffffff80029acc}, {"isr_stub_111"}}, 
   {{0xffffffff80029afe}, {"isr_stub_112"}}, 
   {{0xffffffff80029b30}, {"isr_stub_113"}}, 
   {{0xffffffff80029b62}, {"isr_stub_114"}}, 
   {{0xffffffff80029b94}, {"isr_stub_115"}}, 
   {{0xffffffff80029bc6}, {"isr_stub_116"}}, 
   {{0xffffffff80029bf8}, {"isr_stub_117"}}, 
   {{0xffffffff80029c2a}, {"isr_stub_118"}}, 
   {{0xffffffff80029c5c}, {"isr_stub_119"}}, 
   {{0xffffffff80029c8e}, {"isr_stub_120"}}, 
   {{0xffffffff80029cc0}, {"isr_stub_121"}}, 
   {{0xffffffff80029cf2}, {"isr_stub_122"}}, 
   {{0xffffffff80029d24}, {"isr_stub_123"}}, 
   {{0xffffffff80029d56}, {"isr_stub_124"}}, 
   {{0xffffffff80029d88}, {"isr_stub_125"}}, 
   {{0xffffffff80029dba}, {"isr_stub_126"}}, 
   {{0xffffffff80029dec}, {"isr_stub_127"}}, 
   {{0xffffffff80029e1e}, {"isr_stub_128"}}, 
   {{0xffffffff80029e50}, {"isr_stub_129"}}, 
   {{0xffffffff80029e82}, {"isr_stub_130"}}, 
   {{0xffffffff80029eb4}, {"isr_stub_131"}}, 
   {{0xffffffff80029ee6}, {"isr_stub_132"}}, 
   {{0xffffffff80029f18}, {"isr_stub_133"}}, 
   {{0xffffffff80029f4a}, {"isr_stub_134"}}, 
   {{0xffffffff80029f7c}, {"isr_stub_135"}}, 
   {{0xffffffff80029fae}, {"isr_stub_136"}}, 
   {{0xffffffff80029fe0}, {"isr_stub_137"}}, 
   {{0xffffffff8002a012}, {"isr_stub_138"}}, 
   {{0xffffffff8002a044}, {"isr_stub_139"}}, 
   {{0xffffffff8002a076}, {"isr_stub_140"}}, 
   {{0xffffffff8002a0a8}, {"isr_stub_141"}}, 
   {{0xffffffff8002a0da}, {"isr_stub_142"}}, 
   {{0xffffffff8002a10c}, {"isr_stub_143"}}, 
   {{0xffffffff8002a13e}, {"isr_stub_144"}}, 
   {{0xffffffff8002a170}, {"isr_stub_145"}}, 
   {{0xffffffff8002a1a2}, {"isr_stub_146"}}, 
   {{0xffffffff8002a1d4}, {"isr_stub_147"}}, 
   {{0xffffffff8002a206}, {"isr_stub_148"}}, 
   {{0xffffffff8002a238}, {"isr_stub_149"}}, 
   {{0xffffffff8002a26a}, {"isr_stub_150"}}, 
   {{0xffffffff8002a29c}, {"isr_stub_151"}}, 
   {{0xffffffff8002a2ce}, {"isr_stub_152"}}, 
   {{0xffffffff8002a300}, {"isr_stub_153"}}, 
   {{0xffffffff8002a332}, {"isr_stub_154"}}, 
   {{0xffffffff8002a364}, {"isr_stub_155"}}, 
   {{0xffffffff8002a396}, {"isr_stub_156"}}, 
   {{0xffffffff8002a3c8}, {"isr_stub_157"}}, 
   {{0xffffffff8002a3fa}, {"isr_stub_158"}}, 
   {{0xffffffff8002a42c}, {"isr_stub_159"}}, 
   {{0xffffffff8002a45e}, {"isr_stub_160"}}, 
   {{0xffffffff8002a490}, {"isr_stub_161"}}, 
   {{0xffffffff8002a4c2}, {"isr_stub_162"}}, 
   {{0xffffffff8002a4f4}, {"isr_stub_163"}}, 
   {{0xffffffff8002a526}, {"isr_stub_164"}}, 
   {{0xffffffff8002a558}, {"isr_stub_165"}}, 
   {{0xffffffff8002a58a}, {"isr_stub_166"}}, 
   {{0xffffffff8002a5bc}, {"isr_stub_167"}}, 
   {{0xffffffff8002a5ee}, {"isr_stub_168"}}, 
   {{0xffffffff8002a620}, {"isr_stub_169"}}, 
   {{0xffffffff8002a652}, {"isr_stub_170"}}, 
   {{0xffffffff8002a684}, {"isr_stub_171"}}, 
   {{0xffffffff8002a6b6}, {"isr_stub_172"}}, 
   {{0xffffffff8002a6e8}, {"isr_stub_173"}}, 
   {{0xffffffff8002a71a}, {"isr_stub_174"}}, 
   {{0xffffffff8002a74c}, {"isr_stub_175"}}, 
   {{0xffffffff8002a77e}, {"isr_stub_176"}}, 
   {{0xffffffff8002a7b0}, {"isr_stub_177"}}, 
   {{0xffffffff8002a7e2}, {"isr_stub_178"}}, 
   {{0xffffffff8002a814}, {"isr_stub_179"}}, 
   {{0xffffffff8002a846}, {"isr_stub_180"}}, 
   {{0xffffffff8002a878}, {"isr_stub_181"}}, 
   {{0xffffffff8002a8aa}, {"isr_stub_182"}}, 
   {{0xffffffff8002a8dc}, {"isr_stub_183"}}, 
   {{0xffffffff8002a90e}, {"isr_stub_184"}}, 
   {{0xffffffff8002a940}, {"isr_stub_185"}}, 
   {{0xffffffff8002a972}, {"isr_stub_186"}}, 
   {{0xffffffff8002a9a4}, {"isr_stub_187"}}, 
   {{0xffffffff8002a9d6}, {"isr_stub_188"}}, 
   {{0xffffffff8002aa08}, {"isr_stub_189"}}, 
   {{0xffffffff8002aa3a}, {"isr_stub_190"}}, 
   {{0xffffffff8002aa6c}, {"isr_stub_191"}}, 
   {{0xffffffff8002aa9e}, {"isr_stub_192"}}, 
   {{0xffffffff8002aad0}, {"isr_stub_193"}}, 
   {{0xffffffff8002ab02}, {"isr_stub_194"}}, 
   {{0xffffffff8002ab34}, {"isr_stub_195"}}, 
   {{0xffffffff8002ab66}, {"isr_stub_196"}}, 
   {{0xffffffff8002ab98}, {"isr_stub_197"}}, 
   {{0xffffffff8002abca}, {"isr_stub_198"}}, 
   {{0xffffffff8002abfc}, {"isr_stub_199"}}, 
   {{0xffffffff8002ac2e}, {"isr_stub_200"}}, 
   {{0xffffffff8002ac60}, {"isr_stub_201"}}, 
   {{0xffffffff8002ac92}, {"isr_stub_202"}}, 
   {{0xffffffff8002acc4}, {"isr_stub_203"}}, 
   {{0xffffffff8002acf6}, {"isr_stub_204"}}, 
   {{0xffffffff8002ad28}, {"isr_stub_205"}}, 
   {{0xffffffff8002ad5a}, {"isr_stub_206"}}, 
   {{0xffffffff8002ad8c}, {"isr_stub_207"}}, 
   {{0xffffffff8002adbe}, {"isr_stub_208"}}, 
   {{0xffffffff8002adf0}, {"isr_stub_209"}}, 
   {{0xffffffff8002ae22}, {"isr_stub_210"}}, 
   {{0xffffffff8002ae54}, {"isr_stub_211"}}, 
   {{0xffffffff8002ae86}, {"isr_stub_212"}}, 
   {{0xffffffff8002aeb8}, {"isr_stub_213"}}, 
   {{0xffffffff8002aeea}, {"isr_stub_214"}}, 
   {{0xffffffff8002af1c}, {"isr_stub_215"}}, 
   {{0xffffffff8002af4e}, {"isr_stub_216"}}, 
   {{0xffffffff8002af80}, {"isr_stub_217"}}, 
   {{0xffffffff8002afb2}, {"isr_stub_218"}}, 
   {{0xffffffff8002afe4}, {"isr_stub_219"}}, 
   {{0xffffffff8002b016}, {"isr_stub_220"}}, 
   {{0xffffffff8002b048}, {"isr_stub_221"}}, 
   {{0xffffffff8002b07a}, {"isr_stub_222"}}, 
   {{0xffffffff8002b0ac}, {"isr_stub_223"}}, 
   {{0xffffffff8002b0de}, {"isr_stub_224"}}, 
   {{0xffffffff8002b110}, {"isr_stub_225"}}, 
   {{0xffffffff8002b142}, {"isr_stub_226"}}, 
   {{0xffffffff8002b174}, {"isr_stub_227"}}, 
   {{0xffffffff8002b1a6}, {"isr_stub_228"}}, 
   {{0xffffffff8002b1d8}, {"isr_stub_229"}}, 
   {{0xffffffff8002b20a}, {"isr_stub_230"}}, 
   {{0xffffffff8002b23c}, {"isr_stub_231"}}, 
   {{0xffffffff8002b26e}, {"isr_stub_232"}}, 
   {{0xffffffff8002b2a0}, {"isr_stub_233"}}, 
   {{0xffffffff8002b2d2}, {"isr_stub_234"}}, 
   {{0xffffffff8002b304}, {"isr_stub_235"}}, 
   {{0xffffffff8002b336}, {"isr_stub_236"}}, 
   {{0xffffffff8002b368}, {"isr_stub_237"}}, 
   {{0xffffffff8002b39a}, {"isr_stub_238"}}, 
   {{0xffffffff8002b3cc}, {"isr_stub_239"}}, 
   {{0xffffffff8002b3fe}, {"isr_stub_240"}}, 
   {{0xffffffff8002b430}, {"isr_stub_241"}}, 
   {{0xffffffff8002b462}, {"isr_stub_242"}}, 
   {{0xffffffff8002b494}, {"isr_stub_243"}}, 
   {{0xffffffff8002b4c6}, {"isr_stub_244"}}, 
   {{0xffffffff8002b4f8}, {"isr_stub_245"}}, 
   {{0xffffffff8002b52a}, {"isr_stub_246"}}, 
   {{0xffffffff8002b55c}, {"isr_stub_247"}}, 
   {{0xffffffff8002b58e}, {"isr_stub_248"}}, 
   {{0xffffffff8002b5c0}, {"isr_stub_249"}}, 
   {{0xffffffff8002b5f2}, {"isr_stub_250"}}, 
   {{0xffffffff8002b624}, {"isr_stub_251"}}, 
   {{0xffffffff8002b656}, {"isr_stub_252"}}, 
   {{0xffffffff8002b688}, {"isr_stub_253"}}, 
   {{0xffffffff8002b6ba}, {"isr_stub_254"}}, 
   {{0xffffffff8002b6fe}, {"isr_stub_255"}}, 
   {{0xffffffff8002b730}, {"isrStubTable"}}, 
   {{0xffffffff8002d2a0}, {"_ZTV10UARTDevice"}}, 
   {{0xffffffff8002d580}, {"_ZTV8SpinLock"}}, 
   {{0xffffffff8002d640}, {"_ZL14memTypeStrings"}}, 
   {{0xffffffff80030d40}, {"symbols"}}, 
   {{0xffffffff800332b0}, {"symbolCount"}}, 
   {{0xffffffff80033850}, {"_ZTV16PageTableManager"}}, 
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
