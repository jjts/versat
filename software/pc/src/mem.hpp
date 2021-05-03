#include "type.hpp"
#if nMEM > 0

class CMem
{
private:
    versat_t data[MEM_SIZE];
    versat_t read(uint32_t addr);
    void write(uint32_t addr, versat_t data_in);

public:
    friend class CMemPort;
};

class CMemPort // 4 Loop AGU
{
private:
    //count delay during a run():
    int run_delay = 0;
    versat_t out = 0;
    int enable = 0;
    versat_t output_port[MEMP_LAT] = {0}; //output FIFO
    int loop1 = 0, loop2 = 0, loop3 = 0, loop4 = 0;
    uint32_t pos = 0;
    uint32_t pos2 = 0;
    uint32_t aux = 0;
    int duty_cnt = 0;

public:
    CMem *my_mem;
    int versat_base, mem_base, data_base;
    int iter, per, duty, sel, start, shift, incr, delay, in_wr /* read or write*/;
    int rvrs = 0 /* reverse addr*/, ext = 0 /* use FU to addr MEM*/, iter2 = 0, per2 = 0, shift2 = 0, incr2 = 0;
    bool done = 0;
    versat_t *databus = NULL;

    //Default constructor
    CMemPort();
    //Constructor with an associated base
    CMemPort(int versat_base, int i, int offset, versat_t *databus);
    //set MEMPort configuration to shadow register
    //start run
    void start_run();
    //update output buffer, write results to databus
    void update();

    versat_t output();
    uint32_t AGU();
    uint32_t acumulator();
    void setIter(int iter);
    void setPer(int per);
    void setDuty(int duty);
    void setSel(int sel);
    void setStart(int start);
    void setIncr(int incr);
    void setShift(int shift);
    void setDelay(int delay);
    void setExt(int ext);
    void setRvrs(int rvrs);
    void setInWr(int in_wr);
    void setIter2(int iter2);
    void setPer2(int per2);
    void setIncr2(int incr);
    void setShift2(int shift2);
    void copy(CMemPort that);

    void write(int addr, int val);
    int read(int addr);
    void reset();
    string info();
    string info_iter();
}; //end class CMEM

extern CMem versat_mem[nSTAGE][nMEM];
extern int sMEMA[nMEM];
extern int sMEMB[nMEM];
extern versat_t global_databus[(nSTAGE + 1) * (1 << (N_W - 1))];
#endif