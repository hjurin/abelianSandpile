CC := g++
EXEC := sand ompOnly
CFLAGS	:= -Wall -g -O2 -std=c99 -fopenmp
CPPFLAGS := -Wall -g -O2 -std=c++98 -fopenmp

ODIR := obj
SDIR := src
BDIR := bin

_SANDPILES := ArraySandpileSeq1.o ArraySandpileSeq2.o ArraySandpileOMP1.o ArraySandpileOMP2.o ArraySandpileOMP3.o
SANDPILES := $(patsubst %,$(ODIR)/%,$(_SANDPILES))

.phony: default clean

default: $(EXEC)

$(ODIR)/%.o: $(SDIR)/%.cpp $(SDIR)/%.hpp $(SDIR)/ArraySandpile.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(ODIR)/ArraySandpile.o: $(SDIR)/ArraySandpile.cpp $(SDIR)/Sandpile.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(ODIR)/Execution.o: $(SDIR)/Execution.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(ODIR)/launcher.o: $(SDIR)/launcher.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

sand: $(ODIR)/launcher.o $(SANDPILES) $(ODIR)/ArraySandpile.o $(ODIR)/Execution.o
	$(CC) $(CPPFLAGS) $^ -o $(BDIR)/$@

ompOnly: ompOnly.c
	$(CC) $^ -o $(BDIR)/$@ $(CFLAGS)

clean:
	rm -rf $(BDIR)/* $(ODIR)/*
