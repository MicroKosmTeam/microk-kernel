include ../Makefile.inc

SRCDIR = src
COMMON_SOURCES = $(SRCDIR)/generic
ARCH_SOURCES = $(SRCDIR)/arch/$(ARCH)

LDS64 = $(SRCDIR)/kernel_$(ARCH).ld

COMMON_CFLAGS = -ffreestanding             \
	 -fno-stack-protector          \
	 -fno-omit-frame-pointer    \
	 -fno-builtin-g             \
	 -I $(SRCDIR)/include       \
	 -Wall                      \
	 -Wextra                    \
	 -Werror                    \
	 -Wno-write-strings         \
	 -Wno-shift-count-overflow \
	 -Wno-expansion-to-defined \
	 -Og                        \
	 -fno-rtti                  \
	 -fno-exceptions            \
	 -fno-lto                   \
	 -fno-pie                   \
	 -fno-pic                   \
	 -ggdb

LDFLAGS = -nostdlib                 \
	  -static                   \
	  -z max-page-size=0x1000

COMMON_CPPSRC = $(call rwildcard,$(COMMON_SOURCES),*.cpp)
COMMON_OBJS = $(patsubst $(COMMON_SOURCES)/%.cpp, $(COMMON_SOURCES)/%.o, $(COMMON_CPPSRC))

ARCH_CPPSRC = $(call rwildcard,$(ARCH_SOURCES),*.cpp)
ARCH_ASMSRC = $(call rwildcard,$(ARCH_SOURCES),*.asm)
ARCH_OBJS = $(patsubst $(ARCH_SOURCES)/%.cpp, $(ARCH_SOURCES)/%.o, $(ARCH_CPPSRC))
ARCH_OBJS += $(patsubst $(ARCH_SOURCES)/%.asm, $(ARCH_SOURCES)/%.o, $(ARCH_ASMSRC))

ifeq ($(ARCH), x86_64)
	CFLAGS = $(COMMON_CFLAGS) -mno-80387 \
	 -mno-80387                 \
         -mno-mmx                   \
         -mno-sse                   \
         -mno-sse2                  \
	 -mno-red-zone              \
	 -m64                       \
	 -mabi=sysv                 \
	 -masm=intel \
	 -mcmodel=kernel            \
	 -march=x86-64 

	LDFLAGS += -m elf_x86_64

	ASMFLAGS = -f elf64
else ifeq ($(ARCH), aarch64)
	CFLAGS = $(COMMON_CFLAGS) -march=armv8-a \
         -mabi=lp64     \
	 -mcmodel=large

	LDFLAGS += -m aarch64elf
	ASMFLAGS = 
else
	$(error Unsupported ARCH: $(ARCH))
endif



.PHONY: kernel link clean

$(COMMON_SOURCES)/%.o: $(COMMON_SOURCES)/%.cpp
	@ mkdir -p $(@D)
	@ echo !==== COMPILING $^ && \
	$(CPP) $(CFLAGS) -c $^ -o $@

$(ARCH_SOURCES)/%.o: $(ARCH_SOURCES)/%.cpp
	@ mkdir -p $(@D)
	@ echo !==== COMPILING $(ARCH) $^ && \
	$(CPP) $(CFLAGS) -c $^ -o $@


$(ARCH_SOURCES)/%.o: $(ARCH_SOURCES)/%.asm
	@ mkdir -p $(@D)
	@ echo !==== COMPILING $(ARCH) $^  && \
	$(ASM) $(ASMFLAGS) $^ -o $@

link: $(COMMON_OBJS) $(ARCH_OBJS)
	@ echo !==== LINKING
	$(LD) $(LDFLAGS) -T $(LDS64) -o microk.elf $(COMMON_OBJS) $(ARCH_OBJS)

kernel: $(COMMON_OBJS) $(ARCH_OBJS) link
	@ echo !==== KERNEL COMPILED

clean:
	@rm $(COMMON_OBJS) $(ARCH_OBJS)
