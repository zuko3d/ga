CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread -lgomp
QMAKE_CXXFLAGS += -fopenmp
#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_CXXFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2

#QMAKE_CXXFLAGS += -O3

SOURCES += main.cpp \
    src/evolution/testphenotype.cpp \
    src/evolution/testphenotypediofant.cpp \
    src/evolution/trainerphenotype.cpp \
    ../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.cpp \
    ../blake/blake2b.c \
    ../blake/blake2bp.c \
    ../NeuroGeneticHash/NeuroGeneticHash/MultilayerPerceptron.cpp \
    ../NeuroGeneticHash/NeuroGeneticHash/HashTester.cpp \
    globalstatistics.cpp

HEADERS += \
    src/evolution/phenotype.h \
    src/evolution/genetictrainer.h \
    src/global.h \
    src/evolution/village.h \
    src/evolution/testphenotype.h \
    src/evolution/testphenotypediofant.h \
    src/evolution/trainerphenotype.h \
    ../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h \
    ../blake/blake2.h \
    ../blake/blake2b-load-sse2.h \
    ../blake/blake2b-load-sse41.h \
    ../blake/blake2b-round.h \
    ../blake/blake2-config.h \
    ../blake/blake2-impl.h \
    ../NeuroGeneticHash/NeuroGeneticHash/MultilayerPerceptron.h \
    ../NeuroGeneticHash/NeuroGeneticHash/HashTester.h \
    annautoencoder.h \
    globalstatistics.h
