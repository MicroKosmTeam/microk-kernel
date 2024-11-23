[bits 64]

%macro pushall 0
push rdi
push rsi

push rax
push rbx
push rcx
push rdx

push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15

%endmacro

%macro popall 0

pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8

pop rdx
pop rcx
pop rbx
pop rax

pop rsi
pop rdi

%endmacro

extern HandleVMExit

global SVMLaunchVM
SVMLaunchVM:
        ;
        ; Update the current stack pointer with the host RSP. This protects
        ; values stored on stack for the hypervisor from being overwritten by
        ; the guest due to a use of the same stack memory.
        ;
        mov rsp, rdi    ; Rsp <= HostRsp

        ; Run the loop to executed the guest and handle #VMEXIT. Below is the
        ; current stack leyout.
        ; ----
        ; Rsp          => 0x...fd0 GuestVmcbPa       ; HostStackLayout
        ;                 0x...fd8 HostVmcbPa        ;
        ;                 0x...fe0 Self              ;
        ;                 0x...fe8 SharedVpData      ;
        ;                 0x...ff0 Padding1          ;
        ;                 0x...ff8 Reserved1         ;
        ; ----
        ;
        mov rax, rsi  ; RAX <= VpData->HostStackLayout.GuestVmcbPa
SvLV10: ;
        vmload       ; load previously saved guest state from VMCB

        ;
        ; Start the guest. The VMRUN instruction resumes execution of the guest
        ; with state described in VMCB (specified by RAX by its physical address)
        ; until #VMEXI is triggered. On #VMEXIT, the VMRUN instruction completes
        ; and resumes the next instruction (ie, vmsave in our case).
        ;
        ; The VMRUN instruction does the following things in this order:
        ; - saves some current state (ie. host state) into the host state-save
        ;   area specified in IA32_MSR_VM_HSAVE_PA
        ; - loads guest state from the VMCB state-save area
        ; - enables interrupts by setting the the global interrupt flag (GIF)
        ; - resumes execution of the guest until #VMEXIT occurs
        ; See "Basic Operation" for more details.
        ;
        ; On #VMEXIT:
        ; - disables interrupts by clearing the the global interrupt flag (GIF)
        ; - saves current guest state into and update VMCB to provide information
        ;   to handle #VMEXIT
        ; - loads the host state previously saved by the VMRUN instruction
        ; See "#VMEXIT" in the volume 2 and "VMRUN" in the volume 3 for more
        ; details.
        ;
        vmrun        ; Switch to the guest until #VMEXIT

        ;
        ; #VMEXIT occured. Now, some of guest state has been saved to VMCB, but
        ; not all of it. Save some of unsaved state with the VMSAVE instruction.
        ;
        ; RAX (and some other state like RSP) has been restored from the host
        ; state-save, so it has the same value as before and not guest's one.
        ;
        vmsave       ; Save current guest state to VMCB

        ; Also save guest's GPRs since those are not saved anywhere by the
        ; processor on #VMEXIT and will be destroyed by subsequent host code.
        ;
        pushall ; Stack pointer decreased 8 * 16

        ;
        ; Set parameters for SvHandleVmExit. Below is the current stack leyout.
        ; ----
        ; Rsp                             => 0x...dc0 R15               ; GUEST_REGISTERS
        ;                                    0x...dc8 R14               ;
        ;                                             ...               ;
        ;                                    0x...e38 RAX               ;
        ; Rsp + 8 * 16                    => 0x...e40 TrapFrame         ; HostStackLayout
        ;                                             ...               ;
        ; Rsp + 8 * 16 + KTRAP_FRAME_SIZE => 0x...fd0 GuestVmcbPa       ;
        ;                                    0x...fd8 HostVmcbPa        ;
        ; Rsp + 8 * 18 + KTRAP_FRAME_SIZE => 0x...fe0 Self              ;
        ;                                    0x...fe8 SharedVpData      ;
        ;                                    0x...ff0 Padding1          ;
        ;                                    0x...ff8 Reserved1         ;
        ; ----
        ;
	mov rdi, rax
        mov rsi, rsp                                ; Rdx <= GuestRegisters

        ;
        ; Handle #VMEXIT.
        ;
        call HandleVMExit

        popall

        jmp SvLV10                  ; jmp SvLV10

