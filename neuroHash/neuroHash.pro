TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS += -O3

SOURCES += main.cpp \
    src/evolution/testphenotype.cpp \
    src/evolution/testphenotypediofant.cpp \
    src/evolution/trainerphenotype.cpp

HEADERS += \
    src/evolution/phenotype.h \
    src/evolution/genetictrainer.h \
    src/global.h \
    src/evolution/village.h \
    src/evolution/testphenotype.h \
    src/evolution/testphenotypediofant.h \
    src/evolution/trainerphenotype.h
