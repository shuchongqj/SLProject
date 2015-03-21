##############################################################################
#  File:      ch09_TextureMapping.pro
#  Purpose:   QMake project definition file for the Hello Cube demo w. OpenGL
#  Author:    Marcus Hudritsch
#  Date:      September 2012 (HS12)
#  Copyright: Marcus Hudritsch, Switzerland
#             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
#             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
##############################################################################

TEMPLATE = app
TARGET = VolumeRendering
CONFIG += c++11 console
CONFIG -= app_bundle

#define platform variable for folder name
win32 {contains(QMAKE_TARGET.arch, x86_64) {PLATFORM = x64} else {PLATFORM = Win32}}
macx {PLATFORM = macx}
unix:!macx:!android {PLATFORM = linux}
android {PLATFORM = android}

#define configuration variable for folder name
CONFIG(debug, debug|release) {CONFIGURATION = Debug} else {CONFIGURATION = Release}

DESTDIR = ../_bin-$$CONFIGURATION-$$PLATFORM
OBJECTS_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM
LIBS += -L../_lib/$$CONFIGURATION/$$PLATFORM -llib-SLExternal
L
include(../SLProjectCommon.pro)

SOURCES += VolumeRendering.cpp
SOURCES += glUtils.cpp 
SOURCES += ../lib-SLProject/source/SL/SLImage.cpp 
SOURCES += ../lib-SLProject/source/SL/SL.cpp

HEADERS += glUtils.h
HEADERS += ../include/SLImage.h

OTHER_FILES += ../lib-SLProject/source/oglsl/VolumeRenderingRayCast.vert
OTHER_FILES += ../lib-SLProject/source/oglsl/VolumeRenderingSiddon_TF.frag
OTHER_FILES += ../lib-SLProject/source/oglsl/VolumeRenderingSiddon_MIP.frag
OTHER_FILES += ../lib-SLProject/source/oglsl/VolumeRenderingSampling_TF.frag
OTHER_FILES += ../lib-SLProject/source/oglsl/VolumeRenderingSampling_MIP.frag
OTHER_FILES += ../lib-SLProject/source/oglsl/VolumeRenderingSlicing.vert
OTHER_FILES += ../lib-SLProject/source/oglsl/VolumeRenderingSlicing.frag
