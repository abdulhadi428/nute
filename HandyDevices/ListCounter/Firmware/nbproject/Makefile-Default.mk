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
CC=avr-gcc.exe
CCC=avr-g++.exe
CXX=avr-g++.exe
FC=
AS=avr-as.exe

# Macros
CND_PLATFORM=WinAVR-Windows
CND_CONF=Default
CND_DISTDIR=dist

# Include project Makefile
include Makefile-Firmware.mk

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES=


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
	cd D\:/Nute/ListCounter/Firmware && $(MAKE) -f Makefile

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	cd D\:/Nute/ListCounter/Firmware && $(MAKE) -f Makefile clean

# Subprojects
.clean-subprojects:
