#include <sys/symbol.hpp>
const uint64_t symbolCount = 620;
const Symbol symbols[] = {
   {{0xffffffff80000000}, {"SyscallEntry"}}, 
   {{0xffffffff80000000}, {"syscall_start"}}, 
   {{0xffffffff80000050}, {"SyscallStack"}}, 
   {{0xffffffff80020050}, {"syscall_end"}}, 
   {{0xffffffff80021050}, {"EfiMain"}}, 
   {{0xffffffff8002106c}, {"LimineEntry"}}, 
   {{0xffffffff80021498}, {"__stack_chk_fail"}}, 
   {{0xffffffff800214bd}, {"_Z11UnwindStacki"}}, 
   {{0xffffffff80021552}, {"_ZN10UARTDevice4InitE11SerialPorts"}}, 
   {{0xffffffff80021644}, {"_ZN10UARTDevice15isTransmitEmptyEv"}}, 
   {{0xffffffff8002165c}, {"_ZN10UARTDevice7PutCharEc"}}, 
   {{0xffffffff8002168e}, {"_ZN10UARTDevice6PutStrEPKc"}}, 
   {{0xffffffff800216d6}, {"_ZN10UARTDevice14serialReceivedEv"}}, 
   {{0xffffffff800216ee}, {"_ZN10UARTDevice7GetCharEv"}}, 
   {{0xffffffff80021726}, {"_ZN10UARTDevice5IoctlEmz"}}, 
   {{0xffffffff8002182c}, {"_Z9ParseArgsv"}}, 
   {{0xffffffff800218e4}, {"_Z8InitInfov"}}, 
   {{0xffffffff8002190d}, {"_Z7GetInfov"}}, 
   {{0xffffffff80021915}, {"_Z15EarlyKernelInitv"}}, 
   {{0xffffffff80021934}, {"_Z11PrintBannerv"}}, 
   {{0xffffffff800219dd}, {"_Z11KernelStartv"}}, 
   {{0xffffffff80021b49}, {"_ZN7BOOTMEM17DeactivateBootmemEv"}}, 
   {{0xffffffff80021b51}, {"_ZN7BOOTMEM15BootmemIsActiveEv"}}, 
   {{0xffffffff80021b59}, {"_ZN7BOOTMEM6MallocEm"}}, 
   {{0xffffffff80021ba5}, {"_ZN7BOOTMEM4FreeEPv"}}, 
   {{0xffffffff80021ba6}, {"_ZN7BOOTMEM7GetFreeEv"}}, 
   {{0xffffffff80021bb2}, {"_ZN7BOOTMEM8GetTotalEv"}}, 
   {{0xffffffff80021bb8}, {"_Z41__static_initialization_and_destruction_0v"}}, 
   {{0xffffffff80021bd6}, {"_ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff80021bde}, {"_ZN4HEAP7GetFreeEv"}}, 
   {{0xffffffff80021be6}, {"_ZN4HEAP8GetTotalEv"}}, 
   {{0xffffffff80021bee}, {"_ZN13HeapSegHeader14CombineForwardEv"}}, 
   {{0xffffffff80021c36}, {"_ZN13HeapSegHeader15CombineBackwardEv"}}, 
   {{0xffffffff80021c52}, {"_ZN13HeapSegHeader5SplitEm"}}, 
   {{0xffffffff80021cca}, {"_ZN4HEAP14InitializeHeapEPvm"}}, 
   {{0xffffffff80021d8b}, {"_ZN4HEAP4FreeEPv"}}, 
   {{0xffffffff80021dea}, {"_ZN4HEAP10ExpandHeapEm"}}, 
   {{0xffffffff80021eda}, {"_ZN4HEAP6MallocEm"}}, 
   {{0xffffffff80021fa7}, {"_GLOBAL__sub_I__ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff80021fb2}, {"_ZN8SpinLock6UnlockEv"}}, 
   {{0xffffffff80021fb8}, {"_ZN8SpinLock8IsLockedEv"}}, 
   {{0xffffffff80021fbe}, {"_ZN8SpinLock4LockEv"}}, 
   {{0xffffffff80021ff0}, {"memcmp"}}, 
   {{0xffffffff8002201b}, {"memcpy"}}, 
   {{0xffffffff8002203c}, {"_Z6Mallocm"}}, 
   {{0xffffffff80022079}, {"_Z4FreePv"}}, 
   {{0xffffffff80022084}, {"_Znwm"}}, 
   {{0xffffffff800220c1}, {"_Znam"}}, 
   {{0xffffffff800220fe}, {"_ZdlPv"}}, 
   {{0xffffffff80022109}, {"_ZdlPvm"}}, 
   {{0xffffffff80022114}, {"_ZdaPv"}}, 
   {{0xffffffff8002211f}, {"_ZdaPvm"}}, 
   {{0xffffffff8002212a}, {"_ZN3MEM10DisplayRamEv"}}, 
   {{0xffffffff80022197}, {"_ZN3MEM4InitEv"}}, 
   {{0xffffffff800221a7}, {"memset"}}, 
   {{0xffffffff800221bf}, {"_ZL10InitBitmapmPv"}}, 
   {{0xffffffff800221ea}, {"_Z41__static_initialization_and_destruction_0v"}}, 
   {{0xffffffff80022208}, {"_ZL13UnreservePagePv"}}, 
   {{0xffffffff80022271}, {"_ZL14UnreservePagesPvm"}}, 
   {{0xffffffff800222b1}, {"_ZL11ReservePagePv"}}, 
   {{0xffffffff8002230a}, {"_ZL12ReservePagesPvm"}}, 
   {{0xffffffff8002234a}, {"_ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff800224b0}, {"_ZN3PMM8FreePageEPv"}}, 
   {{0xffffffff80022527}, {"_ZN3PMM8LockPageEPv"}}, 
   {{0xffffffff80022593}, {"_ZN3PMM11RequestPageEv"}}, 
   {{0xffffffff8002262c}, {"_ZN3PMM9FreePagesEPvm"}}, 
   {{0xffffffff8002266c}, {"_ZN3PMM9LockPagesEPvm"}}, 
   {{0xffffffff800226ac}, {"_ZN3PMM12RequestPagesEm"}}, 
   {{0xffffffff80022775}, {"_ZN3PMM10GetFreeMemEv"}}, 
   {{0xffffffff8002277d}, {"_ZN3PMM10GetUsedMemEv"}}, 
   {{0xffffffff80022785}, {"_ZN3PMM14GetReservedMemEv"}}, 
   {{0xffffffff8002278d}, {"_GLOBAL__sub_I__ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80022798}, {"_Z6strlenPKc"}}, 
   {{0xffffffff800227aa}, {"_Z6strcpyPcPKc"}}, 
   {{0xffffffff800227df}, {"_Z6strcmpPKcS0_"}}, 
   {{0xffffffff80022822}, {"_Z8is_delimcPc"}}, 
   {{0xffffffff8002283b}, {"_Z6strtokPcS_"}}, 
   {{0xffffffff800228d4}, {"_Z4itoaPcix"}}, 
   {{0xffffffff8002296d}, {"_Z4atoiPc"}}, 
   {{0xffffffff80022998}, {"_ZN3VMM15NewVirtualSpaceEv"}}, 
   {{0xffffffff800229a3}, {"_ZN3VMM21NewModuleVirtualSpaceEv"}}, 
   {{0xffffffff80022b0c}, {"_ZN3VMM21NewKernelVirtualSpaceEv"}}, 
   {{0xffffffff80022c1e}, {"_ZN3VMM16LoadVirtualSpaceEPNS_12VirtualSpaceE"}}, 
   {{0xffffffff80022c29}, {"_ZN3VMM7InitVMMEv"}}, 
   {{0xffffffff80022c62}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_"}}, 
   {{0xffffffff80022c73}, {"_ZN3VMM9MapMemoryEPNS_12VirtualSpaceEPvS2_m"}}, 
   {{0xffffffff80022c80}, {"_ZN6MODULE13BufferManagerC1Ev"}}, 
   {{0xffffffff80022c80}, {"_ZN6MODULE13BufferManagerC2Ev"}}, 
   {{0xffffffff80022cb6}, {"_ZN6MODULE13BufferManager8FindNodeEjPPNS_10BufferNodeEPb"}}, 
   {{0xffffffff80022cf0}, {"_ZN6MODULE13BufferManager7AddNodeEPNS_6BufferE"}}, 
   {{0xffffffff80022d5a}, {"_ZN6MODULE13BufferManager10RemoveNodeEj"}}, 
   {{0xffffffff80022dd2}, {"_ZN6MODULE13BufferManager12CreateBufferEjjNS_10BufferTypeEm"}}, 
   {{0xffffffff80022e82}, {"_ZN6MODULE13BufferManager9GetBufferEj"}}, 
   {{0xffffffff80022ecc}, {"_ZN6MODULE13BufferManager9MapBufferEjjjPN3VMM12VirtualSpaceEm"}}, 
   {{0xffffffff80022ff2}, {"_ZN6MODULE13BufferManager12DeleteBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023054}, {"_ZN6MODULE13BufferManager10LockBufferEPNS_6BufferE"}}, 
   {{0xffffffff80023092}, {"_ZN6MODULE13BufferManager12UnlockBufferEPNS_6BufferE"}}, 
   {{0xffffffff800230ba}, {"_ZN6MODULE14ComposeMessageEPNS_7MessageEjjm"}}, 
   {{0xffffffff80023140}, {"_ZN6MODULE6ModuleC1EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff80023140}, {"_ZN6MODULE6ModuleC2EjjPN4PROC7ProcessE"}}, 
   {{0xffffffff8002314a}, {"_ZN6MODULE7ManagerC1Ev"}}, 
   {{0xffffffff8002314a}, {"_ZN6MODULE7ManagerC2Ev"}}, 
   {{0xffffffff80023178}, {"_ZN6MODULE7Manager8FindNodeEjjPPNS_10ModuleNodeEPb"}}, 
   {{0xffffffff800231c8}, {"_ZN6MODULE7Manager7AddNodeEjj"}}, 
   {{0xffffffff8002322c}, {"_ZN6MODULE7Manager10RemoveNodeEjj"}}, 
   {{0xffffffff800232a4}, {"_ZN6MODULE7Manager14RegisterModuleEPN4PROC7ProcessEjj"}}, 
   {{0xffffffff80023324}, {"_ZN6MODULE7Manager9GetModuleEjj"}}, 
   {{0xffffffff8002336a}, {"_ZN6MODULE7Manager9GetModuleEm"}}, 
   {{0xffffffff800233a8}, {"_ZN6MODULE7Manager16UnregisterModuleEjj"}}, 
   {{0xffffffff800233b4}, {"_ZN6MODULE14SectionManagerC1Ev"}}, 
   {{0xffffffff800233b4}, {"_ZN6MODULE14SectionManagerC2Ev"}}, 
   {{0xffffffff800233f2}, {"_ZN6MODULE14SectionManager8FindNodeEPKcjjPPNS_11SectionNodeEPb"}}, 
   {{0xffffffff800234b8}, {"_ZN6MODULE14SectionManager7AddNodeEPKcjj"}}, 
   {{0xffffffff80023538}, {"_ZN6MODULE14SectionManager10RemoveNodeEPKcjj"}}, 
   {{0xffffffff8002359c}, {"_ZN6MODULE14SectionManager21RegisterSectionDriverEPKcjj"}}, 
   {{0xffffffff8002360a}, {"_ZN6MODULE14SectionManager16GetSectionDriverEPKcPjS3_"}}, 
   {{0xffffffff80023682}, {"_ZN6MODULE14SectionManager23UnregisterSectionDriverEPKcjj"}}, 
   {{0xffffffff8002368e}, {"_ZN4PROC7ProcessD1Ev"}}, 
   {{0xffffffff8002368e}, {"_ZN4PROC7ProcessD2Ev"}}, 
   {{0xffffffff800236d6}, {"_ZN4PROC7Process9GetThreadEm"}}, 
   {{0xffffffff800236f4}, {"_ZN4PROC7Process10RequestTIDEv"}}, 
   {{0xffffffff80023702}, {"_ZN4PROC7Process13DestroyThreadEPNS_6ThreadE"}}, 
   {{0xffffffff80023704}, {"_ZN4PROC7Process13GetMainThreadEv"}}, 
   {{0xffffffff8002370a}, {"_ZN4PROC7Process16GetMessageThreadEv"}}, 
   {{0xffffffff80023710}, {"_ZN4PROC7Process11SetPriorityEh"}}, 
   {{0xffffffff80023716}, {"_ZN4PROC7Process15SetProcessStateENS_12ProcessStateE"}}, 
   {{0xffffffff8002374c}, {"_ZN4PROC7Process6GetPIDEv"}}, 
   {{0xffffffff80023750}, {"_ZN4PROC7Process15GetProcessStateEv"}}, 
   {{0xffffffff80023754}, {"_ZN4PROC7Process21GetVirtualMemorySpaceEv"}}, 
   {{0xffffffff8002375a}, {"_ZN4PROC7Process14GetHighestFreeEv"}}, 
   {{0xffffffff80023760}, {"_ZN4PROC7Process14SetHighestFreeEm"}}, 
   {{0xffffffff80023766}, {"_ZN4PROC6ThreadD1Ev"}}, 
   {{0xffffffff80023766}, {"_ZN4PROC6ThreadD2Ev"}}, 
   {{0xffffffff80023792}, {"_ZN4PROC6Thread8SetStateENS_12ProcessStateE"}}, 
   {{0xffffffff80023796}, {"_ZN4PROC7Process14SetThreadStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff800237c4}, {"_ZN4PROC6Thread8GetStateEv"}}, 
   {{0xffffffff800237c8}, {"_ZN4PROC7Process14GetThreadStateEm"}}, 
   {{0xffffffff800237fc}, {"_ZN4PROC7Process13SetMainThreadEm"}}, 
   {{0xffffffff8002383a}, {"_ZN4PROC6Thread6GetTIDEv"}}, 
   {{0xffffffff8002383e}, {"_ZN4PROC6ThreadC1EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff8002383e}, {"_ZN4PROC6ThreadC2EPNS_7ProcessEmmPm"}}, 
   {{0xffffffff800239b6}, {"_ZN4PROC6Thread14SetInstructionEm"}}, 
   {{0xffffffff800239bc}, {"_ZN4PROC7Process16SetMessageThreadEPv"}}, 
   {{0xffffffff800239cc}, {"_ZN4PROC6Thread8SetStackEm"}}, 
   {{0xffffffff800239d2}, {"_ZN4PROC6Thread12GetStackBaseEv"}}, 
   {{0xffffffff800239d8}, {"_ZN4PROC6Thread8GetStackEv"}}, 
   {{0xffffffff800239de}, {"_ZN4PROC6Thread12GetStackSizeEv"}}, 
   {{0xffffffff800239e4}, {"_ZN4PROC7Process12CreateThreadEmm"}}, 
   {{0xffffffff80023a6c}, {"_ZN4PROC7ProcessC1ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80023a6c}, {"_ZN4PROC7ProcessC2ENS_11ProcessTypeEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80023b08}, {"_ZN6VectorIPN4PROC6ThreadEE4InitEv"}}, 
   {{0xffffffff80023b46}, {"_ZN6VectorIPN4PROC6ThreadEE4PushES2_"}}, 
   {{0xffffffff80023bf8}, {"_ZN4PROC9Scheduler12PopFirstNodeEPNS_11ProcessNodeE"}}, 
   {{0xffffffff80023c2e}, {"_ZN4PROC9Scheduler8FindNodeEPNS_11ProcessNodeEmPS2_Pb"}}, 
   {{0xffffffff80023c9e}, {"_ZN4PROC9Scheduler7AddNodeEPNS_11ProcessNodeEPNS_7ProcessE"}}, 
   {{0xffffffff80023d26}, {"_ZN4PROC9Scheduler10RemoveNodeEPNS_11ProcessNodeEm"}}, 
   {{0xffffffff80023d96}, {"_ZN4PROC9Scheduler10AddProcessEPNS_7ProcessE"}}, 
   {{0xffffffff80023dd6}, {"_ZN4PROC9SchedulerC1Ev"}}, 
   {{0xffffffff80023dd6}, {"_ZN4PROC9SchedulerC2Ev"}}, 
   {{0xffffffff80023e78}, {"_ZN4PROC9Scheduler10GetProcessEmPNS_11ProcessNodeE"}}, 
   {{0xffffffff80023ed8}, {"_ZN4PROC9Scheduler10GetProcessEm"}}, 
   {{0xffffffff80023f68}, {"_ZN4PROC9Scheduler15SetProcessStateEmNS_12ProcessStateE"}}, 
   {{0xffffffff800240d8}, {"_ZN4PROC9Scheduler17GetRunningProcessEv"}}, 
   {{0xffffffff800240de}, {"_ZN4PROC9Scheduler12SwitchToTaskEPNS_7ProcessEm"}}, 
   {{0xffffffff800241f4}, {"_ZN4PROC9Scheduler20RecalculateSchedulerEv"}}, 
   {{0xffffffff80024246}, {"_ZN6Bitmap3GetEm"}}, 
   {{0xffffffff80024282}, {"_ZN6BitmapixEm"}}, 
   {{0xffffffff8002428e}, {"_ZN6Bitmap3SetEmb"}}, 
   {{0xffffffff800242e0}, {"_Z5IoctlP6Drivermz"}}, 
   {{0xffffffff80024340}, {"_Z18LoadProgramHeadersPhmP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff8002456a}, {"_Z11LinkSymbolsPhmP10Elf64_Ehdr"}}, 
   {{0xffffffff800245de}, {"_Z11LoadProcessP10Elf64_EhdrPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80024681}, {"_Z7LoadELFPhm"}}, 
   {{0xffffffff800246df}, {"_Z10FB_PutCharc"}}, 
   {{0xffffffff800247e8}, {"_Z14FB_PrintScreenPKc"}}, 
   {{0xffffffff80024811}, {"_Z6InitFBv"}}, 
   {{0xffffffff80024839}, {"_Z14FB_PrintScreeniiPKc"}}, 
   {{0xffffffff80024949}, {"_ZN4FILE4OpenEPcPm"}}, 
   {{0xffffffff80024ade}, {"__cxa_guard_acquire"}}, 
   {{0xffffffff80024ae8}, {"__cxa_guard_release"}}, 
   {{0xffffffff80024aec}, {"__cxa_guard_abort"}}, 
   {{0xffffffff80024aed}, {"__cxa_pure_virtual"}}, 
   {{0xffffffff80024b04}, {"__cxa_atexit"}}, 
   {{0xffffffff80024b47}, {"__cxa_finalize"}}, 
   {{0xffffffff80024bdc}, {"_Z16RequestInterruptj"}}, 
   {{0xffffffff80024bdf}, {"_Z18LoadExecutableFilePhm"}}, 
   {{0xffffffff80024c26}, {"_Z12TryLockMutexPb"}}, 
   {{0xffffffff80024c38}, {"_Z17SpinlockLockMutexPb"}}, 
   {{0xffffffff80024c49}, {"_Z11UnlockMutexPb"}}, 
   {{0xffffffff80024c4d}, {"_Z5PanicPKcS0_S0_j"}}, 
   {{0xffffffff80024c82}, {"_Z4OopsPKcS0_S0_j"}}, 
   {{0xffffffff80024cb7}, {"_Z41__static_initialization_and_destruction_0v"}}, 
   {{0xffffffff80024cd5}, {"_ZN6PRINTK11FlushBufferEv"}}, 
   {{0xffffffff80024d14}, {"_ZN6PRINTK7PutCharEc"}}, 
   {{0xffffffff80024d7c}, {"_ZN6PRINTK6PutStrEPc"}}, 
   {{0xffffffff80024da5}, {"_ZN6PRINTK7VPrintKEPcP13__va_list_tag"}}, 
   {{0xffffffff80024f25}, {"_ZN6PRINTK6PrintKEPcz"}}, 
   {{0xffffffff80024f82}, {"_ZN6PRINTK9EarlyInitEv"}}, 
   {{0xffffffff80024fec}, {"_GLOBAL__sub_I_kernelPort"}}, 
   {{0xffffffff80024ff7}, {"_Z11LoadShebangPhm"}}, 
   {{0xffffffff80024fff}, {"_Z12LookupSymbolPKc"}}, 
   {{0xffffffff80025052}, {"_Z12LookupSymbolm"}}, 
   {{0xffffffff800250ae}, {"_Z11AddOverridem"}}, 
   {{0xffffffff800250af}, {"_Z13CheckOverridem"}}, 
   {{0xffffffff800250b5}, {"_Z11RunOverridem"}}, 
   {{0xffffffff800250bb}, {"_Z24HandleSyscallDebugPrintKPKc"}}, 
   {{0xffffffff800250da}, {"_Z26HandleSyscallMemoryGetinfom"}}, 
   {{0xffffffff80025132}, {"_Z26HandleSyscallMemoryVmallocmmm"}}, 
   {{0xffffffff8002523b}, {"_Z25HandleSyscallMemoryPallocPmm"}}, 
   {{0xffffffff800252b5}, {"_Z25HandleSyscallMemoryVmfreemm"}}, 
   {{0xffffffff8002539c}, {"_Z23HandleSyscallMemoryMmapmmmm"}}, 
   {{0xffffffff80025453}, {"_Z24HandleSyscallMemoryUnmapmm"}}, 
   {{0xffffffff8002550e}, {"_Z24HandleSyscallMemoryInOutmbmPmh"}}, 
   {{0xffffffff80025584}, {"_Z21HandleSyscallProcExecmm"}}, 
   {{0xffffffff800256eb}, {"_Z21HandleSyscallProcForkm"}}, 
   {{0xffffffff800256f1}, {"_Z23HandleSyscallProcReturnmm"}}, 
   {{0xffffffff8002575c}, {"_Z21HandleSyscallProcExitmm"}}, 
   {{0xffffffff80025790}, {"_Z21HandleSyscallProcWaitm"}}, 
   {{0xffffffff80025796}, {"_Z21HandleSyscallProcKillm"}}, 
   {{0xffffffff8002579c}, {"_Z27HandleSyscallModuleRegistermm"}}, 
   {{0xffffffff8002580b}, {"_Z29HandleSyscallModuleUnregisterv"}}, 
   {{0xffffffff80025884}, {"_Z31HandleSyscallModuleBufferCreatemmPj"}}, 
   {{0xffffffff8002592c}, {"_Z28HandleSyscallModuleBufferMapmj"}}, 
   {{0xffffffff800259cf}, {"_Z30HandleSyscallModuleBufferUnmapmj"}}, 
   {{0xffffffff800259d5}, {"_Z31HandleSyscallModuleBufferDeletej"}}, 
   {{0xffffffff800259db}, {"_Z33HandleSyscallModuleMessageHandlerm"}}, 
   {{0xffffffff80025a36}, {"_Z30HandleSyscallModuleMessageSendjjPvm"}}, 
   {{0xffffffff80025c81}, {"_Z34HandleSyscallModuleSectionRegisterPKc"}}, 
   {{0xffffffff80025d76}, {"_Z29HandleSyscallModuleSectionGetPKcPjS1_"}}, 
   {{0xffffffff80025e36}, {"_Z36HandleSyscallModuleSectionUnregisterPKc"}}, 
   {{0xffffffff80025f09}, {"_Z21HandleSyscallFileOpenPcPmS0_"}}, 
   {{0xffffffff80025fa3}, {"_Z21HandleSyscallFileReadPcmm"}}, 
   {{0xffffffff80026057}, {"_Z22HandleSyscallFileWritem"}}, 
   {{0xffffffff8002605d}, {"_Z22HandleSyscallFileClosem"}}, 
   {{0xffffffff80026063}, {"_Z25HandleSyscallKernOverridem"}}, 
   {{0xffffffff80026069}, {"HandleSyscall"}}, 
   {{0xffffffff800263a6}, {"_ZL10EnableSMIDv"}}, 
   {{0xffffffff80026464}, {"_ZN6x86_646x86CPU4InitEv"}}, 
   {{0xffffffff800264a8}, {"_ZN6x86_646x86CPU9GetVendorEPc"}}, 
   {{0xffffffff80026502}, {"_ZN6x86_647TSSInitEm"}}, 
   {{0xffffffff8002659c}, {"_ZN6x86_647LoadGDTEm"}}, 
   {{0xffffffff800265df}, {"_Z15InitializeStackPvm"}}, 
   {{0xffffffff800265ee}, {"_ZN6x86_6411GetAPICBaseEv"}}, 
   {{0xffffffff800265fb}, {"_ZN6x86_6411SetAPICBaseEm"}}, 
   {{0xffffffff80026615}, {"_ZN6x86_6417WriteAPICRegisterEtj"}}, 
   {{0xffffffff80026632}, {"_ZN6x86_6416ReadAPICRegisterEt"}}, 
   {{0xffffffff8002664e}, {"_ZN6x86_6412SetAPICTimerEj"}}, 
   {{0xffffffff80026655}, {"_ZN6x86_648WaitAPICEv"}}, 
   {{0xffffffff8002666b}, {"_ZN6x86_6410EnableAPICEv"}}, 
   {{0xffffffff80026718}, {"_ZN6x86_6411SendAPICEOIEv"}}, 
   {{0xffffffff8002672d}, {"_Z20CalibrateTSCWithHPETmPm"}}, 
   {{0xffffffff800267fe}, {"_ZN6x86_646IOAPICC1Em"}}, 
   {{0xffffffff800267fe}, {"_ZN6x86_646IOAPICC2Em"}}, 
   {{0xffffffff80026812}, {"_ZN6x86_646IOAPIC10ReadIOAPICEj"}}, 
   {{0xffffffff80026820}, {"_ZN6x86_646IOAPIC11WriteIOAPICEjj"}}, 
   {{0xffffffff8002682e}, {"_ZN6x86_646IOAPIC5IoctlEmz"}}, 
   {{0xffffffff80026907}, {"_ZN6x86_6410HandleMADTEP10MADTHeader"}}, 
   {{0xffffffff80026a59}, {"_ZN6x86_6410HandleSRATEP9SDTHeader"}}, 
   {{0xffffffff80026a9d}, {"_ZN6x86_6410HandleHPETEP10HPETHeader"}}, 
   {{0xffffffff80026b87}, {"_ZN6x86_6411InitDevicesEv"}}, 
   {{0xffffffff80026c90}, {"_ZN6x86_643PIT5IoctlEmz"}}, 
   {{0xffffffff80026ce6}, {"_ZN6x86_643PITC1Ev"}}, 
   {{0xffffffff80026ce6}, {"_ZN6x86_643PITC2Ev"}}, 
   {{0xffffffff80026cf5}, {"_ZL16IDTSetDescriptorhPvh"}}, 
   {{0xffffffff80026d3d}, {"_ZL9PrintRegsP9CPUStatus"}}, 
   {{0xffffffff80026daa}, {"_ZN6x86_647IDTInitEv"}}, 
   {{0xffffffff80026e28}, {"InterruptHandler"}}, 
   {{0xffffffff800270d5}, {"_ZN6x86_644OutBEth"}}, 
   {{0xffffffff800270db}, {"_ZN6x86_644OutWEtt"}}, 
   {{0xffffffff800270e2}, {"_ZN6x86_644OutDEtj"}}, 
   {{0xffffffff800270e8}, {"_ZN6x86_643InBEt"}}, 
   {{0xffffffff800270ec}, {"_ZN6x86_643InWEt"}}, 
   {{0xffffffff800270f1}, {"_ZN6x86_643InDEt"}}, 
   {{0xffffffff800270f5}, {"_ZN6x86_646IoWaitEv"}}, 
   {{0xffffffff8002710a}, {"_ZN6x86_649EarlyInitEv"}}, 
   {{0xffffffff8002715f}, {"_ZN6x86_644InitEv"}}, 
   {{0xffffffff800271d8}, {"_ZN14PageMapIndexerC1Em"}}, 
   {{0xffffffff800271d8}, {"_ZN14PageMapIndexerC2Em"}}, 
   {{0xffffffff80027220}, {"_ZN16PageTableManager4ForkEPN3VMM12VirtualSpaceEb"}}, 
   {{0xffffffff80027600}, {"_ZN16PageTableManager9MapMemoryEPvS0_m"}}, 
   {{0xffffffff80027980}, {"_ZN16PageTableManager11UnmapMemoryEPv"}}, 
   {{0xffffffff80027a90}, {"_ZN16PageTableManager18GetPhysicalAddressEPv"}}, 
   {{0xffffffff80027b96}, {"_ZN16PageTableManagerC1EP9PageTable"}}, 
   {{0xffffffff80027b96}, {"_ZN16PageTableManagerC2EP9PageTable"}}, 
   {{0xffffffff80027baa}, {"_ZN16PageTableManager13GetTopAddressEv"}}, 
   {{0xffffffff80027bb0}, {"_ZN18PageDirectoryEntry7SetFlagE7PT_Flagb"}}, 
   {{0xffffffff80027bd8}, {"_ZN18PageDirectoryEntry7GetFlagE7PT_Flag"}}, 
   {{0xffffffff80027bee}, {"_ZN18PageDirectoryEntry10SetAddressEm"}}, 
   {{0xffffffff80027c16}, {"_ZN18PageDirectoryEntry10GetAddressEv"}}, 
   {{0xffffffff80027c2b}, {"_ZN6x86_6415NewVirtualSpaceEv"}}, 
   {{0xffffffff80027c6c}, {"_ZN6x86_6416LoadVirtualSpaceEPN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80027c80}, {"CPUPause"}}, 
   {{0xffffffff80027c90}, {"FlushGDT"}}, 
   {{0xffffffff80027c93}, {"reloadSegments"}}, 
   {{0xffffffff80027c9f}, {"reloadSegments.reloadCS"}}, 
   {{0xffffffff80027cae}, {"FlushTSS"}}, 
   {{0xffffffff80027cc0}, {"EnableSCE"}}, 
   {{0xffffffff80027d00}, {"EnableSSE"}}, 
   {{0xffffffff80027d1b}, {"EnableAVX"}}, 
   {{0xffffffff80027d30}, {"SwitchStack"}}, 
   {{0xffffffff80027d70}, {"EnterUserspace"}}, 
   {{0xffffffff80027d80}, {"isr_stub_0"}}, 
   {{0xffffffff80027dc1}, {"isr_stub_1"}}, 
   {{0xffffffff80027e02}, {"isr_stub_2"}}, 
   {{0xffffffff80027e43}, {"isr_stub_3"}}, 
   {{0xffffffff80027e84}, {"isr_stub_4"}}, 
   {{0xffffffff80027ec5}, {"isr_stub_5"}}, 
   {{0xffffffff80027f06}, {"isr_stub_6"}}, 
   {{0xffffffff80027f47}, {"isr_stub_7"}}, 
   {{0xffffffff80027f88}, {"isr_stub_8"}}, 
   {{0xffffffff80027fc7}, {"isr_stub_9"}}, 
   {{0xffffffff80028008}, {"isr_stub_10"}}, 
   {{0xffffffff80028047}, {"isr_stub_11"}}, 
   {{0xffffffff80028086}, {"isr_stub_12"}}, 
   {{0xffffffff800280c5}, {"isr_stub_13"}}, 
   {{0xffffffff80028104}, {"isr_stub_14"}}, 
   {{0xffffffff80028143}, {"isr_stub_15"}}, 
   {{0xffffffff80028184}, {"isr_stub_16"}}, 
   {{0xffffffff800281c5}, {"isr_stub_17"}}, 
   {{0xffffffff80028204}, {"isr_stub_18"}}, 
   {{0xffffffff80028245}, {"isr_stub_19"}}, 
   {{0xffffffff80028286}, {"isr_stub_20"}}, 
   {{0xffffffff800282c7}, {"isr_stub_21"}}, 
   {{0xffffffff80028308}, {"isr_stub_22"}}, 
   {{0xffffffff80028349}, {"isr_stub_23"}}, 
   {{0xffffffff8002838a}, {"isr_stub_24"}}, 
   {{0xffffffff800283cb}, {"isr_stub_25"}}, 
   {{0xffffffff8002840c}, {"isr_stub_26"}}, 
   {{0xffffffff8002844d}, {"isr_stub_27"}}, 
   {{0xffffffff8002848e}, {"isr_stub_28"}}, 
   {{0xffffffff800284cf}, {"isr_stub_29"}}, 
   {{0xffffffff80028510}, {"isr_stub_30"}}, 
   {{0xffffffff8002854f}, {"isr_stub_31"}}, 
   {{0xffffffff80028590}, {"isr_stub_32"}}, 
   {{0xffffffff800285d1}, {"isr_stub_33"}}, 
   {{0xffffffff80028612}, {"isr_stub_34"}}, 
   {{0xffffffff80028653}, {"isr_stub_35"}}, 
   {{0xffffffff80028694}, {"isr_stub_36"}}, 
   {{0xffffffff800286d5}, {"isr_stub_37"}}, 
   {{0xffffffff80028716}, {"isr_stub_38"}}, 
   {{0xffffffff80028757}, {"isr_stub_39"}}, 
   {{0xffffffff80028798}, {"isr_stub_40"}}, 
   {{0xffffffff800287d9}, {"isr_stub_41"}}, 
   {{0xffffffff8002881a}, {"isr_stub_42"}}, 
   {{0xffffffff8002885b}, {"isr_stub_43"}}, 
   {{0xffffffff8002889c}, {"isr_stub_44"}}, 
   {{0xffffffff800288dd}, {"isr_stub_45"}}, 
   {{0xffffffff8002891e}, {"isr_stub_46"}}, 
   {{0xffffffff8002895f}, {"isr_stub_47"}}, 
   {{0xffffffff800289a0}, {"isr_stub_48"}}, 
   {{0xffffffff800289e1}, {"isr_stub_49"}}, 
   {{0xffffffff80028a22}, {"isr_stub_50"}}, 
   {{0xffffffff80028a63}, {"isr_stub_51"}}, 
   {{0xffffffff80028aa4}, {"isr_stub_52"}}, 
   {{0xffffffff80028ae5}, {"isr_stub_53"}}, 
   {{0xffffffff80028b26}, {"isr_stub_54"}}, 
   {{0xffffffff80028b67}, {"isr_stub_55"}}, 
   {{0xffffffff80028ba8}, {"isr_stub_56"}}, 
   {{0xffffffff80028be9}, {"isr_stub_57"}}, 
   {{0xffffffff80028c2a}, {"isr_stub_58"}}, 
   {{0xffffffff80028c6b}, {"isr_stub_59"}}, 
   {{0xffffffff80028cac}, {"isr_stub_60"}}, 
   {{0xffffffff80028ced}, {"isr_stub_61"}}, 
   {{0xffffffff80028d2e}, {"isr_stub_62"}}, 
   {{0xffffffff80028d6f}, {"isr_stub_63"}}, 
   {{0xffffffff80028db0}, {"isr_stub_64"}}, 
   {{0xffffffff80028df1}, {"isr_stub_65"}}, 
   {{0xffffffff80028e32}, {"isr_stub_66"}}, 
   {{0xffffffff80028e73}, {"isr_stub_67"}}, 
   {{0xffffffff80028eb4}, {"isr_stub_68"}}, 
   {{0xffffffff80028ef5}, {"isr_stub_69"}}, 
   {{0xffffffff80028f36}, {"isr_stub_70"}}, 
   {{0xffffffff80028f77}, {"isr_stub_71"}}, 
   {{0xffffffff80028fb8}, {"isr_stub_72"}}, 
   {{0xffffffff80028ff9}, {"isr_stub_73"}}, 
   {{0xffffffff8002903a}, {"isr_stub_74"}}, 
   {{0xffffffff8002907b}, {"isr_stub_75"}}, 
   {{0xffffffff800290bc}, {"isr_stub_76"}}, 
   {{0xffffffff800290fd}, {"isr_stub_77"}}, 
   {{0xffffffff8002913e}, {"isr_stub_78"}}, 
   {{0xffffffff8002917f}, {"isr_stub_79"}}, 
   {{0xffffffff800291c0}, {"isr_stub_80"}}, 
   {{0xffffffff80029201}, {"isr_stub_81"}}, 
   {{0xffffffff80029242}, {"isr_stub_82"}}, 
   {{0xffffffff80029283}, {"isr_stub_83"}}, 
   {{0xffffffff800292c4}, {"isr_stub_84"}}, 
   {{0xffffffff80029305}, {"isr_stub_85"}}, 
   {{0xffffffff80029346}, {"isr_stub_86"}}, 
   {{0xffffffff80029387}, {"isr_stub_87"}}, 
   {{0xffffffff800293c8}, {"isr_stub_88"}}, 
   {{0xffffffff80029409}, {"isr_stub_89"}}, 
   {{0xffffffff8002944a}, {"isr_stub_90"}}, 
   {{0xffffffff8002948b}, {"isr_stub_91"}}, 
   {{0xffffffff800294cc}, {"isr_stub_92"}}, 
   {{0xffffffff8002950d}, {"isr_stub_93"}}, 
   {{0xffffffff8002954e}, {"isr_stub_94"}}, 
   {{0xffffffff8002958f}, {"isr_stub_95"}}, 
   {{0xffffffff800295d0}, {"isr_stub_96"}}, 
   {{0xffffffff80029611}, {"isr_stub_97"}}, 
   {{0xffffffff80029652}, {"isr_stub_98"}}, 
   {{0xffffffff80029693}, {"isr_stub_99"}}, 
   {{0xffffffff800296d4}, {"isr_stub_100"}}, 
   {{0xffffffff80029715}, {"isr_stub_101"}}, 
   {{0xffffffff80029756}, {"isr_stub_102"}}, 
   {{0xffffffff80029797}, {"isr_stub_103"}}, 
   {{0xffffffff800297d8}, {"isr_stub_104"}}, 
   {{0xffffffff80029819}, {"isr_stub_105"}}, 
   {{0xffffffff8002985a}, {"isr_stub_106"}}, 
   {{0xffffffff8002989b}, {"isr_stub_107"}}, 
   {{0xffffffff800298dc}, {"isr_stub_108"}}, 
   {{0xffffffff8002991d}, {"isr_stub_109"}}, 
   {{0xffffffff8002995e}, {"isr_stub_110"}}, 
   {{0xffffffff8002999f}, {"isr_stub_111"}}, 
   {{0xffffffff800299e0}, {"isr_stub_112"}}, 
   {{0xffffffff80029a21}, {"isr_stub_113"}}, 
   {{0xffffffff80029a62}, {"isr_stub_114"}}, 
   {{0xffffffff80029aa3}, {"isr_stub_115"}}, 
   {{0xffffffff80029ae4}, {"isr_stub_116"}}, 
   {{0xffffffff80029b25}, {"isr_stub_117"}}, 
   {{0xffffffff80029b66}, {"isr_stub_118"}}, 
   {{0xffffffff80029ba7}, {"isr_stub_119"}}, 
   {{0xffffffff80029be8}, {"isr_stub_120"}}, 
   {{0xffffffff80029c29}, {"isr_stub_121"}}, 
   {{0xffffffff80029c6a}, {"isr_stub_122"}}, 
   {{0xffffffff80029cab}, {"isr_stub_123"}}, 
   {{0xffffffff80029cec}, {"isr_stub_124"}}, 
   {{0xffffffff80029d2d}, {"isr_stub_125"}}, 
   {{0xffffffff80029d6e}, {"isr_stub_126"}}, 
   {{0xffffffff80029daf}, {"isr_stub_127"}}, 
   {{0xffffffff80029df0}, {"isr_stub_128"}}, 
   {{0xffffffff80029e34}, {"isr_stub_129"}}, 
   {{0xffffffff80029e78}, {"isr_stub_130"}}, 
   {{0xffffffff80029ebc}, {"isr_stub_131"}}, 
   {{0xffffffff80029f00}, {"isr_stub_132"}}, 
   {{0xffffffff80029f44}, {"isr_stub_133"}}, 
   {{0xffffffff80029f88}, {"isr_stub_134"}}, 
   {{0xffffffff80029fcc}, {"isr_stub_135"}}, 
   {{0xffffffff8002a010}, {"isr_stub_136"}}, 
   {{0xffffffff8002a054}, {"isr_stub_137"}}, 
   {{0xffffffff8002a098}, {"isr_stub_138"}}, 
   {{0xffffffff8002a0dc}, {"isr_stub_139"}}, 
   {{0xffffffff8002a120}, {"isr_stub_140"}}, 
   {{0xffffffff8002a164}, {"isr_stub_141"}}, 
   {{0xffffffff8002a1a8}, {"isr_stub_142"}}, 
   {{0xffffffff8002a1ec}, {"isr_stub_143"}}, 
   {{0xffffffff8002a230}, {"isr_stub_144"}}, 
   {{0xffffffff8002a274}, {"isr_stub_145"}}, 
   {{0xffffffff8002a2b8}, {"isr_stub_146"}}, 
   {{0xffffffff8002a2fc}, {"isr_stub_147"}}, 
   {{0xffffffff8002a340}, {"isr_stub_148"}}, 
   {{0xffffffff8002a384}, {"isr_stub_149"}}, 
   {{0xffffffff8002a3c8}, {"isr_stub_150"}}, 
   {{0xffffffff8002a40c}, {"isr_stub_151"}}, 
   {{0xffffffff8002a450}, {"isr_stub_152"}}, 
   {{0xffffffff8002a494}, {"isr_stub_153"}}, 
   {{0xffffffff8002a4d8}, {"isr_stub_154"}}, 
   {{0xffffffff8002a51c}, {"isr_stub_155"}}, 
   {{0xffffffff8002a560}, {"isr_stub_156"}}, 
   {{0xffffffff8002a5a4}, {"isr_stub_157"}}, 
   {{0xffffffff8002a5e8}, {"isr_stub_158"}}, 
   {{0xffffffff8002a62c}, {"isr_stub_159"}}, 
   {{0xffffffff8002a670}, {"isr_stub_160"}}, 
   {{0xffffffff8002a6b4}, {"isr_stub_161"}}, 
   {{0xffffffff8002a6f8}, {"isr_stub_162"}}, 
   {{0xffffffff8002a73c}, {"isr_stub_163"}}, 
   {{0xffffffff8002a780}, {"isr_stub_164"}}, 
   {{0xffffffff8002a7c4}, {"isr_stub_165"}}, 
   {{0xffffffff8002a808}, {"isr_stub_166"}}, 
   {{0xffffffff8002a84c}, {"isr_stub_167"}}, 
   {{0xffffffff8002a890}, {"isr_stub_168"}}, 
   {{0xffffffff8002a8d4}, {"isr_stub_169"}}, 
   {{0xffffffff8002a918}, {"isr_stub_170"}}, 
   {{0xffffffff8002a95c}, {"isr_stub_171"}}, 
   {{0xffffffff8002a9a0}, {"isr_stub_172"}}, 
   {{0xffffffff8002a9e4}, {"isr_stub_173"}}, 
   {{0xffffffff8002aa28}, {"isr_stub_174"}}, 
   {{0xffffffff8002aa6c}, {"isr_stub_175"}}, 
   {{0xffffffff8002aab0}, {"isr_stub_176"}}, 
   {{0xffffffff8002aaf4}, {"isr_stub_177"}}, 
   {{0xffffffff8002ab38}, {"isr_stub_178"}}, 
   {{0xffffffff8002ab7c}, {"isr_stub_179"}}, 
   {{0xffffffff8002abc0}, {"isr_stub_180"}}, 
   {{0xffffffff8002ac04}, {"isr_stub_181"}}, 
   {{0xffffffff8002ac48}, {"isr_stub_182"}}, 
   {{0xffffffff8002ac8c}, {"isr_stub_183"}}, 
   {{0xffffffff8002acd0}, {"isr_stub_184"}}, 
   {{0xffffffff8002ad14}, {"isr_stub_185"}}, 
   {{0xffffffff8002ad58}, {"isr_stub_186"}}, 
   {{0xffffffff8002ad9c}, {"isr_stub_187"}}, 
   {{0xffffffff8002ade0}, {"isr_stub_188"}}, 
   {{0xffffffff8002ae24}, {"isr_stub_189"}}, 
   {{0xffffffff8002ae68}, {"isr_stub_190"}}, 
   {{0xffffffff8002aeac}, {"isr_stub_191"}}, 
   {{0xffffffff8002aef0}, {"isr_stub_192"}}, 
   {{0xffffffff8002af34}, {"isr_stub_193"}}, 
   {{0xffffffff8002af78}, {"isr_stub_194"}}, 
   {{0xffffffff8002afbc}, {"isr_stub_195"}}, 
   {{0xffffffff8002b000}, {"isr_stub_196"}}, 
   {{0xffffffff8002b044}, {"isr_stub_197"}}, 
   {{0xffffffff8002b088}, {"isr_stub_198"}}, 
   {{0xffffffff8002b0cc}, {"isr_stub_199"}}, 
   {{0xffffffff8002b110}, {"isr_stub_200"}}, 
   {{0xffffffff8002b154}, {"isr_stub_201"}}, 
   {{0xffffffff8002b198}, {"isr_stub_202"}}, 
   {{0xffffffff8002b1dc}, {"isr_stub_203"}}, 
   {{0xffffffff8002b220}, {"isr_stub_204"}}, 
   {{0xffffffff8002b264}, {"isr_stub_205"}}, 
   {{0xffffffff8002b2a8}, {"isr_stub_206"}}, 
   {{0xffffffff8002b2ec}, {"isr_stub_207"}}, 
   {{0xffffffff8002b330}, {"isr_stub_208"}}, 
   {{0xffffffff8002b374}, {"isr_stub_209"}}, 
   {{0xffffffff8002b3b8}, {"isr_stub_210"}}, 
   {{0xffffffff8002b3fc}, {"isr_stub_211"}}, 
   {{0xffffffff8002b440}, {"isr_stub_212"}}, 
   {{0xffffffff8002b484}, {"isr_stub_213"}}, 
   {{0xffffffff8002b4c8}, {"isr_stub_214"}}, 
   {{0xffffffff8002b50c}, {"isr_stub_215"}}, 
   {{0xffffffff8002b550}, {"isr_stub_216"}}, 
   {{0xffffffff8002b594}, {"isr_stub_217"}}, 
   {{0xffffffff8002b5d8}, {"isr_stub_218"}}, 
   {{0xffffffff8002b61c}, {"isr_stub_219"}}, 
   {{0xffffffff8002b660}, {"isr_stub_220"}}, 
   {{0xffffffff8002b6a4}, {"isr_stub_221"}}, 
   {{0xffffffff8002b6e8}, {"isr_stub_222"}}, 
   {{0xffffffff8002b72c}, {"isr_stub_223"}}, 
   {{0xffffffff8002b770}, {"isr_stub_224"}}, 
   {{0xffffffff8002b7b4}, {"isr_stub_225"}}, 
   {{0xffffffff8002b7f8}, {"isr_stub_226"}}, 
   {{0xffffffff8002b83c}, {"isr_stub_227"}}, 
   {{0xffffffff8002b880}, {"isr_stub_228"}}, 
   {{0xffffffff8002b8c4}, {"isr_stub_229"}}, 
   {{0xffffffff8002b908}, {"isr_stub_230"}}, 
   {{0xffffffff8002b94c}, {"isr_stub_231"}}, 
   {{0xffffffff8002b990}, {"isr_stub_232"}}, 
   {{0xffffffff8002b9d4}, {"isr_stub_233"}}, 
   {{0xffffffff8002ba18}, {"isr_stub_234"}}, 
   {{0xffffffff8002ba5c}, {"isr_stub_235"}}, 
   {{0xffffffff8002baa0}, {"isr_stub_236"}}, 
   {{0xffffffff8002bae4}, {"isr_stub_237"}}, 
   {{0xffffffff8002bb28}, {"isr_stub_238"}}, 
   {{0xffffffff8002bb6c}, {"isr_stub_239"}}, 
   {{0xffffffff8002bbb0}, {"isr_stub_240"}}, 
   {{0xffffffff8002bbf4}, {"isr_stub_241"}}, 
   {{0xffffffff8002bc38}, {"isr_stub_242"}}, 
   {{0xffffffff8002bc7c}, {"isr_stub_243"}}, 
   {{0xffffffff8002bcc0}, {"isr_stub_244"}}, 
   {{0xffffffff8002bd04}, {"isr_stub_245"}}, 
   {{0xffffffff8002bd48}, {"isr_stub_246"}}, 
   {{0xffffffff8002bd8c}, {"isr_stub_247"}}, 
   {{0xffffffff8002bdd0}, {"isr_stub_248"}}, 
   {{0xffffffff8002be14}, {"isr_stub_249"}}, 
   {{0xffffffff8002be58}, {"isr_stub_250"}}, 
   {{0xffffffff8002be9c}, {"isr_stub_251"}}, 
   {{0xffffffff8002bee0}, {"isr_stub_252"}}, 
   {{0xffffffff8002bf24}, {"isr_stub_253"}}, 
   {{0xffffffff8002bf68}, {"isr_stub_254"}}, 
   {{0xffffffff8002bfac}, {"isr_stub_255"}}, 
   {{0xffffffff8002bff0}, {"isrStubTable"}}, 
   {{0xffffffff8002d978}, {"_ZTV10UARTDevice"}}, 
   {{0xffffffff8002dc90}, {"_ZTV5Mutex"}}, 
   {{0xffffffff8002dcb8}, {"_ZTV8SpinLock"}}, 
   {{0xffffffff8002dd80}, {"_ZL14memTypeStrings"}}, 
   {{0xffffffff800315a0}, {"symbols"}}, 
   {{0xffffffff80033c60}, {"symbolCount"}}, 
   {{0xffffffff80033df0}, {"_ZTV6Device"}}, 
   {{0xffffffff80033e08}, {"_ZTVN6x86_646IOAPICE"}}, 
   {{0xffffffff80034268}, {"_ZTVN6x86_643PITE"}}, 
   {{0xffffffff800346e0}, {"_ZTVN3VMM12VirtualSpaceE"}}, 
   {{0xffffffff80034718}, {"_ZTV16PageTableManager"}}, 
   {{0xffffffff80036000}, {"_ZL11timeRequest"}}, 
   {{0xffffffff80036040}, {"_ZL17kernelFileRequest"}}, 
   {{0xffffffff80036080}, {"_ZL18framebufferRequest"}}, 
   {{0xffffffff800360c0}, {"_ZL11rsdpRequest"}}, 
   {{0xffffffff80036100}, {"_ZL12kaddrRequest"}}, 
   {{0xffffffff80036140}, {"_ZL11mMapRequest"}}, 
   {{0xffffffff80036180}, {"_ZL11hhdmRequest"}}, 
   {{0xffffffff800361c0}, {"_ZL13moduleRequest"}}, 
   {{0xffffffff80036200}, {"_ZL12stackRequest"}}, 
   {{0xffffffff80036240}, {"_ZL17entryPointRequest"}}, 
   {{0xffffffff80036278}, {"bootmemStatus"}}, 
   {{0xffffffff80036280}, {"_ZN4PROCL10CurrentPIDE"}}, 
   {{0xffffffff800362a0}, {"font"}}, 
   {{0xffffffff800366a0}, {"StartSyscallStack"}}, 
   {{0xffffffff80037000}, {"gdt"}}, 
   {{0xffffffff80038000}, {"__stack_chk_guard"}}, 
   {{0xffffffff80038008}, {"info"}}, 
   {{0xffffffff80038010}, {"lastPosition"}}, 
   {{0xffffffff80038020}, {"bootmemMemory"}}, 
   {{0xffffffff80039020}, {"_ZL8HeapLock"}}, 
   {{0xffffffff80039030}, {"_ZL8totalMem"}}, 
   {{0xffffffff80039038}, {"_ZL7freeMem"}}, 
   {{0xffffffff80039040}, {"_ZL12isHeapActive"}}, 
   {{0xffffffff80039048}, {"_ZL10lastHeader"}}, 
   {{0xffffffff80039050}, {"_ZL7heapEnd"}}, 
   {{0xffffffff80039058}, {"_ZL9heapStart"}}, 
   {{0xffffffff80039060}, {"_ZL10HigherHalf"}}, 
   {{0xffffffff80039070}, {"_ZL10BitmapLock"}}, 
   {{0xffffffff80039080}, {"_ZL17page_bitmap_index"}}, 
   {{0xffffffff80039090}, {"_ZL11page_bitmap"}}, 
   {{0xffffffff800390a0}, {"_ZL11initialized"}}, 
   {{0xffffffff800390a8}, {"_ZL11used_memory"}}, 
   {{0xffffffff800390b0}, {"_ZL15reserved_memory"}}, 
   {{0xffffffff800390b8}, {"_ZL11free_memory"}}, 
   {{0xffffffff800390c0}, {"_ZZ6strtokPcS_E1p"}}, 
   {{0xffffffff800390d0}, {"vid"}}, 
   {{0xffffffff800390e0}, {"fbData"}}, 
   {{0xffffffff800390f8}, {"_ZL9globalRow"}}, 
   {{0xffffffff800390fc}, {"_ZL9globalCol"}}, 
   {{0xffffffff80039100}, {"__dso_handle"}}, 
   {{0xffffffff80039108}, {"__atexit_func_count"}}, 
   {{0xffffffff80039120}, {"__atexit_funcs"}}, 
   {{0xffffffff80039d20}, {"kernelPort"}}, 
   {{0xffffffff80039d28}, {"_ZN6PRINTKL16TerminalPositionE"}}, 
   {{0xffffffff80039d40}, {"_ZN6PRINTKL14TerminalColumnE"}}, 
   {{0xffffffff80039e40}, {"_ZN6PRINTKL14PrintKSpinlockE"}}, 
   {{0xffffffff8003a000}, {"gdtPointer"}}, 
   {{0xffffffff8003b000}, {"tss"}}, 
   {{0xffffffff8003b068}, {"_ZN6x86_64L6cyclesE"}}, 
   {{0xffffffff8003b070}, {"idtr"}}, 
   {{0xffffffff8003b080}, {"idt"}}, 
};
