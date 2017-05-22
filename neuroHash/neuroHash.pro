CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread -lgomp
QMAKE_CXXFLAGS += -fopenmp

#QMAKE_CXXFLAGS += -O3

SOURCES += main.cpp \
    src/evolution/testphenotype.cpp \
    src/evolution/testphenotypediofant.cpp \
    src/evolution/trainerphenotype.cpp \
    src/neuralnetwork/Simple2x2x1Perceptron.cpp \
    blake/blake2b.c \
    blake/blake2bp.c \
    src/neuralnetwork/MultilayerPerceptron.cpp \
    src/testers/HashTester.cpp \
    src/global/globalstatistics.cpp \
    src/neuralnetwork/AnnHasher.cpp \
    src/neuralnetwork/CohonenNetwork.cpp \
    src/neuralnetwork/MaxEquation.cpp \
    src/neuralnetwork/PerceptronHasher.cpp \
    src/neuralnetwork/fastmlp.cpp

HEADERS += \
    src/evolution/phenotype.h \
    src/evolution/genetictrainer.h \
    src/global/global.h \
    src/evolution/village.h \
    src/evolution/testphenotype.h \
    src/evolution/testphenotypediofant.h \
    src/evolution/trainerphenotype.h \
    src/neuralnetwork/Simple2x2x1Perceptron.h \
    blake/blake2.h \
    blake/blake2b-load-sse2.h \
    blake/blake2b-load-sse41.h \
    blake/blake2b-round.h \
    blake/blake2-config.h \
    blake/blake2-impl.h \
    src/neuralnetwork/MultilayerPerceptron.h \
    src/testers/HashTester.h \
    src/neuralnetwork/annautoencoder.h \
    src/global/globalstatistics.h \
    src/neuralnetwork/AnnHasher.h \
    src/neuralnetwork/CohonenNetwork.h \
    src/neuralnetwork/MaxEquation.h \
    src/neuralnetwork/PerceptronHasher.h \
    src/neuralnetwork/fastmlp.h
