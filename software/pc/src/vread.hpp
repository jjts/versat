#include "type.hpp"

class CMem;

#if nVI > 0
class CRead
{
private:
#if YOLO_VERSAT == 1
    int offset = 0;
#endif
    int delay = 0;
    int duty = 0;
    int extAddr = 0, extIter = 0, extPer = 0, extShift = 0, extIncr = 0;
    int intAddr = 0, intIter = 0, intPer = 0, intShift = 0, intIncr = 0, intStart = 0;
    int intIter2 = 0, intPer2 = 0, intShift2 = 0, intIncr2 = 0;

    //SIM VARIABLES
    int pos = 0, pos2 = 0, run_delay = 0;
    bool next_half = false;
    versat_t output_port[MEMP_LAT], out = 0;
    versat_t *databus = NULL;
    versat_t mem[MEM_SIZE] = {0};

    //AGU VARIABLES
    int loop1 = 0,
        loop2 = 0, loop3 = 0, loop4 = 0;
    int enable = 0;
    int duty_cnt = 0;
    versat_t aux = 0;

public:
    int base, versat_base, done = 0;

    //Default constructor
    CRead();

    //Constructor with an associated base
    CRead(int versat_base, int base, versat_t *databus);
    CRead(int versat_base, int base);

    //Methods to set config parameters
    void setExtAddr(int extAddr);
#if YOLO_VERSAT == 1
    void setOffset(int offset);
#endif
    void setIntAddr(int intAddr);
    void setExtIter(int iter);
    void setExtPer(int per);
    void setExtShift(int shift);
    void setExtIncr(int incr);
    void setIntStart(int start);
    void setIntIter(int iter);
    void setIntPer(int per);
    void setIntShift(int shift);
    void setIntIncr(int incr);
    void setIntIter2(int iter);
    void setIntPer2(int per);
    void setIntShift2(int shift);
    void setIntIncr2(int incr);
    void setDuty(int duty);
    void setDelay(int delay);
    //SIM
    //start run
    void start_run();

    //update output buffer, write results to databus
    void update();

    versat_t output();
    void getdatafromDDR();
    versat_t AGU();
    versat_t addrgen2loop();
    versat_t addrgen4loop();
    versat_t read(int addr);
    void copy(CRead that);
    void copy_ext(CRead that);

    string info();
    string info_iter();
};
extern versat_t global_databus[(nSTAGE + 1) * (1 << (N_W - 1))];
extern int sVI[nVI];
extern versat_t *FPGA_mem;

#endif