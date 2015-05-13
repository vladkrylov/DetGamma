# $Id: GNUmakefile 68025 2013-03-13 13:43:46Z gcosmo $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := mainDetGamma
G4TARGET := $(name)
G4EXLIB := true

LOCALDIR := ${PWD}

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

include $(G4INSTALL)/config/architecture.gmk

ifndef PLATFORM
  PLATFORM = slc4_ia32_gcc34
endif

# ROOT support
CPPFLAGS += -I$(shell root-config --incdir) 
EXTRALIBS = $(shell root-config --glibs) -lReflex -lCintex
ROOTBIN =  $(shell root-config --bindir)
ROOTINC =  $(shell root-config --incdir)
GCCXMLPATH=/usr/bin

DICTIONARYPATH := ${G4WORKDIR}/tmp/${G4SYSTEM}/${G4TARGET}/dictionary

.PHONY: all
all: dictionary lib bin

dictionary:

	mkdir -p ${DICTIONARYPATH}
	${ROOTBIN}/genreflex ${LOCALDIR}/include/DetGammaClasses.hh \
	-s ${LOCALDIR}/xml/selection.xml -o ${DICTIONARYPATH}/DetGammaClasses_rflx.cpp \
	--gccxmlpath ${GCCXMLPATH} -Iinclude -I${G4INCLUDE} -I${CLHEP_INCLUDE_DIR} -I${ROOTINC}

	$(CXX) -o ${G4WORKDIR}/tmp/${G4SYSTEM}/${G4TARGET}/libDetGammaClassesDict.so -fpic -shared \
	${DICTIONARYPATH}/DetGammaClasses_rflx.cpp src/*.cc \
	-Iinclude -I${ROOTINC} -I${G4INCLUDE} -I${CLHEP_INCLUDE_DIR} \
	${EXTRALIBS} -L${G4LIB}/${G4SYSTEM} ${LDLIBS2} ${LOADLIBS}

#	$(CXX) -o ${G4WORKDIR}/bin/${G4SYSTEM}/makePlots src/*.cc makePlots.cc \
#	-Iinclude -I${ROOTINC} -I${G4INCLUDE} -I${CLHEP_INCLUDE_DIR} \
#	${EXTRALIBS} -L${G4LIB}/${G4SYSTEM} ${LDLIBS2} ${LDLIBS3} ${LOADLIBS}

#	$(CXX) -o ${G4WORKDIR}/bin/${G4SYSTEM}/makeDetectorConstructionPlots src/*.cc makeDetectorConstructionPlots.cc \
#	-Iinclude -I${ROOTINC} -I${G4INCLUDE} -I${CLHEP_INCLUDE_DIR} \
#	${EXTRALIBS} -L${G4LIB}/${G4SYSTEM} ${LDLIBS2} ${LDLIBS3} ${LOADLIBS}

clean_all: clean
	@$(RM) 

include $(G4INSTALL)/config/binmake.gmk
