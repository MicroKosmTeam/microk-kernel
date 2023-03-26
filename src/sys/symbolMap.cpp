#include <sys/symbol.hpp>
const uint64_t symbolCount = 224;
const Symbol symbols[] = {
   {{0xffffffff80000000}, {"_ZN6x86_646x86CPU8CheckSSEEi"}}, 
   {{0xffffffff80000000}, {"kernel_start_addr"}}, 
   {{0xffffffff80000000}, {"text_start_addr"}}, 
   {{0xffffffff800000a2}, {"_ZN6x86_646x86CPU8CheckAVXEv"}}, 
   {{0xffffffff800000bc}, {"_ZN6x86_646x86CPU4InitEv"}}, 
   {{0xffffffff800001f8}, {"_ZN6x86_646x86CPU9GetVendorEPc"}}, 
   {{0xffffffff80000252}, {"_ZN6x86_647TSSInitEm"}}, 
   {{0xffffffff800002c7}, {"_ZN6x86_647LoadGDTEm"}}, 
   {{0xffffffff800002f7}, {"_ZL16IDTSetDescriptorhPvh"}}, 
   {{0xffffffff8000033f}, {"_ZN6x86_647IDTInitEv"}}, 
   {{0xffffffff80000391}, {"pageFaultHandler"}}, 
   {{0xffffffff800003b4}, {"exceptionHandler"}}, 
   {{0xffffffff800003d7}, {"_ZN6x86_644OutBEth"}}, 
   {{0xffffffff800003dd}, {"_ZN6x86_644OutWEtt"}}, 
   {{0xffffffff800003e4}, {"_ZN6x86_643InBEt"}}, 
   {{0xffffffff800003e8}, {"_ZN6x86_643InWEt"}}, 
   {{0xffffffff800003ed}, {"_ZN6x86_646IoWaitEv"}}, 
   {{0xffffffff80000402}, {"_ZN6x86_644InitEv"}}, 
   {{0xffffffff800004d0}, {"_ZN14PageMapIndexerC1Em"}}, 
   {{0xffffffff800004d0}, {"_ZN14PageMapIndexerC2Em"}}, 
   {{0xffffffff8000050e}, {"_ZN16PageTableManagerC1EP9PageTable"}}, 
   {{0xffffffff8000050e}, {"_ZN16PageTableManagerC2EP9PageTable"}}, 
   {{0xffffffff80000512}, {"_ZN16PageTableManager9MapMemoryEPvS0_"}}, 
   {{0xffffffff80000738}, {"_ZN16PageTableManager9MapMemoryEPvS0_hP7PT_FlagPb"}}, 
   {{0xffffffff80000992}, {"_ZN18PageDirectoryEntry7SetFlagE7PT_Flagb"}}, 
   {{0xffffffff800009ba}, {"_ZN18PageDirectoryEntry7GetFlagE7PT_Flag"}}, 
   {{0xffffffff800009d0}, {"_ZN18PageDirectoryEntry10SetAddressEm"}}, 
   {{0xffffffff800009f8}, {"_ZN18PageDirectoryEntry10GetAddressEv"}}, 
   {{0xffffffff80000a0d}, {"_ZN6x86_647InitVMMEv"}}, 
   {{0xffffffff80000ad2}, {"EfiMain"}}, 
   {{0xffffffff80000aee}, {"LimineEntry"}}, 
   {{0xffffffff80000d6f}, {"__stack_chk_fail"}}, 
   {{0xffffffff80000da3}, {"_Z11UnwindStacki"}}, 
   {{0xffffffff80000e33}, {"_ZL13PrintLocationP14SourceLocation"}}, 
   {{0xffffffff80000e53}, {"__ubsan_handle_missing_return"}}, 
   {{0xffffffff80000e54}, {"__ubsan_handle_divrem_overflow"}}, 
   {{0xffffffff80000e55}, {"__ubsan_handle_load_invalid_value"}}, 
   {{0xffffffff80000e56}, {"__ubsan_handle_shift_out_of_bounds"}}, 
   {{0xffffffff80000e57}, {"__ubsan_handle_builtin_unreachable"}}, 
   {{0xffffffff80000e58}, {"__ubsan_handle_mul_overflow"}}, 
   {{0xffffffff80000e59}, {"__ubsan_handle_negate_overflow"}}, 
   {{0xffffffff80000e5a}, {"__ubsan_handle_out_of_bounds"}}, 
   {{0xffffffff80000e85}, {"__ubsan_handle_pointer_overflow"}}, 
   {{0xffffffff80000e86}, {"__ubsan_handle_add_overflow"}}, 
   {{0xffffffff80000e87}, {"__ubsan_handle_sub_overflow"}}, 
   {{0xffffffff80000e88}, {"__ubsan_handle_type_mismatch_v1"}}, 
   {{0xffffffff80000f60}, {"_ZN10UARTDevice4InitE11SerialPorts"}}, 
   {{0xffffffff8000104e}, {"_ZN10UARTDevice15isTransmitEmptyEv"}}, 
   {{0xffffffff80001066}, {"_ZN10UARTDevice7PutCharEc"}}, 
   {{0xffffffff80001098}, {"_ZN10UARTDevice6PutStrEPKc"}}, 
   {{0xffffffff80001102}, {"_ZN10UARTDevice14serialReceivedEv"}}, 
   {{0xffffffff8000111a}, {"_ZN10UARTDevice7GetCharEv"}}, 
   {{0xffffffff80001152}, {"_ZN10UARTDevice5IoctlEmP13__va_list_tag"}}, 
   {{0xffffffff80001201}, {"_Z8InitInfov"}}, 
   {{0xffffffff80001218}, {"_Z7GetInfov"}}, 
   {{0xffffffff80001220}, {"UserFunction"}}, 
   {{0xffffffff80001222}, {"_Z15EarlyKernelInitv"}}, 
   {{0xffffffff80001237}, {"_Z8RestInitv"}}, 
   {{0xffffffff80001274}, {"_Z11KernelStartv"}}, 
   {{0xffffffff80001355}, {"_ZN6MODULE4InitEv"}}, 
   {{0xffffffff800014be}, {"_ZN7BOOTMEM17DeactivateBootmemEv"}}, 
   {{0xffffffff800014c5}, {"_ZN7BOOTMEM15BootmemIsActiveEv"}}, 
   {{0xffffffff800014cd}, {"_ZN7BOOTMEM6MallocEm"}}, 
   {{0xffffffff800014fa}, {"_ZN7BOOTMEM4FreeEPv"}}, 
   {{0xffffffff800014fb}, {"_ZN7BOOTMEM7GetFreeEv"}}, 
   {{0xffffffff80001507}, {"_ZN7BOOTMEM8GetTotalEv"}}, 
   {{0xffffffff8000150e}, {"_ZN4HEAP12IsHeapActiveEv"}}, 
   {{0xffffffff80001516}, {"_ZN4HEAP7GetFreeEv"}}, 
   {{0xffffffff8000151e}, {"_ZN4HEAP8GetTotalEv"}}, 
   {{0xffffffff80001526}, {"_ZN13HeapSegHeader14CombineForwardEv"}}, 
   {{0xffffffff8000156e}, {"_ZN13HeapSegHeader15CombineBackwardEv"}}, 
   {{0xffffffff8000158a}, {"_ZN13HeapSegHeader5SplitEm"}}, 
   {{0xffffffff80001602}, {"_ZN4HEAP14InitializeHeapEPvm"}}, 
   {{0xffffffff800016ab}, {"_ZN4HEAP4FreeEPv"}}, 
   {{0xffffffff800016ef}, {"_ZN4HEAP10ExpandHeapEm"}}, 
   {{0xffffffff800017b7}, {"_ZN4HEAP6MallocEm"}}, 
   {{0xffffffff80001842}, {"_Z6memcmpPKvS0_m"}}, 
   {{0xffffffff8000186d}, {"_Z6memcpyPvS_m"}}, 
   {{0xffffffff80001889}, {"_Z6Mallocm"}}, 
   {{0xffffffff800018c6}, {"_Z4FreePv"}}, 
   {{0xffffffff800018d1}, {"_Znwm"}}, 
   {{0xffffffff8000190e}, {"_Znam"}}, 
   {{0xffffffff8000194b}, {"_ZdlPv"}}, 
   {{0xffffffff80001956}, {"_ZdlPvm"}}, 
   {{0xffffffff80001961}, {"_ZdaPv"}}, 
   {{0xffffffff8000196c}, {"_ZdaPvm"}}, 
   {{0xffffffff80001977}, {"_ZN3MEM4InitEv"}}, 
   {{0xffffffff800019fc}, {"_Z6memsetPvhm"}}, 
   {{0xffffffff80001a11}, {"_ZL10InitBitmapmPv"}}, 
   {{0xffffffff80001a3c}, {"_ZL13UnreservePagePv"}}, 
   {{0xffffffff80001aa5}, {"_ZL14UnreservePagesPvm"}}, 
   {{0xffffffff80001ae5}, {"_ZL11ReservePagePv"}}, 
   {{0xffffffff80001b3e}, {"_ZL12ReservePagesPvm"}}, 
   {{0xffffffff80001b7e}, {"_ZN3PMM22InitPageFrameAllocatorEv"}}, 
   {{0xffffffff80001cb5}, {"_ZN3PMM8FreePageEPv"}}, 
   {{0xffffffff80001d1e}, {"_ZN3PMM8LockPageEPv"}}, 
   {{0xffffffff80001d77}, {"_ZN3PMM11RequestPageEv"}}, 
   {{0xffffffff80001e29}, {"_ZN3PMM9FreePagesEPvm"}}, 
   {{0xffffffff80001e69}, {"_ZN3PMM9LockPagesEPvm"}}, 
   {{0xffffffff80001ea9}, {"_ZN3PMM12RequestPagesEm"}}, 
   {{0xffffffff80001f4d}, {"_ZN3PMM10GetFreeMemEv"}}, 
   {{0xffffffff80001f55}, {"_ZN3PMM10GetUsedMemEv"}}, 
   {{0xffffffff80001f5d}, {"_ZN3PMM14GetReservedMemEv"}}, 
   {{0xffffffff80001f65}, {"_Z6strlenPKc"}}, 
   {{0xffffffff80001f77}, {"_Z6strcpyPcPKc"}}, 
   {{0xffffffff80001fac}, {"_Z6strcmpPKcS0_"}}, 
   {{0xffffffff80001fef}, {"_Z8is_delimcPc"}}, 
   {{0xffffffff80002008}, {"_Z6strtokPcS_"}}, 
   {{0xffffffff800020a1}, {"_Z4itoaPcix"}}, 
   {{0xffffffff8000213a}, {"_Z4atoiPc"}}, 
   {{0xffffffff80002165}, {"_ZN3VMM7InitVMMEv"}}, 
   {{0xffffffff80002170}, {"_ZN3VMM9MapMemoryEPvS0_"}}, 
   {{0xffffffff800021b4}, {"_ZN6Bitmap3GetEm"}}, 
   {{0xffffffff800021f0}, {"_ZN6BitmapixEm"}}, 
   {{0xffffffff800021fc}, {"_ZN6Bitmap3SetEmb"}}, 
   {{0xffffffff8000224e}, {"_Z5IoctlP6Drivermz"}}, 
   {{0xffffffff8000228f}, {"_Z7LoadELFPh"}}, 
   {{0xffffffff800024cb}, {"__cxa_guard_acquire"}}, 
   {{0xffffffff800024d5}, {"__cxa_guard_release"}}, 
   {{0xffffffff800024d9}, {"__cxa_guard_abort"}}, 
   {{0xffffffff800024da}, {"__cxa_atexit"}}, 
   {{0xffffffff8000251d}, {"__cxa_finalize"}}, 
   {{0xffffffff800025b2}, {"_Z5PanicPKcS0_S0_j"}}, 
   {{0xffffffff80002638}, {"_ZN6PRINTK6PrintKEPcz"}}, 
   {{0xffffffff80002843}, {"_ZN6PRINTK9EarlyInitEv"}}, 
   {{0xffffffff80002899}, {"_Z12LookupSymbolPKc"}}, 
   {{0xffffffff800028ec}, {"_Z12LookupSymbolm"}}, 
   {{0xffffffff80002920}, {"ActivateAVX"}}, 
   {{0xffffffff80002940}, {"FlushGDT"}}, 
   {{0xffffffff8000295b}, {"FlushTSS"}}, 
   {{0xffffffff80002970}, {"EnableSCE"}}, 
   {{0xffffffff80002990}, {"ActivateSSE"}}, 
   {{0xffffffff800029b0}, {"EnterUserspace"}}, 
   {{0xffffffff800029bc}, {"sysretq"}}, 
   {{0xffffffff800029c0}, {"isr_stub_0"}}, 
   {{0xffffffff800029c7}, {"isr_stub_1"}}, 
   {{0xffffffff800029ce}, {"isr_stub_2"}}, 
   {{0xffffffff800029d5}, {"isr_stub_3"}}, 
   {{0xffffffff800029dc}, {"isr_stub_4"}}, 
   {{0xffffffff800029e3}, {"isr_stub_5"}}, 
   {{0xffffffff800029ea}, {"isr_stub_6"}}, 
   {{0xffffffff800029f1}, {"isr_stub_7"}}, 
   {{0xffffffff800029f8}, {"isr_stub_8"}}, 
   {{0xffffffff800029ff}, {"isr_stub_9"}}, 
   {{0xffffffff80002a06}, {"isr_stub_10"}}, 
   {{0xffffffff80002a0d}, {"isr_stub_11"}}, 
   {{0xffffffff80002a14}, {"isr_stub_12"}}, 
   {{0xffffffff80002a1b}, {"isr_stub_13"}}, 
   {{0xffffffff80002a22}, {"isr_stub_14"}}, 
   {{0xffffffff80002a29}, {"isr_stub_15"}}, 
   {{0xffffffff80002a30}, {"isr_stub_16"}}, 
   {{0xffffffff80002a37}, {"isr_stub_17"}}, 
   {{0xffffffff80002a3e}, {"isr_stub_18"}}, 
   {{0xffffffff80002a45}, {"isr_stub_19"}}, 
   {{0xffffffff80002a4c}, {"isr_stub_20"}}, 
   {{0xffffffff80002a53}, {"isr_stub_21"}}, 
   {{0xffffffff80002a5a}, {"isr_stub_22"}}, 
   {{0xffffffff80002a61}, {"isr_stub_23"}}, 
   {{0xffffffff80002a68}, {"isr_stub_24"}}, 
   {{0xffffffff80002a6f}, {"isr_stub_25"}}, 
   {{0xffffffff80002a76}, {"isr_stub_26"}}, 
   {{0xffffffff80002a7d}, {"isr_stub_27"}}, 
   {{0xffffffff80002a84}, {"isr_stub_28"}}, 
   {{0xffffffff80002a8b}, {"isr_stub_29"}}, 
   {{0xffffffff80002a92}, {"isr_stub_30"}}, 
   {{0xffffffff80002a99}, {"isr_stub_31"}}, 
   {{0xffffffff80002aa0}, {"isrStubTable"}}, 
   {{0xffffffff80002ba0}, {"JumpUsermode"}}, 
   {{0xffffffff80002bac}, {"_ZN4MKMI12BufferCreateEmNS_10BufferTypeEm"}}, 
   {{0xffffffff80002bac}, {"sysretq"}}, 
   {{0xffffffff80002c1e}, {"_ZN4MKMI8BufferIOEPNS_6BufferENS_15BufferOperationEz"}}, 
   {{0xffffffff80002d63}, {"_ZN4MKMI12BufferDeleteEPNS_6BufferE"}}, 
   {{0xffffffff80002dc4}, {"text_end_addr"}}, 
   {{0xffffffff80003dc4}, {"rodata_start_addr"}}, 
   {{0xffffffff80004370}, {"_ZTV10UARTDevice"}}, 
   {{0xffffffff80004660}, {"_ZL14memTypeStrings"}}, 
   {{0xffffffff80005a00}, {"symbols"}}, 
   {{0xffffffff80006800}, {"symbolCount"}}, 
   {{0xffffffff80006808}, {"rodata_end_addr"}}, 
   {{0xffffffff80007808}, {"data_start_addr"}}, 
   {{0xffffffff80008000}, {"gdt"}}, 
   {{0xffffffff80008060}, {"_ZL12kaddrRequest"}}, 
   {{0xffffffff800080a0}, {"_ZL11mMapRequest"}}, 
   {{0xffffffff800080e0}, {"_ZL11hhdmRequest"}}, 
   {{0xffffffff80008120}, {"_ZL13moduleRequest"}}, 
   {{0xffffffff80008160}, {"_ZL12stackRequest"}}, 
   {{0xffffffff800081a0}, {"_ZL17bootloaderRequest"}}, 
   {{0xffffffff800081e0}, {"_ZL11timeRequest"}}, 
   {{0xffffffff80008220}, {"_ZL17entryPointRequest"}}, 
   {{0xffffffff80008258}, {"__stack_chk_guard"}}, 
   {{0xffffffff80008260}, {"TypeCheckKinds"}}, 
   {{0xffffffff800082b0}, {"bootmemStatus"}}, 
   {{0xffffffff80009000}, {"tss"}}, 
   {{0xffffffff8000a000}, {"gdtPointer"}}, 
   {{0xffffffff8000a010}, {"idtr"}}, 
   {{0xffffffff8000a020}, {"idt"}}, 
   {{0xffffffff8000b020}, {"GlobalPageTableManager"}}, 
   {{0xffffffff8000b028}, {"PML4"}}, 
   {{0xffffffff8000b030}, {"initialized"}}, 
   {{0xffffffff8000b038}, {"info"}}, 
   {{0xffffffff8000b040}, {"userStack"}}, 
   {{0xffffffff8000d040}, {"KRNLSYMTABLE"}}, 
   {{0xffffffff8000d060}, {"lastPosition"}}, 
   {{0xffffffff8000d080}, {"bootmemMemory"}}, 
   {{0xffffffff8001d080}, {"_ZL8totalMem"}}, 
   {{0xffffffff8001d088}, {"_ZL7freeMem"}}, 
   {{0xffffffff8001d090}, {"_ZL12isHeapActive"}}, 
   {{0xffffffff8001d098}, {"_ZL10lastHeader"}}, 
   {{0xffffffff8001d0a0}, {"_ZL7heapEnd"}}, 
   {{0xffffffff8001d0a8}, {"_ZL9heapStart"}}, 
   {{0xffffffff8001d0b0}, {"_ZL17page_bitmap_index"}}, 
   {{0xffffffff8001d0c0}, {"_ZL11page_bitmap"}}, 
   {{0xffffffff8001d0d0}, {"_ZL11initialized"}}, 
   {{0xffffffff8001d0d8}, {"_ZL11used_memory"}}, 
   {{0xffffffff8001d0e0}, {"_ZL15reserved_memory"}}, 
   {{0xffffffff8001d0e8}, {"_ZL11free_memory"}}, 
   {{0xffffffff8001d0f0}, {"_ZZ6strtokPcS_E1p"}}, 
   {{0xffffffff8001d0f8}, {"currentStack"}}, 
   {{0xffffffff8001d100}, {"__dso_handle"}}, 
   {{0xffffffff8001d108}, {"__atexit_func_count"}}, 
   {{0xffffffff8001d120}, {"__atexit_funcs"}}, 
   {{0xffffffff8001dd20}, {"kernelPort"}}, 
   {{0xffffffff8001dd28}, {"data_end_addr"}}, 
   {{0xffffffff8001dd28}, {"kernel_end_addr"}}, 
};
