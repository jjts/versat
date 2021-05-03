#ifndef VERSAT_SIM
#define VERSAT_SIM
#include <iostream>
#include <thread>
#include "versat.h"
#include <string.h>
#include <bitset>
#include "stage.hpp"

//
// VERSAT CLASSES
//
class CMem;
class CMemPort;
class CALU;
class CALULite;
class CMul;
class CMulAdd;
class CStage;
//
//VERSAT global variables
//
#ifndef VERSAT_cpp // include guard
#define VERSAT_cpp

//
//VERSAT FUNCTIONS
//
void versat_init(int base_addr);

extern int run_done;

void *run_sim();

void run();

int done();

void globalClearConf();
#endif

#endif

#define INFO 1

#if INFO == 1

void print_versat_mems();
void print_versat_info();
void print_versat_iter();

#endif
