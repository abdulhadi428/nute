################################################################################
# This is Kreyl-built hand-made makefile for STM32 projects making.            #
################################################################################

######################## Project-dependable section ############################

# Target processor family: uncomment needed line
# STM32 Medium density Value Line - STM32F100x with 64 to 128 kByte of FLASH
#CPUFAMILY = STM32F10X_MD_VL
# STM32 High density Performance Line - STM32F103x with 256 to 512 kByte of FLASH
CPUFAMILY = STM32F10X_HD

# Define output type here, uncomment needed line
OUTTYPE = binary
#OUTTYPE = ihex

# List project source files here
PRJCSRC = uart.c \
sd/stm32_eval_sdio_sd.c \
sd/sd_lowlevel.c



PRJCPPSRC = main.cpp \
i2c_mgr.cpp \
leds_pca.cpp \
delay_util.cpp \
sd/sd.cpp \


# List project include directories here
PRJINCDIR =

# Define optimization level here
OPT = s


################################ General section ###############################
# In general, there is no need to change anything here

# Object files directory
OBJDIR = obj
# Output files directory
OUTDIR = output

# CPU flags
MCFLAGS = -mcpu=cortex-m3 -mthumb

# All default directories to look for include files here
STINCDIR = . \
lib \
lib/CoreSupport \
lib/inc \
sd \

#./lib/startup

# Define linker script file here
LDSCRIPT_FLASH = lib/stm32_flash.ld

# List C source files here
SRC  = $(PRJCSRC) \
lib/system_stm32f10x.c \
lib/src/misc.c \
lib/src/stm32f10x_gpio.c \
lib/src/stm32f10x_rcc.c \
lib/src/stm32f10x_i2c.c \
lib/src/stm32f10x_dma.c \
lib/src/stm32f10x_usart.c \
lib/src/stm32f10x_tim.c \
lib/src/stm32f10x_sdio.c

#lib/src/stm32f10x_spi.c \
#lib/src/stm32f10x_exti.c \


# Startup ASM file here
ifeq ($(CPUFAMILY), STM32F10X_HD)
    ASRC =lib/startup/startup_stm32f10x_hd.s
endif

ifeq ($(CPUFAMILY), STM32F10X_MD_VL)
    ASRC =lib/startup/startup_stm32f10x_md_vl.s
endif


# Debugging format
DEBUG = dwarf-2

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

#################################### Tools #####################################
# Tools
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
OD   = $(TRGT)objdump
AS   = $(TRGT)gcc -x assembler-with-cpp
BIN  = $(CP) -O binary
SIZE = $(TRGT)size

# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(ASRC:%.s=$(OBJDIR)/%.o) $(PRJCPPSRC:%.cpp=$(OBJDIR)/%.o)

# Define all listing files.
#LST = $(SRC:%.c=$(OBJDIR)/%.lst)


#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS  = $(MCFLAGS)
CFLAGS += -g$(DEBUG)
CFLAGS += -O$(OPT)
CFLAGS += -D$(CPUFAMILY)
CFLAGS += $(patsubst %, -I%, $(STINCDIR) $(PRJINCDIR))
CFLAGS += $(CSTANDARD)
# Enable ST library usage
CFLAGS += -DUSE_STDPERIPH_DRIVER
# From Wiki:
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wextra
CFLAGS += -Wimplicit -Wpointer-arith -Wredundant-decls
CFLAGS += -Wshadow -Wcast-qual -Wnested-externs
# End of Wiki
#CFLAGS += -Wa-ahlms=$(<:.c=.lst) $(DEFS)
CFLAGS += -fomit-frame-pointer
#CFLAGS += -fverbose-asm
# Compiler flags to generate dependency files.
CFLAGS += -MMD -MP -MF .dep/$(@F).d


#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS  = $(MCFLAGS)
CPPFLAGS += -g$(DEBUG)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -D$(CPUFAMILY) -D__cplusplus
CPPFLAGS += $(patsubst %, -I%, $(STINCDIR) $(PRJINCDIR))
# Enable ST library usage
CPPFLAGS += -DUSE_STDPERIPH_DRIVER
# From Wiki:
CPPFLAGS += -fomit-frame-pointer
CPPFLAGS += -fno-exceptions -fno-rtti
CPPFLAGS += -ffunction-sections -fdata-sections
CPPFLAGS += -fno-elide-constructors
CPPFLAGS += -funsigned-bitfields -fshort-enums
CPPFLAGS += -Wall -Wextra
CPPFLAGS += -Winline -finline-limit=40
CPPFLAGS += -Wpointer-arith -Wredundant-decls
CPPFLAGS += -Wshadow -Wcast-qual -Wcast-align
# End of Wiki
CPPFLAGS += -x c++
# Compiler flags to generate dependency files.
CPPFLAGS += -MMD -MP -MF .dep/$(@F).d


#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -adhlns:   create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex
#       dump that will be displayed for a given single line of source input.
#ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:%.S=$(OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100
ASFLAGS = $(MCFLAGS) -g -gdwarf-2
#ASFLAGS += -Wa,-amhls=$(<:.s=.lst)
#ASFLAGS += $(patsubst %, -I%, $(STINCDIR))


#--------------- Linker options --------------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file LDFLAGS  = -mthumb
LDFLAGS  = $(MCFLAGS)
#LDFLAGS += -nostartfiles
LDFLAGS += -static
LDFLAGS += -Wl,-cref,-u,Reset_Handler
LDFLAGS += -Wl,-Map=$(OUTDIR)/$(PROJECT).map
LDFLAGS += -Wl,--gc-sections
#LDFLAGS += -Wl,-Map=$(OUTDIR)/$(PROJECT).map,--cref,--no-warn-mismatch
#LDFLAGS += $(patsubst %, -I%, $(STINCDIR) $(PRJINCDIR))

LDFLAGS_FLASH = $(LDFLAGS) -T$(LDSCRIPT_FLASH)



################################ Messages ######################################
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_COMPILING = Compiling:
MSG_LINKING = Linking:
MSG_ASSEMBLING = Assembling:
MSG_FLASH = Creating load file for Flash:
MSG_SIZE_AFTER = Size after:
MSG_CLEANING = Cleaning project:

############################## makefile rules ##################################
# Default target
all: begin build sizeafter end
#gccversion


build: compile link makebin

compile: $(OBJ)
link:    output.elf
makebin: output.bin


# Display sizes
ELFSIZE = $(SIZE) -d $(OUTDIR)/output.elf

sizeafter:
	@if test -f $(OUTDIR)/output.elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; echo; fi


# Compiling C files. We need all .o files in OBJ directory and their source .c files to success
$(OBJDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	@mkdir -p `dirname $@`
	$(CC) -c $(CFLAGS) $< -o $@
# Compiling CPP files. We need all .o files in OBJ directory and their source .cpp files to success
$(OBJDIR)/%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING) $<
	@mkdir -p `dirname $@`
	$(CC) -c $(CPPFLAGS) $< -o $@

# Assembling ASM files.
$(OBJDIR)/%.o : %.s
	@echo
	@echo $(MSG_ASSEMBLING) $<
	@mkdir -p `dirname $@`
	$(CC) -c $(ASFLAGS) $< -o $@

# Linking .o files. Those must be ready yet.
output.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(OBJ) $(LDFLAGS_FLASH) -o $(OUTDIR)/$@


# Create .bin file
output.bin : output.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(BIN) $(OUTDIR)/$< $(OUTDIR)/$@


# Eye candy
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo

# Display compiler version information.
gccversion:
	@$(CC) --version


################ Clean ################
clean:
	@echo
	@echo $(MSG_CLEANING)
	@echo $(OBJ)
	-rm -f $(OBJ)
	-rm -f $(OUTDIR)/*.*
	-rm -f $(SRC:.c=.lst)
	-rm -f $(ASRC:.s=.lst)
	-rm -fR .dep

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
