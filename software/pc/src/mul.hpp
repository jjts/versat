#include "type.hpp"

#if nMUL > 0
class CMul
{
private:
    versat_t opa = 0, opb = 0;
    versat_t output_buff[MUL_LAT]; //output FIFO
    versat_t out = 0;
    versat_t *databus = NULL;

public:
    int versat_base, mul_base;
    int sela = 0, selb = 0, fns = 0;
    //Default constructor
    CMul();

    CMul(int versat_base, int i, versat_t *databus);
    //set Mul configuration to shadow register

    //start run
    void start_run();

    //update output buffer, write results to databus
    void update();

    versat_t output();
    void copy(CMul that);
    void setSelA(int sela);
    void setSelB(int selb);
    void setFNS(int fns);

    string info();
    string info_iter();

}; //end class CMUL
extern int sMUL[nMUL];
extern versat_t global_databus[(nSTAGE + 1) * (1 << (N_W - 1))];
#endif