

#OBJDIR = obj
SRCDIR = src
RESDIR = bin


PROGRAM = TaskManagerTest

INCLUDEDIRS = 
LIBDIRS = 
LIBS = 

LDFLAGS =  $(LIBDIRS) $(LIBS)  -Wl -lrt

CXX = g++
CC = gcc

CXXSOURCES := main.cpp TaskManager.c TaskManager.cpp
CXXOBJECTS := $(CXXSOURCES)
#CXXOBJECTS := $(CXXSOURCES:.c=_c.o) 
#CXXOBJECTS := $(CXXSOURCES:.cpp=_cpp.o)
CXXOBJECTS := $(CXXOBJECTS:.c=_c.o) 
CXXOBJECTS := $(CXXOBJECTS:.cpp=_cpp.o)

CXXFLAGS = -ggdb  -fno-inline -DESRI_UNIX -time $(INCLUDEDIRS)
CCFLAGS =  -ggdb  -DESRI_UNIX -time  $(INCLUDEDIRS)


all: $(PROGRAM)

#
# Link target: automatically builds its object dependencies before
# executing its link command.
#


$(PROGRAM): MKDIR $(CXXOBJECTS)
	$(CXX)  -o $(RESDIR)/$@  $(CXXOBJECTS) $(LDFLAGS)

MKDIR:
	mkdir -p $(RESDIR)

#
# Object targets: rules that define objects, their dependencies, and
# a list of commands for compilation.
#

main_cpp.o: $(SRCDIR)/main.cpp $(SRCDIR)/TaskManagerImpl.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRCDIR)/main.cpp

TaskManager_c.o: $(SRCDIR)/TaskManager.c $(SRCDIR)/TaskManagerImpl.h
	$(CC) $(CCFLAGS) -c -o $@ $(SRCDIR)/TaskManager.c


TaskManager_cpp.o: $(SRCDIR)/TaskManager.cpp $(SRCDIR)/TaskManagerImpl.h $(SRCDIR)/TaskManager.hpp
	$(CXX) $(CCFLAGS) -c -o $@ $(SRCDIR)/TaskManager.cpp


clean:  FORCE
	$(RM) $(CXXOBJECTS) $(PROGRAM)

run:
	$(RESDIR)/$(PROGRAM) -tio 7 -tt 3

doc:   FORCE 
	doxygen

FORCE:
