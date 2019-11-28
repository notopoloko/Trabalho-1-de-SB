SOURCE=./source
CXX=g++
CXXFLAGS=-I $(SOURCE) -Wall -std=c++11

ODIR=./obj

all: $(ODIR)/main.o $(ODIR)/pre_proc.o $(ODIR)/ligador.o $(ODIR)/montador.o montador
default: all
debug_version: CXXFLAGS=-g -I $(SOURCE) -Wall
debug_version: clean $(ODIR)/main.o $(ODIR)/pre_proc.o $(ODIR)/ligador.o $(ODIR)/montador.o montador_debug;

# C++ -> .hpp
_DEPS = pre_proc.h montador.hpp ligador.h
DEPS = $(patsubst %,$(SOURCE)/%,$(_DEPS))

_OBJ = main.o pre_proc.o montador.o ligador.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/main.o: main.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# C++ -> .cpp
$(ODIR)/pre_proc.o: $(SOURCE)/pre_proc.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR)/ligador.o: $(SOURCE)/ligador.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR)/montador.o: $(SOURCE)/montador.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Gera executavel
montador: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Gere versao debug
montador_debug: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

simulador:
	$(CXX) -o $@ $(SOURCE)/simulador.c

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o montador montador_debug simulador