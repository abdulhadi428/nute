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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o \
	${OBJECTDIR}/lib/src/misc.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o \
	${OBJECTDIR}/sd/stm32_eval_sdio_sd.o \
	${OBJECTDIR}/lib/src/stm32f10x_adc.o \
	${OBJECTDIR}/leds_pca.o \
	${OBJECTDIR}/lib/src/stm32f10x_can.o \
	${OBJECTDIR}/lib/src/stm32f10x_flash.o \
	${OBJECTDIR}/lcd110x.o \
	${OBJECTDIR}/lib/src/stm32f10x_rcc.o \
	${OBJECTDIR}/lib/src/stm32f10x_i2c.o \
	${OBJECTDIR}/lib/src/stm32f10x_gpio.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/lib/src/stm32f10x_rtc.o \
	${OBJECTDIR}/lib/src/stm32f10x_dma.o \
	${OBJECTDIR}/lib/src/stm32f10x_crc.o \
	${OBJECTDIR}/images.o \
	${OBJECTDIR}/sd/sd.o \
	${OBJECTDIR}/lcd_font.o \
	${OBJECTDIR}/lib/src/stm32f10x_spi.o \
	${OBJECTDIR}/lib/CoreSupport/core_cm3.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o \
	${OBJECTDIR}/vs.o \
	${OBJECTDIR}/lib/src/stm32f10x_tim.o \
	${OBJECTDIR}/cc1101.o \
	${OBJECTDIR}/lib/src/stm32f10x_cec.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o \
	${OBJECTDIR}/lib/src/stm32f10x_iwdg.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o \
	${OBJECTDIR}/lib/src/stm32f10x_fsmc.o \
	${OBJECTDIR}/i2c_mgr.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o \
	${OBJECTDIR}/lib/src/stm32f10x_dac.o \
	${OBJECTDIR}/lib/src/stm32f10x_sdio.o \
	${OBJECTDIR}/uart.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o \
	${OBJECTDIR}/lib/src/stm32f10x_bkp.o \
	${OBJECTDIR}/lib/src/stm32f10x_exti.o \
	${OBJECTDIR}/lib/src/stm32f10x_dbgmcu.o \
	${OBJECTDIR}/lib/system_stm32f10x.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o \
	${OBJECTDIR}/lib/src/stm32f10x_usart.o \
	${OBJECTDIR}/lib/src/stm32f10x_wwdg.o \
	${OBJECTDIR}/sd/sd_lowlevel.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rock_fw.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rock_fw.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rock_fw ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o: lib/startup/startup_stm32f10x_ld_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o lib/startup/startup_stm32f10x_ld_vl.s

${OBJECTDIR}/lib/src/misc.o: lib/src/misc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/misc.o lib/src/misc.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o: lib/startup/startup_stm32f10x_hd_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o lib/startup/startup_stm32f10x_hd_vl.s

${OBJECTDIR}/sd/stm32_eval_sdio_sd.o: sd/stm32_eval_sdio_sd.c 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sd/stm32_eval_sdio_sd.o sd/stm32_eval_sdio_sd.c

${OBJECTDIR}/lib/src/stm32f10x_adc.o: lib/src/stm32f10x_adc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_adc.o lib/src/stm32f10x_adc.c

${OBJECTDIR}/leds_pca.o: leds_pca.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/leds_pca.o leds_pca.cpp

${OBJECTDIR}/lib/src/stm32f10x_can.o: lib/src/stm32f10x_can.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_can.o lib/src/stm32f10x_can.c

${OBJECTDIR}/lib/src/stm32f10x_flash.o: lib/src/stm32f10x_flash.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_flash.o lib/src/stm32f10x_flash.c

${OBJECTDIR}/lcd110x.o: lcd110x.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lcd110x.o lcd110x.cpp

${OBJECTDIR}/lib/src/stm32f10x_rcc.o: lib/src/stm32f10x_rcc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_rcc.o lib/src/stm32f10x_rcc.c

${OBJECTDIR}/lib/src/stm32f10x_i2c.o: lib/src/stm32f10x_i2c.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_i2c.o lib/src/stm32f10x_i2c.c

${OBJECTDIR}/lib/src/stm32f10x_gpio.o: lib/src/stm32f10x_gpio.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_gpio.o lib/src/stm32f10x_gpio.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/lib/src/stm32f10x_rtc.o: lib/src/stm32f10x_rtc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_rtc.o lib/src/stm32f10x_rtc.c

${OBJECTDIR}/lib/src/stm32f10x_dma.o: lib/src/stm32f10x_dma.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_dma.o lib/src/stm32f10x_dma.c

${OBJECTDIR}/lib/src/stm32f10x_crc.o: lib/src/stm32f10x_crc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_crc.o lib/src/stm32f10x_crc.c

${OBJECTDIR}/images.o: images.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/images.o images.cpp

${OBJECTDIR}/sd/sd.o: sd/sd.cpp 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sd/sd.o sd/sd.cpp

${OBJECTDIR}/lcd_font.o: lcd_font.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lcd_font.o lcd_font.cpp

${OBJECTDIR}/lib/src/stm32f10x_spi.o: lib/src/stm32f10x_spi.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_spi.o lib/src/stm32f10x_spi.c

${OBJECTDIR}/lib/CoreSupport/core_cm3.o: lib/CoreSupport/core_cm3.c 
	${MKDIR} -p ${OBJECTDIR}/lib/CoreSupport
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/CoreSupport/core_cm3.o lib/CoreSupport/core_cm3.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o: lib/startup/startup_stm32f10x_md_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o lib/startup/startup_stm32f10x_md_vl.s

${OBJECTDIR}/vs.o: vs.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vs.o vs.cpp

${OBJECTDIR}/lib/src/stm32f10x_tim.o: lib/src/stm32f10x_tim.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_tim.o lib/src/stm32f10x_tim.c

${OBJECTDIR}/cc1101.o: cc1101.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/cc1101.o cc1101.cpp

${OBJECTDIR}/lib/src/stm32f10x_cec.o: lib/src/stm32f10x_cec.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_cec.o lib/src/stm32f10x_cec.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o: lib/startup/startup_stm32f10x_xl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o lib/startup/startup_stm32f10x_xl.s

${OBJECTDIR}/lib/src/stm32f10x_iwdg.o: lib/src/stm32f10x_iwdg.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_iwdg.o lib/src/stm32f10x_iwdg.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o: lib/startup/startup_stm32f10x_hd.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o lib/startup/startup_stm32f10x_hd.s

${OBJECTDIR}/lib/src/stm32f10x_fsmc.o: lib/src/stm32f10x_fsmc.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_fsmc.o lib/src/stm32f10x_fsmc.c

${OBJECTDIR}/i2c_mgr.o: i2c_mgr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/i2c_mgr.o i2c_mgr.cpp

${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o: lib/startup/startup_stm32f10x_cl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o lib/startup/startup_stm32f10x_cl.s

${OBJECTDIR}/lib/src/stm32f10x_dac.o: lib/src/stm32f10x_dac.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_dac.o lib/src/stm32f10x_dac.c

${OBJECTDIR}/lib/src/stm32f10x_sdio.o: lib/src/stm32f10x_sdio.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_sdio.o lib/src/stm32f10x_sdio.c

${OBJECTDIR}/uart.o: uart.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/uart.o uart.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o: lib/startup/startup_stm32f10x_md.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o lib/startup/startup_stm32f10x_md.s

${OBJECTDIR}/lib/src/stm32f10x_bkp.o: lib/src/stm32f10x_bkp.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_bkp.o lib/src/stm32f10x_bkp.c

${OBJECTDIR}/lib/src/stm32f10x_exti.o: lib/src/stm32f10x_exti.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_exti.o lib/src/stm32f10x_exti.c

${OBJECTDIR}/lib/src/stm32f10x_dbgmcu.o: lib/src/stm32f10x_dbgmcu.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_dbgmcu.o lib/src/stm32f10x_dbgmcu.c

${OBJECTDIR}/lib/system_stm32f10x.o: lib/system_stm32f10x.c 
	${MKDIR} -p ${OBJECTDIR}/lib
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/system_stm32f10x.o lib/system_stm32f10x.c

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o: lib/startup/startup_stm32f10x_ld.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o lib/startup/startup_stm32f10x_ld.s

${OBJECTDIR}/lib/src/stm32f10x_usart.o: lib/src/stm32f10x_usart.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_usart.o lib/src/stm32f10x_usart.c

${OBJECTDIR}/lib/src/stm32f10x_wwdg.o: lib/src/stm32f10x_wwdg.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_wwdg.o lib/src/stm32f10x_wwdg.c

${OBJECTDIR}/sd/sd_lowlevel.o: sd/sd_lowlevel.c 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sd/sd_lowlevel.o sd/sd_lowlevel.c

${OBJECTDIR}/delay_util.o: delay_util.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/delay_util.o delay_util.cpp

${OBJECTDIR}/lib/src/stm32f10x_pwr.o: lib/src/stm32f10x_pwr.c 
	${MKDIR} -p ${OBJECTDIR}/lib/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/lib/src/stm32f10x_pwr.o lib/src/stm32f10x_pwr.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rock_fw.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
