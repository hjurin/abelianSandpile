OS := $(shell uname)
ifeq ($(OS),Darwin)
CC := clang-omp
CCPP := clang++-omp
else
CC := gcc
CCPP := g++
endif

EXEC := sand ompOnly
CFLAGS := -Wall -g -O2 -std=c99 -lm -fopenmp
CPPFLAGS := -Wall -g -O2 -std=c++98 -fopenmp

ODIR := obj/
SDIR := src/
BDIR := bin/

_SANDPILES := ArraySandpileSeq1.o ArraySandpileSeq2.o ArraySandpileOMP1.o ArraySandpileOMP2.o ArraySandpileOMP3.o
SANDPILES := $(patsubst %,$(ODIR)%,$(_SANDPILES))

.phony: default clean

default: mkdir $(EXEC)

mkdir:
	@if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi
	@if [ ! -d $(BDIR) ]; then mkdir $(BDIR); fi

$(ODIR)%.o: $(SDIR)%.cpp $(SDIR)%.hpp $(SDIR)ArraySandpile.hpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@

$(ODIR)ArraySandpile.o: $(SDIR)ArraySandpile.cpp $(SDIR)Sandpile.hpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@

$(ODIR)Execution.o: $(SDIR)Execution.cpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@

$(ODIR)launcher.o: $(SDIR)launcher.cpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@

sand: $(ODIR)launcher.o $(SANDPILES) $(ODIR)ArraySandpile.o $(ODIR)Execution.o
	$(CCPP) $(CPPFLAGS) $^ -o $(BDIR)$@

ompOnly: $(SDIR)ompOnly.c
	$(CC) $^ -o $(BDIR)$@ $(CFLAGS)

clean:
	rm -rf $(BDIR) $(ODIR)
