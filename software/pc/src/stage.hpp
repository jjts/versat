#include "type.hpp"
#include "alu.hpp"
#include "alu_lite.hpp"
#include "bs.hpp"
#include "mem.hpp"
#include "mul.hpp"
#include "mul_add.hpp"
#include "vread.hpp"
#include "vwrite.hpp"
class CStage
{
private:
public:
    int versat_base;
    versat_t *databus;
    //versat_t* databus[N*2];
    //Versat Function Units
#if nMEM > 0
    CMemPort memA[nMEM];
    CMemPort memB[nMEM];
#endif
#if nVI > 0
    CRead vi[nVI];
#endif
#if nVO > 0
    CWrite vo[nVO];
#endif
#if nALU > 0
    CALU alu[nALU];
#endif
#if nALULITE > 0
    CALULite alulite[nALULITE];
#endif
#if nBS > 0
    CBS bs[nBS];
#endif
#if nMUL > 0
    CMul mul[nMUL];
#endif
#if nMULADD > 0
    CMulAdd muladd[nMULADD];
#endif

    //Default constructor
    CStage();
    //Default Constructor
    CStage(int versat_base);
    //clear Versat config
    void clearConf();

#ifdef CONF_MEM_USE
    //write current config in conf_mem
    void confMemWrite(int addr);

    //set addressed config in conf_mem as current config
    void confMemRead(int addr);
#endif

    //set run start on all FUs
    void start_all_FUs();

    //set update output buffers on all FUs
    void update_all_FUs();

    //calculate new output on all FUs
    void output_all_FUs();
    void copy(CStage that);
    string info();
    string info_iter();

    bool done();
    void reset();

}; //end class CStage

extern int base;
extern CStage stage[nSTAGE];
extern CStage shadow_reg[nSTAGE];
extern CMem versat_mem[nSTAGE][nMEM];
extern int versat_iter;
extern int versat_debug;
extern versat_t global_databus[(nSTAGE + 1) * (1 << (N_W - 1))];
/*databus vector
stage 0 is repeated in the start and at the end
stage order in databus
[ 0 | nSTAGE-1 | nSTAGE-2 | ... | 2  | 1 | 0 ]
^                                    ^
|                                    |
stage 0 databus                      stage 1 databus

*/
#if nMEM > 0
extern int sMEMA[nMEM], sMEMA_p[nMEM], sMEMB[nMEM], sMEMB_p[nMEM];
#endif
#if nVI > 0
extern int sVI[nVI], sVI_p[nVI];
#endif
#if nVO > 0
extern int sVO[nVO], sVO_p[nVO];
#endif
#if nALU > 0
extern int sALU[nALU], sALU_p[nALU];
#endif
#if nALULITE > 0
extern int sALULITE[nALULITE], sALULITE_p[nALULITE];
#endif
#if nMUL > 0
extern int sMUL[nMUL], sMUL_p[nMUL];
#endif
#if nMULADD > 0
extern int sMULADD[nMULADD], sMULADD_p[nMULADD];
#endif
#if nBS > 0
extern int sBS[nBS], sBS_p[nBS];
#endif