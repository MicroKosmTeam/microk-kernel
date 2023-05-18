include ../Makefile.inc

SRCDIR = src

LDS64 = $(SRCDIR)/kernel_$(ARCH).ld

CFLAGS = -ffreestanding             \
	 -fstack-protector          \
	 -fno-omit-frame-pointer    \
	 -fno-builtin-g             \
	 -I ../$(MKMIDIR)/src/include   \
	 -I $(SRCDIR)/include       \
	 -m64                       \
	 -mabi=sysv                 \
	 -mno-80387                 \
         -mno-mmx                   \
         -mno-sse                   \
         -mno-sse2                  \
	 -mno-red-zone              \
	 -mcmodel=kernel            \
	 -fpermissive               \
	 -Wall                      \
	 -Wextra                    \
	 -Wno-write-strings         \
	 -Weffc++                   \
	 -Og                        \
	 -fno-rtti                  \
	 -fno-exceptions            \
	 -fno-lto                   \
	 -fno-pie                   \
	 -fno-pic                   \
	 -march=x86-64              \
	 -ggdb


ASMFLAGS = -f elf64

LDFLAGS = -nostdlib                 \
	  -static                   \
	  -m elf_$(ARCH)            \
	  -z max-page-size=0x1000

CPPSRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)
KOBJS = $(patsubst $(SRCDIR)/%.cpp, $(SRCDIR)/%.o, $(CPPSRC))
KOBJS += $(patsubst $(SRCDIR)/%.asm, $(SRCDIR)/%.o, $(ASMSRC))

.PHONY: kernel link clean

kernel: $(KOBJS) link
	@ echo !==== KERNEL COMPILED

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@ mkdir -p $(@D)
	@ echo !==== COMPILING $^ && \
	$(CPP) $(CFLAGS) -c $^ -o $@


$(SRCDIR)/%.o: $(SRCDIR)/%.asm
	@ mkdir -p $(@D)
	@ echo !==== COMPILING $^  && \
	$(ASM) $(ASMFLAGS) $^ -o $@

link: $(KOBJS)
	@ echo !==== LINKING
	$(LD) $(LDFLAGS) -T $(LDS64) -o microk.elf $(KOBJS)
	@ ./symbolstocpp.sh
	$(CPP) $(CFLAGS) -c $(SRCDIR)/sys/symbolMap.cpp -o $(SRCDIR)/sys/symbolMap.o
	$(LD) $(LDFLAGS) -T $(LDS64) -o microk.elf $(KOBJS)


clean:
	@rm $(KOBJS)
