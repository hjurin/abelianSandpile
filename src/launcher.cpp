#define _XOPEN_SOURCE 600

#include <iostream>
#include <string>
#include <stdlib.h>
#include <getopt.h>
#include "omp.h"

#include "Execution.hpp"
#include "Sandpile.hpp"
#include "ArraySandpileSeq1.hpp"
#include "ArraySandpileSeq2.hpp"
#include "ArraySandpileOMP1.hpp"
#include "ArraySandpileOMP2.hpp"
#include "ArraySandpileOMP3.hpp"

#define GLOBAL_HEIGHT 5
#define PEAK_HEIGHT 100000

int main (int argc, char **argv)
{
  int size = -1;
  int config = -1;
  int dispMode = -1;
  int version = -1;
  int batchSize = -1;

  std::string usage = ("Usage: " + std::string(argv[0]) + " -b value -c value -d value -s value -v value\n" +
    "-b (batchSize): the states of the sandpile are computed batchSize by batchSize.\n" +
    "-c (configuration): 0 for an homogenous configuration, 1 for a peak configuration.\n" +
    "-d (display mode): 0 for duration only, 1 for final state and duration, 2 for all states and duration.\n" +
    "-s (size): the sandpile's size is s x s.\n" +
    "-v (version): 1 for seq1, 2 for seq2, 3 for omp1, 4 for omp2, 5 for gpu.\n\n");

  int opt;
  while ((opt = getopt(argc, argv, "b:c:d:s:v:")) != -1) {
    switch (opt) {
      case 'b':
        batchSize = atoi(optarg);
        break;
      case 'c':
        config = atoi(optarg);
        break;
      case 'd':
        dispMode = atoi(optarg);
        break;
      case 's':
        size = atoi(optarg);
        break;
      case 'v':
        version = atoi(optarg);
        break;
      case '?':
      default:
        std::cerr << usage;
        abort();
    }
  }

  if (batchSize < 1 || config < 0 || config > 1 || dispMode < 0 ||
    dispMode > 2 || size < 1 || version < 1 || version > 5)
  {
    std::cerr << usage;
    return 1;
  }

  Sandpile* pile;
  switch (version) {
    case 1:
      pile = new ArraySandpileSeq1(size,
                                  (1-config)*GLOBAL_HEIGHT,
                                  config*PEAK_HEIGHT + (1-config)*GLOBAL_HEIGHT);
      break;
    case 2:
      pile = new ArraySandpileSeq2(size,
                                  (1-config)*GLOBAL_HEIGHT, 
                                  config*PEAK_HEIGHT + (1-config)*GLOBAL_HEIGHT);
      break;
    case 3:
      pile = new ArraySandpileOMP1(size,
                                  (1-config)*GLOBAL_HEIGHT, 
                                  config*PEAK_HEIGHT + (1-config)*GLOBAL_HEIGHT,
                                  omp_get_max_threads());
      break;
    case 4:
      pile = new ArraySandpileOMP2(size,
                                  (1-config)*GLOBAL_HEIGHT, 
                                  config*PEAK_HEIGHT + (1-config)*GLOBAL_HEIGHT,
                                  omp_get_max_threads());
      break;
    case 5:
      pile = new ArraySandpileOMP3(size,
                                  (1-config)*GLOBAL_HEIGHT, 
                                  config*PEAK_HEIGHT + (1-config)*GLOBAL_HEIGHT,
                                  omp_get_max_threads());
      break;
    default:
      return 1;
  }

  Execution *exec = new Execution(*pile , dispMode, batchSize);
  exec->start();
  std::cout << "Finished in " << exec->executionTime() << " ms." << std::endl;
  delete exec;
  delete pile;
  return 0;
}
