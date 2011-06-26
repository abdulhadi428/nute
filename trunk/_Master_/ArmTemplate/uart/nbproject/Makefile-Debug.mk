#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-none-eabi-gcc.exe
CCC=arm-none-eabi-g++.exe
CXX=arm-none-eabi-g++.exe
FC=
AS=arm-none-eabi-as.exe

# Macros
CND_PLATFORM=Yagarto-Windows
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o \
	${OBJECTDIR}/lib/src/misc.o \
	${OBJECTDIR}/lib/src/stm32f10x_adc.o \
	${OBJECTDIR}/lib/src/stm32f10x_rcc.o \
	${OBJECTDIR}/lib/src/stm32f10x_can.o \
	${OBJECTDIR}/lib/src/stm32f10x_i2c.o \
	${OBJECTDIR}/lib/src/stm32f10x_flash.o \
	${OBJECTDIR}/lib/src/stm32f10x_gpio.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/lib/src/stm32f10x_rtc.o \
	${OBJECTDIR}/lib/src/stm32f10x_dma.o \
	${OBJECTDIR}/lib/src/stm32f10x_crc.o \
	${OBJECTDIR}/system_kl.o \
	${OBJECTDIR}/lib/src/stm32f10x_spi.o \
	${OBJECTDIR}/lib/CoreSupport/core_cm3.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o \
	${OBJECTDIR}/lib/src/stm32f10x_tim.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o \
	${OBJECTDIR}/lib/src/stm32f10x_cec.o \
	${OBJECTDIR}/lib/src/stm32f10x_iwdg.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o \
	${OBJECTDIR}/lib/src/stm32f10x_fsmc.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o \
	${OBJECTDIR}/lib/src/stm32f10x_dac.o \
	${OBJECTDIR}/lib/src/stm32f10x_sdio.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o \
	${OBJECTDIR}/lib/src/stm32f10x_exti.o \
	${OBJECTDIR}/lib/src/stm32f10x_bkp.o \
	${OBJECTDIR}/lib/src/stm32f10x_dbgmcu.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o \
	${OBJECTDIR}/lib/src/stm32f10x_usart.o \
	${OBJECTDIR}/lib/src/stm32f10x_wwdg.o \
	${OBJECTDIR}/delay_util.o \
	${OBJECTDIR}/lib/src/stm32f10x_pwr.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk output/output.elf.exe

output/output.elf.exe: ${OBJECTFILES}
	${MKDIR} -p output
	${LINK.cc} -o output/output.elf ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o: lib/startup/startup_stm32f10x_hd_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o lib/startup/startup_stm32f10x_hd_vl.s

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o: lib/startup/startup_stm32f10x_ld_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o lib/startup/startup_stm32f10x_ld_vl.s

${OBJECTDIR}/lib/src/misc.o: lib/src/misc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/misc.o lib/src/misc.c

${OBJECTDIR}/lib/src/stm32f10x_adc.o: lib/src/stm32f10x_adc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_adc.o lib/src/stm32f10x_adc.c

${OBJECTDIR}/lib/src/stm32f10x_rcc.o: lib/src/stm32f10x_rcc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_rcc.o lib/src/stm32f10x_rcc.c

${OBJECTDIR}/lib/src/stm32f10x_can.o: lib/src/stm32f10x_can.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_can.o lib/src/stm32f10x_can.c

${OBJECTDIR}/lib/src/stm32f10x_i2c.o: lib/src/stm32f10x_i2c.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_i2c.o lib/src/stm32f10x_i2c.c

${OBJECTDIR}/lib/src/stm32f10x_flash.o: lib/src/stm32f10x_flash.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_flash.o lib/src/stm32f10x_flash.c

${OBJECTDIR}/lib/src/stm32f10x_gpio.o: lib/src/stm32f10x_gpio.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_gpio.o lib/src/stm32f10x_gpio.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/lib/src/stm32f10x_rtc.o: lib/src/stm32f10x_rtc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_rtc.o lib/src/stm32f10x_rtc.c

${OBJECTDIR}/lib/src/stm32f10x_dma.o: lib/src/stm32f10x_dma.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_dma.o lib/src/stm32f10x_dma.c

${OBJECTDIR}/lib/src/stm32f10x_crc.o: lib/src/stm32f10x_crc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_crc.o lib/src/stm32f10x_crc.c

${OBJECTDIR}/system_kl.o: system_kl.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/system_kl.o system_kl.c

${OBJECTDIR}/lib/src/stm32f10x_spi.o: lib/src/stm32f10x_spi.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_spi.o lib/src/stm32f10x_spi.c

${OBJECTDIR}/lib/CoreSupport/core_cm3.o: lib/CoreSupport/core_cm3.c 
	${MKDIR} -p ${OBJECTDIR}/lib/CoreSupport
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/CoreSupport/core_cm3.o lib/CoreSupport/core_cm3.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o: lib/startup/startup_stm32f10x_md_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o lib/startup/startup_stm32f10x_md_vl.s

${OBJECTDIR}/lib/src/stm32f10x_tim.o: lib/src/stm32f10x_tim.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_tim.o lib/src/stm32f10x_tim.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o: lib/startup/startup_stm32f10x_xl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o lib/startup/startup_stm32f10x_xl.s

${OBJECTDIR}/lib/src/stm32f10x_cec.o: lib/src/stm32f10x_cec.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_cec.o lib/src/stm32f10x_cec.c

${OBJECTDIR}/lib/src/stm32f10x_iwdg.o: lib/src/stm32f10x_iwdg.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_iwdg.o lib/src/stm32f10x_iwdg.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o: lib/startup/startup_stm32f10x_hd.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o lib/startup/startup_stm32f10x_hd.s

${OBJECTDIR}/lib/src/stm32f10x_fsmc.o: lib/src/stm32f10x_fsmc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_fsmc.o lib/src/stm32f10x_fsmc.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o: lib/startup/startup_stm32f10x_cl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o lib/startup/startup_stm32f10x_cl.s

${OBJECTDIR}/lib/src/stm32f10x_dac.o: lib/src/stm32f10x_dac.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_dac.o lib/src/stm32f10x_dac.c

${OBJECTDIR}/lib/src/stm32f10x_sdio.o: lib/src/stm32f10x_sdio.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_sdio.o lib/src/stm32f10x_sdio.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o: lib/startup/startup_stm32f10x_md.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o lib/startup/startup_stm32f10x_md.s

${OBJECTDIR}/lib/src/stm32f10x_exti.o: lib/src/stm32f10x_exti.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_exti.o lib/src/stm32f10x_exti.c

${OBJECTDIR}/lib/src/stm32f10x_bkp.o: lib/src/stm32f10x_bkp.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_bkp.o lib/src/stm32f10x_bkp.c

${OBJECTDIR}/lib/src/stm32f10x_dbgmcu.o: lib/src/stm32f10x_dbgmcu.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_dbgmcu.o lib/src/stm32f10x_dbgmcu.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o: lib/startup/startup_stm32f10x_ld.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o lib/startup/startup_stm32f10x_ld.s

${OBJECTDIR}/lib/src/stm32f10x_usart.o: lib/src/stm32f10x_usart.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_usart.o lib/src/stm32f10x_usart.c

${OBJECTDIR}/lib/src/stm32f10x_wwdg.o: lib/src/stm32f10x_wwdg.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_wwdg.o lib/src/stm32f10x_wwdg.c

${OBJECTDIR}/delay_util.o: delay_util.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/delay_util.o delay_util.cpp

${OBJECTDIR}/lib/src/stm32f10x_pwr.o: lib/src/stm32f10x_pwr.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -Ilib -Ilib/CoreSupport -Ilib/inc -I. -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_pwr.o lib/src/stm32f10x_pwr.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} output/output.elf.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc