# -------------------------------------------------
# Project created by QtCreator 2010-02-24T14:55:33
# -------------------------------------------------
QT -= gui
TARGET = RegionWaveReader
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    chunk.cpp \
    regionwavesaver.cpp \
    RiffFileDefs.cpp \
    regionwavereader.cpp \
    read_wav_markers.cpp
HEADERS += chunk.h \
    RiffFileDefs.h \
    regionwavesaver.h \
    RegionWaveDefs.h \
    read_wave_regions.h \
    regionwavereader.h
INCLUDEPATH += E:\programs\MATLABr2009b\extern\include\
# LIBS += asd.lib
