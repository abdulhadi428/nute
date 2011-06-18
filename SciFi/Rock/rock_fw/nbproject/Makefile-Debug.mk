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
FC=gfortran
AS=arm-none-eabi-as.exe

# Macros
CND_PLATFORM=Yagarto-Windows
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o \
	${OBJECTDIR}/leds_pca.o \
	${OBJECTDIR}/lcd110x.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/images.o \
	${OBJECTDIR}/sd/sd.o \
	${OBJECTDIR}/lcd_font.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o \
	${OBJECTDIR}/vs.o \
	${OBJECTDIR}/cc1101.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o \
	${OBJECTDIR}/i2c_mgr.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o \
	${OBJECTDIR}/media.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o \
	${OBJECTDIR}/acc_mma.o \
	${OBJECTDIR}/delay_util.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk d\Nute\SciFi\Rock\rock_fw\output\output.elf.exe

d\Nute\SciFi\Rock\rock_fw\output\output.elf.exe: ${OBJECTFILES}
	${MKDIR} -p d\Nute\SciFi\Rock\rock_fw\output
	${LINK.cc} -o d:\Nute\SciFi\Rock\rock_fw\output\output.elf ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o: lib/startup/startup_stm32f10x_ld_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o lib/startup/startup_stm32f10x_ld_vl.s

${OBJECTDIR}/lib/src/misc.c.gch: lib/src/misc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/misc.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o: lib/startup/startup_stm32f10x_hd_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o lib/startup/startup_stm32f10x_hd_vl.s

${OBJECTDIR}/sd/stm32_eval_sdio_sd.c.gch: sd/stm32_eval_sdio_sd.c 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ sd/stm32_eval_sdio_sd.c

${OBJECTDIR}/lib/src/stm32f10x_adc.c.gch: lib/src/stm32f10x_adc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_adc.c

${OBJECTDIR}/leds_pca.o: leds_pca.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/leds_pca.o leds_pca.cpp

${OBJECTDIR}/lib/src/stm32f10x_can.c.gch: lib/src/stm32f10x_can.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_can.c

${OBJECTDIR}/lib/src/stm32f10x_flash.c.gch: lib/src/stm32f10x_flash.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_flash.c

${OBJECTDIR}/lcd110x.o: lcd110x.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/lcd110x.o lcd110x.cpp

${OBJECTDIR}/lib/src/stm32f10x_rcc.c.gch: lib/src/stm32f10x_rcc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_rcc.c

${OBJECTDIR}/lib/src/stm32f10x_i2c.c.gch: lib/src/stm32f10x_i2c.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_i2c.c

${OBJECTDIR}/lib/src/stm32f10x_gpio.c.gch: lib/src/stm32f10x_gpio.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_gpio.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/lib/src/stm32f10x_rtc.c.gch: lib/src/stm32f10x_rtc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_rtc.c

${OBJECTDIR}/lib/src/stm32f10x_dma.c.gch: lib/src/stm32f10x_dma.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_dma.c

${OBJECTDIR}/lib/src/stm32f10x_crc.c.gch: lib/src/stm32f10x_crc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_crc.c

${OBJECTDIR}/images.o: images.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/images.o images.cpp

${OBJECTDIR}/sd/sd.o: sd/sd.cpp 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/sd/sd.o sd/sd.cpp

${OBJECTDIR}/lcd_font.o: lcd_font.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/lcd_font.o lcd_font.cpp

${OBJECTDIR}/lib/src/stm32f10x_spi.c.gch: lib/src/stm32f10x_spi.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_spi.c

${OBJECTDIR}/lib/CoreSupport/core_cm3.c.gch: lib/CoreSupport/core_cm3.c 
	${MKDIR} -p ${OBJECTDIR}/lib/CoreSupport
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/CoreSupport/core_cm3.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o: lib/startup/startup_stm32f10x_md_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o lib/startup/startup_stm32f10x_md_vl.s

${OBJECTDIR}/vs.o: vs.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/vs.o vs.cpp

${OBJECTDIR}/lib/src/stm32f10x_tim.c.gch: lib/src/stm32f10x_tim.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_tim.c

${OBJECTDIR}/cc1101.o: cc1101.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/cc1101.o cc1101.cpp

${OBJECTDIR}/lib/src/stm32f10x_cec.c.gch: lib/src/stm32f10x_cec.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_cec.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o: lib/startup/startup_stm32f10x_xl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o lib/startup/startup_stm32f10x_xl.s

${OBJECTDIR}/lib/src/stm32f10x_iwdg.c.gch: lib/src/stm32f10x_iwdg.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_iwdg.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o: lib/startup/startup_stm32f10x_hd.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o lib/startup/startup_stm32f10x_hd.s

${OBJECTDIR}/lib/src/stm32f10x_fsmc.c.gch: lib/src/stm32f10x_fsmc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_fsmc.c

${OBJECTDIR}/i2c_mgr.o: i2c_mgr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/i2c_mgr.o i2c_mgr.cpp

${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o: lib/startup/startup_stm32f10x_cl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o lib/startup/startup_stm32f10x_cl.s

${OBJECTDIR}/lib/src/stm32f10x_dac.c.gch: lib/src/stm32f10x_dac.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_dac.c

${OBJECTDIR}/media.o: media.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/media.o media.cpp

${OBJECTDIR}/lib/src/stm32f10x_sdio.c.gch: lib/src/stm32f10x_sdio.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_sdio.c

${OBJECTDIR}/uart.c.gch: uart.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ uart.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o: lib/startup/startup_stm32f10x_md.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o lib/startup/startup_stm32f10x_md.s

${OBJECTDIR}/lib/src/stm32f10x_bkp.c.gch: lib/src/stm32f10x_bkp.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_bkp.c

${OBJECTDIR}/lib/src/stm32f10x_exti.c.gch: lib/src/stm32f10x_exti.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_exti.c

${OBJECTDIR}/lib/src/stm32f10x_dbgmcu.c.gch: lib/src/stm32f10x_dbgmcu.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_dbgmcu.c

${OBJECTDIR}/lib/system_stm32f10x.c.gch: lib/system_stm32f10x.c 
	${MKDIR} -p ${OBJECTDIR}/lib
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/system_stm32f10x.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o: lib/startup/startup_stm32f10x_ld.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o lib/startup/startup_stm32f10x_ld.s

${OBJECTDIR}/lib/src/stm32f10x_usart.c.gch: lib/src/stm32f10x_usart.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_usart.c

${OBJECTDIR}/sd/sd_lowlevel.c.gch: sd/sd_lowlevel.c 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ sd/sd_lowlevel.c

${OBJECTDIR}/lib/src/stm32f10x_wwdg.c.gch: lib/src/stm32f10x_wwdg.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_wwdg.c

${OBJECTDIR}/acc_mma.o: acc_mma.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/acc_mma.o acc_mma.cpp

${OBJECTDIR}/delay_util.o: delay_util.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/delay_util.o delay_util.cpp

${OBJECTDIR}/sd/ff.c.gch: sd/ff.c 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ sd/ff.c

${OBJECTDIR}/lib/src/stm32f10x_pwr.c.gch: lib/src/stm32f10x_pwr.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o $@ lib/src/stm32f10x_pwr.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} d\Nute\SciFi\Rock\rock_fw\output\output.elf.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
