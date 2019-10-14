SOURCE=./source
CXX=g++
CXXFLAGS=-I $(SOURCE) -Wall -std=c++11

ODIR=./obj

all: $(ODIR)/main.o $(ODIR)/pre_proc.o $(ODIR)/montador.o pre_proc
default: all
debug_version: CXXFLAGS=-g -I $(SOURCE) -Wall
debug_version: clean $(ODIR)/main.o $(ODIR)/pre_proc.o $(ODIR)/montador.o pre_proc_debug;

# C++ -> .hpp
_DEPS = pre_proc.h montador.hpp
DEPS = $(patsubst %,$(SOURCE)/%,$(_DEPS))

_OBJ = main.o pre_proc.o montador.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/main.o: main.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# C++ -> .cpp
$(ODIR)/pre_proc.o: $(SOURCE)/pre_proc.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR)/montador.o: $(SOURCE)/montador.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Gera executavel
pre_proc: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Gere versao debug
pre_proc_debug: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o pre_proc pre_proc_debug