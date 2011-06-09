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
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o \
	${OBJECTDIR}/leds_pca.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/sd/sd.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o \
	${OBJECTDIR}/vs.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o \
	${OBJECTDIR}/i2c_mgr.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o \
	${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rock_fw.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rock_fw.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rock_fw ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o: lib/startup/startup_stm32f10x_hd_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd_vl.o lib/startup/startup_stm32f10x_hd_vl.s

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o: lib/startup/startup_stm32f10x_ld_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld_vl.o lib/startup/startup_stm32f10x_ld_vl.s

${OBJECTDIR}/leds_pca.o: leds_pca.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/leds_pca.o leds_pca.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/sd/sd.o: sd/sd.cpp 
	${MKDIR} -p ${OBJECTDIR}/sd
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/sd/sd.o sd/sd.cpp

${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o: lib/startup/startup_stm32f10x_md_vl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md_vl.o lib/startup/startup_stm32f10x_md_vl.s

${OBJECTDIR}/vs.o: vs.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/vs.o vs.cpp

${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o: lib/startup/startup_stm32f10x_xl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_xl.o lib/startup/startup_stm32f10x_xl.s

${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o: lib/startup/startup_stm32f10x_hd.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_hd.o lib/startup/startup_stm32f10x_hd.s

${OBJECTDIR}/i2c_mgr.o: i2c_mgr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/i2c_mgr.o i2c_mgr.cpp

${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o: lib/startup/startup_stm32f10x_cl.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_cl.o lib/startup/startup_stm32f10x_cl.s

${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o: lib/startup/startup_stm32f10x_md.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_md.o lib/startup/startup_stm32f10x_md.s

${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o: lib/startup/startup_stm32f10x_ld.s 
	${MKDIR} -p ${OBJECTDIR}/lib/startup
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/lib/startup/startup_stm32f10x_ld.o lib/startup/startup_stm32f10x_ld.s

${OBJECTDIR}/delay_util.o: delay_util.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DSTM32F10X_HD -I. -Ilib -Ilib/inc -Isd -MMD -MP -MF $@.d -o ${OBJECTDIR}/delay_util.o delay_util.cpp

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
