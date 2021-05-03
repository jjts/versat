#include "type.hpp"

#if nALULITE > 0
extern versat_t global_databus[(nSTAGE + 1) * (1 << (N_W - 1))];
extern int sALULITE[nALULITE];
class CALULite
{
private:
    versat_t ina = 0, inb = 0, out = 0;
    versat_t ina_loop = 0;
    versat_t loop = 0;
    versat_t output_buff[ALULITE_LAT] = {0}; //output FIFO
    versat_t *databus = NULL;

public:
    int versat_base, alulite_base;
    int opa = 0, opb = 0, fns = 0;

    //Default constructor
    CALULite();
    CALULite(int versat_base, int i, versat_t *databus);

    //start run
    void start_run();

    //update output buffer, write results to databus
    void update();
    versat_t output();
    void copy(CALULite that);

    void setOpA(int opa);
    void setOpB(int opb);
    void setFNS(int fns);

    string info();
    string info_iter();
}; //end class CALUALITE
#endif