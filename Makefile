#
# Makefile for PropNuclei package
#
# 


ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)


CXXFLAGS      = -O -Wall -fPIC -D_REENTRANT $(CXXOPTS)
CXXFLAGS     += $(ROOTCFLAGS)  -I /software/Externals/fftw/3.3.4/include

ifeq ($(DEBUG),1)
CXXFLAGS      += -D_DEBUG
endif

LIBS          = $(ROOTLIBS) -lMinuit

CXX           = g++
LD            = g++
LDFLAGS       = 

CXXINCLU     +=  -I. $(ROOTCFLAGS) -I $(ROOTSYS)/include 
LIBS          = $(ROOTLIBS) 
GLIBS         = $(ROOTGLIBS)


LDFLAGS      +=  $(ROOTGLIBS)  -lMinuit


FFTWLIBS      = -L/software/Externals/fftw/3.3.4/lib
FFTWFLAGS     =  -lfftw3


PROGRAM       = covid

SRCS	      = covid.cpp 

# SR	      = src/Output.cc src/Particle.cc src/NucModel.cc src/MathFunctions.cc src/PhysicsFunctions.cc 

#OBJ           = $(patsubst %.cc, %.o, $(SRCS) $(SR)) 
OBJ           = $(patsubst %.cc, %.o, $(SRCS)) 

src           = $(addprefix ./,$(SRCS))
obj           = $(addprefix ./,$(OBJ))
prg           = $(addprefix ./,$(PROGRAM))

# CONVERT       = Convert
# SRCC          = src/Convert.cc
# OBJC          = $(patsubst %.cc, %.o, $(SRCC))
# objc           = $(addprefix ./,$(OBJC)) 
# con           = $(addprefix ./,$(CONVERT))


# -----------------------------------------------------------------------------

all:: $(prg) $(con)

$(con): $(objc) 
	$(CXX) -o $@ $^ $(LDFLAGS)  $(FFTWFLAGS) $(FFTWLIBS)

$(prg):$(obj) 
#	@if ! [ -x bin ]; then mkdir bin ; fi
	$(CXX) -o $@ $^ $(LDFLAGS) $(FFTWFLAGS) $(FFTWLIBS) $(CXXINCLU)
#	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#%.o : %.cc %.h
#$(CXX) $(CXXFLAGS) -c $<   -o $@ 
#%.o : %.cc 
#$(CXX) $(CXXFLAGS) -c $<   -o $@ 
#%.o : %.c %.h
#	$(CXX) $(CXXFLAGS) -c $<   -o $@ 

%.o : %.cc %.hh
	$(CXX) $(CXXFLAGS) -c $< $(CXXINCLU)  -o $@
%.o : %.cc %.h
	$(CXX) $(CXXFLAGS) -c $< $(CXXINCLU)  -o $@
%.o : %.cc 
	$(CXX) $(CXXFLAGS) -c $< $(CXXINCLU)  -o $@
%.o : %.c %.h
	$(CXX) $(CXXFLAGS) -c $< $(CXXINCLU)  -o $@


clean::
	rm -f $(con)
	rm -f $(prg)
# -----------------------------------------------------------------------------

