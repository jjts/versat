#include "versat.h"
#include <iostream>
#include <bitset>

#define SIMD 0

#if DATAPATH_W == 16
typedef int16_t versat_t;
typedef int32_t mul_t;
typedef uint32_t shift_t;
#elif DATAPATH_W == 8
typedef int8_t versat_t;
typedef int16_t mul_t;
typedef uint16_t shift_t;
#else
typedef int32_t versat_t;
typedef int64_t mul_t;
typedef uint64_t shift_t;

#endif

using namespace std;

#define SET_BITS(var, val, size)   \
    for (int i = 0; i < size; i++) \
    {                              \
        var.set(i, val);           \
    }

#define SET_BITS_SEXT8(var, val, size) \
    for (int i = size - 1; i > 7; i--) \
    {                                  \
        var.set(i, val);               \
    }

#define SET_BITS_SEXT16(var, val, size) \
    for (int i = size - 1; i > 15; i--) \
    {                                   \
        var.set(i, val);                \
    }

#define MSB(var, size) var >> (size - 1)

//constants
#define CONF_BASE (1 << (nMEM_W + MEM_ADDR_W + 1))
#define CONF_MEM_SIZE ((int)pow(2, CONF_MEM_ADDR_W))
//#define MEM_SIZE ((int)pow(2,MEM_ADDR_W))
#define MEM_SIZE (1 << MEM_ADDR_W)
#define RUN_DONE (1 << (nMEM_W + MEM_ADDR_W))