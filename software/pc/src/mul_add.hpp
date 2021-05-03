#include "type.hpp"

#if nMULADD > 0
class CMulAdd
{
private:
    //SIM VARIABLES
    versat_t opa = 0, opb = 0, out = 0;
    mul_t acc = 0, acc_w = 0;
    int done = 0, duty = 0;
    int duty_cnt = 0, enable = 0;
    int shift_addr = 0, incr = 1, aux = 0, pos = 0;
    int loop2 = 0, loop1 = 0, cnt_addr = 0;
    //count delay during a run():
    int run_delay = 0;
    versat_t output_buff[MULADD_LAT] = {0}; //output FIFO
    versat_t *databus = NULL;

public:
    int versat_base, muladd_base;
    int sela = 0, selb = 0, fns = 0, iter = 0, per = 0, delay = 0, shift = 0;

    //Default constructor
    CMulAdd();

    CMulAdd(int versat_base, int i, versat_t *databus);

    //set MulAdd configuration to shadow register
    void update_shadow_reg_MulAdd();

    //start run
    void start_run();

    //update output buffer, write results to databus
    void update();
    versat_t output(); //implemented as PIPELINED MULADD

    void writeConf();
    uint32_t acumulator();
    void copy(CMulAdd that);
    void setSelA(int sela);
    void setSelB(int selb);
    void setFNS(int fns);
    void setIter(int iter);
    void setPer(int per);
    void setDelay(int delay);
    void setShift(int shift);

    string info();
    string info_iter();
}; //end class CMULADD

extern versat_t global_databus[(nSTAGE + 1) * (1 << (N_W - 1))];
extern int sMULADD[nMULADD];
#endif