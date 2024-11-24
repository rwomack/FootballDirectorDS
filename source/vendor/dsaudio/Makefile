#----------------------------------------------------------------------------
# DS Sound Engine example's makefile
#
# Type 'make' to build the sample. Make sure you have NitroSDK installed.
#----------------------------------------------------------------------------

SUBDIRS		=

# the Sound Engine library - you can remove this when you have the source
LLIBRARY_DIRS  += libs
LLIBRARIES     += libDirectSound.safe_speed.a

#----------------------------------------------------------------------------

# the example code - you can add DirectSound.c here when you have the source
SRCS		=	example.c

NITRO_MAKEROM	=	TRUE
ROM_SPEC	=	example.rsf
TARGET_NEF	=	example.nef
TARGET_BIN	=	example.srl

include	$(NITROSDK_ROOT)/build/buildtools/commondefs

#----------------------------------------------------------------------------

do-build:		$(TARGETS)

include	$(NITROSDK_ROOT)/build/buildtools/modulerules

# [EOF]
