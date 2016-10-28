CC := g++
EXEC := sand ompOnly
CFLAGS	:=	-Wall -g -O2 -std=c++11 -fopenmp
LDFLAGS := -Wall -g -O2 -std=c++11 -fopenmp
SANDPILES := ArraySandpileSeq1.o ArraySandpileSeq2.o ArraySandpileOMP1.o ArraySandpileOMP2.o ArraySandpileOMP3.o

BIN_REP := ../bin/

.phony: default clean

default: $(EXEC)

ArraySandpile.o: ArraySandpile.cpp Sandpile.hpp
	$(CC) $(CFLAGS) -c $<

ArraySandpileSeq1.o: ArraySandpileSeq1.cpp ArraySandpileSeq1.hpp ArraySandpile.hpp
	$(CC) $(CFLAGS) -c $<

ArraySandpileSeq2.o: ArraySandpileSeq2.cpp ArraySandpileSeq2.hpp ArraySandpile.hpp
	$(CC) $(CFLAGS) -c $<

ArraySandpileOMP1.o: ArraySandpileOMP1.cpp ArraySandpileOMP1.hpp ArraySandpile.hpp
	$(CC) $(CFLAGS) -c $<

ArraySandpileOMP2.o: ArraySandpileOMP2.cpp ArraySandpileOMP2.hpp ArraySandpile.hpp
	$(CC) $(CFLAGS) -c $<

ArraySandpileOMP3.o: ArraySandpileOMP3.cpp ArraySandpileOMP3.hpp ArraySandpile.hpp
	$(CC) $(CFLAGS) -c $<

Execution.o: Execution.cpp
	$(CC) $(CFLAGS) -c $<

launcher.o: launcher.cpp
	$(CC) $(CFLAGS) -c $<

sand: launcher.o $(SANDPILES) ArraySandpile.o Execution.o
	$(CC) $(LDFLAGS) $^ -o $(BIN_REP)$@

ompOnly: ompOnly.c
	$(CC) $^ -o $(BIN_REP)$@ $(CFLAGS)

clean:
	rm -rf $(BIN_REP)*
