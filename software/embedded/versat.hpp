#include "versat.h"

//Macro functions to use cpu interface
#define MEMSET(base, location, value) (*((volatile int *)(base + (sizeof(int)) * location)) = value)
#define MEMGET(base, location) (*((volatile int *)(base + (sizeof(int)) * location)))

//constants
#define CONF_BASE (1 << (BASE_ADDR_W + 1))
#define CONF_MEM_SIZE ((int)pow(2, CONF_MEM_ADDR_W))
#define MEM_SIZE ((int)pow(2, MEM_ADDR_W))
#define RUN_DONE (1 << (nMEM_W + MEM_ADDR_W))

//
// VERSAT CLASSES
//
#if nMEM > 0
class CMemPort
{
public:
  int versat_base, mem_base, data_base;

  //Default constructor
  CMemPort()
  {
  }

  //Constructor with an associated base
  CMemPort(int versat_base, int i, int offset)
  {
    this->versat_base = versat_base;
    this->mem_base = CONF_BASE + CONF_MEM0A + (2 * i + offset) * MEMP_CONF_OFFSET;
    this->data_base = (i << MEM_ADDR_W);
  }

  //Methods to set config parameters
  void setIter(int iter)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_ITER), iter);
  }
  void setPer(int per)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_PER), per);
  }
  void setDuty(int duty)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_DUTY), duty);
  }
  void setSel(int sel)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_SEL), sel);
  }
  void setStart(int start)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_START), start);
  }
  void setIncr(int incr)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_INCR), incr);
  }
  void setShift(int shift)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_SHIFT), shift);
  }
  void setDelay(int delay)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_DELAY), delay);
  }
  void setExt(int ext)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_EXT), ext);
  }
  void setRvrs(int rvrs)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_RVRS), rvrs);
  }
  void setInWr(int in_wr)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_IN_WR), in_wr);
  }
  void setIter2(int iter2)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_ITER2), iter2);
  }
  void setPer2(int per2)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_PER2), per2);
  }
  void setIncr2(int incr2)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_INCR2), incr2);
  }
  void setShift2(int shift2)
  {
    MEMSET(versat_base, (this->mem_base + MEMP_CONF_SHIFT2), shift2);
  }

  //methods to read/write from/to memory
  void write(int addr, int val)
  {
    MEMSET(versat_base, (this->data_base + addr), val);
  }
  int read(int addr)
  {
    return MEMGET(versat_base, (this->data_base + addr));
  }
}; //end class CMEM
#endif

#if nVI > 0
class CVI
{
public:
  int versat_base, vi_base;

  //Default constructor
  CVI()
  {
  }

  //Constructor with an associated base
  CVI(int versat_base, int i)
  {
    this->versat_base = versat_base;
    this->vi_base = CONF_BASE + CONF_VI0 + i * VI_CONF_OFFSET;
  }

  //Methods to set config parameters
  void setExtAddr(int extAddr)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_EXT_ADDR), extAddr);
  }
  void setIntAddr(int intAddr)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_INT_ADDR), intAddr);
  }
  void setExtSize(int size)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_SIZE), size);
  }
  void setExtIter(int iter)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_ITER_A), iter);
  }
  void setExtPer(int per)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_PER_A), per);
  }
  void setExtDuty(int duty)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_DUTY_A), duty);
  }
  void setExtShift(int shift)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_SHIFT_A), shift);
  }
  void setExtIncr(int incr)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_INCR_A), incr);
  }
  void setIntIter(int iter)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_ITER_B), iter);
  }
  void setIntPer(int per)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_PER_B), per);
  }
  void setIntDuty(int duty)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_DUTY_B), duty);
  }
  void setIntStart(int start)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_START_B), start);
  }
  void setIntShift(int shift)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_SHIFT_B), shift);
  }
  void setIntIncr(int incr)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_INCR_B), incr);
  }
  void setIntDelay(int delay)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_DELAY_B), delay);
  }
  void setIntRVRS(int rvrs)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_RVRS_B), rvrs);
  }
  void setIntExt(int ext)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_EXT_B), ext);
  }
  void setIntIter2(int iter)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_ITER2_B), iter);
  }
  void setIntPer2(int per)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_PER2_B), per);
  }
  void setIntShift2(int shift)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_SHIFT2_B), shift);
  }
  void setIntIncr2(int incr)
  {
    MEMSET(versat_base, (this->vi_base + VI_CONF_INCR2_B), incr);
  }
}; //end class CVI
#endif

#if nVO > 0
class CVO
{
public:
  int versat_base, vo_base;

  //Default constructor
  CVO()
  {
  }

  //Constructor with an associated base
  CVO(int versat_base, int i)
  {
    this->versat_base = versat_base;
    this->vo_base = CONF_BASE + CONF_VO0 + i * VO_CONF_OFFSET;
  }

  //Methods to set config parameters
  void setExtAddr(int extAddr)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_EXT_ADDR), extAddr);
  }
  void setIntAddr(int intAddr)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_INT_ADDR), intAddr);
  }
  void setExtSize(int size)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_SIZE), size);
  }
  void setExtIter(int iter)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_ITER_A), iter);
  }
  void setExtPer(int per)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_PER_A), per);
  }
  void setExtDuty(int duty)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_DUTY_A), duty);
  }
  void setExtShift(int shift)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_SHIFT_A), shift);
  }
  void setExtIncr(int incr)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_INCR_A), incr);
  }
  void setIntIter(int iter)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_ITER_B), iter);
  }
  void setIntPer(int per)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_PER_B), per);
  }
  void setIntDuty(int duty)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_DUTY_B), duty);
  }
  void setIntSel(int sel)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_SEL_B), sel);
  }
  void setIntStart(int start)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_START_B), start);
  }
  void setIntShift(int shift)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_SHIFT_B), shift);
  }
  void setIntIncr(int incr)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_INCR_B), incr);
  }
  void setIntDelay(int delay)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_DELAY_B), delay);
  }
  void setIntRVRS(int rvrs)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_RVRS_B), rvrs);
  }
  void setIntExt(int ext)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_EXT_B), ext);
  }
  void setIntIter2(int iter)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_ITER2_B), iter);
  }
  void setIntPer2(int per)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_PER2_B), per);
  }
  void setIntShift2(int shift)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_SHIFT2_B), shift);
  }
  void setIntIncr2(int incr)
  {
    MEMSET(versat_base, (this->vo_base + VO_CONF_INCR2_B), incr);
  }
}; //end class CVI
#endif

#if nALU > 0
class CALU
{
public:
  int versat_base, alu_base;

  //Default constructor
  CALU()
  {
  }

  CALU(int versat_base, int i)
  {
    this->versat_base = versat_base;
    this->alu_base = CONF_BASE + CONF_ALU0 + i * ALU_CONF_OFFSET;
  }

  //Methods to set config parameters
  void setOpA(int opa)
  {
    MEMSET(versat_base, (this->alu_base + ALU_CONF_SELA), opa);
  }
  void setOpB(int opb)
  {
    MEMSET(versat_base, (this->alu_base + ALU_CONF_SELB), opb);
  }
  void setFNS(int fns)
  {
    MEMSET(versat_base, (this->alu_base + ALU_CONF_FNS), fns);
  }
}; //end class CALU
#endif

#if nALULITE > 0
class CALULite
{
public:
  int versat_base, alulite_base;

  //Default constructor
  CALULite()
  {
  }

  CALULite(int versat_base, int i)
  {
    this->versat_base = versat_base;
    this->alulite_base = CONF_BASE + CONF_ALULITE0 + i * ALULITE_CONF_OFFSET;
  }

  //Methods to set config parameters
  void setOpA(int opa)
  {
    MEMSET(versat_base, (this->alulite_base + ALULITE_CONF_SELA), opa);
  }
  void setOpB(int opb)
  {
    MEMSET(versat_base, (this->alulite_base + ALULITE_CONF_SELB), opb);
  }
  void setFNS(int fns)
  {
    MEMSET(versat_base, (this->alulite_base + ALULITE_CONF_FNS), fns);
  }
}; //end class CALUALITE
#endif

#if nBS > 0
class CBS
{
public:
  int versat_base, bs_base;

  //Default constructor
  CBS()
  {
  }

  CBS(int versat_base, int i)
  {
    this->versat_base = versat_base;
    this->bs_base = CONF_BASE + CONF_BS0 + i * BS_CONF_OFFSET;
  }

  //Methods to set config parameters
  void setData(int data)
  {
    MEMSET(versat_base, (this->bs_base + BS_CONF_SELD), data);
  }
  void setShift(int shift)
  {
    MEMSET(versat_base, (this->bs_base + BS_CONF_SELS), shift);
  }
  void setFNS(int fns)
  {
    MEMSET(versat_base, (this->bs_base + BS_CONF_FNS), fns);
  }
}; //end class CBS
#endif

#if nMUL > 0
class CMul
{
public:
  int versat_base, mul_base;

  //Default constructor
  CMul()
  {
  }

  CMul(int versat_base, int i)
  {
    this->versat_base = versat_base;
    this->mul_base = CONF_BASE + CONF_MUL0 + i * MUL_CONF_OFFSET;
  }

  //Methods to set config parameters
  void setSelA(int sela)
  {
    MEMSET(versat_base, (this->mul_base + MUL_CONF_SELA), sela);
  }
  void setSelB(int selb)
  {
    MEMSET(versat_base, (this->mul_base + MUL_CONF_SELB), selb);
  }
  void setFNS(int fns)
  {
    MEMSET(versat_base, (this->mul_base + MUL_CONF_FNS), fns);
  }
}; //end class CMUL
#endif

#if nMULADD > 0
class CMulAdd
{
public:
  int versat_base, muladd_base;

  //Default constructor
  CMulAdd()
  {
  }

  CMulAdd(int versat_base, int i)
  {
    this->versat_base = versat_base;
    this->muladd_base = CONF_BASE + CONF_MULADD0 + i * MULADD_CONF_OFFSET;
  }

  //Methods to set config parameters
  void setSelA(int sela)
  {
    MEMSET(versat_base, (this->muladd_base + MULADD_CONF_SELA), sela);
  }
  void setSelB(int selb)
  {
    MEMSET(versat_base, (this->muladd_base + MULADD_CONF_SELB), selb);
  }
  void setFNS(int fns)
  {
    MEMSET(versat_base, (this->muladd_base + MULADD_CONF_FNS), fns);
  }
  void setIter(int iter)
  {
    MEMSET(versat_base, (this->muladd_base + MULADD_CONF_ITER), iter);
  }
  void setPer(int per)
  {
    MEMSET(versat_base, (this->muladd_base + MULADD_CONF_PER), per);
  }
  void setDelay(int delay)
  {
    MEMSET(versat_base, (this->muladd_base + MULADD_CONF_DELAY), delay);
  }
  void setShift(int shift)
  {
    MEMSET(versat_base, (this->muladd_base + MULADD_CONF_SHIFT), shift);
  }
}; //end class CMULADD
#endif

class CStage
{

public:
  int versat_base;
  //Versat Function Units
#if nMEM > 0
  CMemPort memA[nMEM];
  CMemPort memB[nMEM];
#endif
#if nVI > 0
  CVI vi[nVI];
#endif
#if nVO > 0
  CVO vo[nVO];
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
  CStage()
  {
  }

  //Default Constructor
  CStage(int versat_base)
  {

    //Define control and databus base address
    this->versat_base = versat_base;

    //Init functional units
    int i;
#if nMEM > 0
    for (i = 0; i < nMEM; i++)
      memA[i] = CMemPort(versat_base, i, 0);
    for (i = 0; i < nMEM; i++)
      memB[i] = CMemPort(versat_base, i, 1);
    a
#endif
#if nVI > 0
        for (i = 0; i < nVI; i++)
            vi[i] = Cread(versat_base, i);
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
      vo[i] = Cwrite(versat_base, i);
#endif
#if nALU > 0
    for (i = 0; i < nALU; i++)
      alu[i] = CALU(versat_base, i);
#endif
#if nALULITE > 0
    for (i = 0; i < nALULITE; i++)
      alulite[i] = CALULite(versat_base, i);
#endif
#if nBS > 0
    for (i = 0; i < nBS; i++)
      bs[i] = CBS(versat_base, i);
#endif
#if nMUL > 0
    for (i = 0; i < nMUL; i++)
      mul[i] = CMul(versat_base, i);
#endif
#if nMULADD > 0
    for (i = 0; i < nMULADD; i++)
      muladd[i] = CMulAdd(versat_base, i);
#endif
  }

  //clear Versat config
  void clearConf()
  {
    MEMSET(versat_base, (CONF_BASE + CONF_CLEAR), 0);
  }

#ifdef CONF_MEM_USE
  //write current config in conf_mem
  void confMemWrite(int addr)
  {
    if (addr < CONF_MEM_SIZE)
      MEMSET(versat_base, (CONF_BASE + CONF_MEM + addr), 0);
  }

  //set addressed config in conf_mem as current config
  void confMemRead(int addr)
  {
    if (addr < CONF_MEM_SIZE)
      MEMGET(versat_base, (CONF_BASE + CONF_MEM + addr));
  }
#endif
}; //end class CStage

//
//VERSAT global variables
//
static int base;
CStage stage[nSTAGE];
#if nMEM > 0
int sMEMA[nMEM], sMEMA_p[nMEM], sMEMB[nMEM], sMEMB_p[nMEM];
#endif
#if nVI > 0
int sVI[nVI], sVI_p[nVI];
#endif

#if nALU > 0
int sALU[nALU], sALU_p[nALU];
#endif
#if nALULITE > 0
int sALULITE[nALULITE], sALULITE_p[nALULITE];
#endif
#if nMUL > 0
int sMUL[nMUL], sMUL_p[nMUL];
#endif
#if nMULADD > 0
int sMULADD[nMULADD], sMULADD_p[nMULADD];
#endif
#if nBS > 0
int sBS[nBS], sBS_p[nBS];
#endif

//
//VERSAT FUNCTIONS
//
inline void versat_init(int base_addr)
{

  //init versat stages
  int i;
  base = base_addr;
  for (i = 0; i < nSTAGE; i++)
    stage[i] = CStage(base_addr + (i << (CTR_ADDR_W - nSTAGE_W + 2))); //+2 as RV32 is not byte addressable

  //prepare sel variables
  int p_offset = (1 << (N_W - 1));
  int s_cnt = 0;

#if nMEM > 0
  //Memories
  for (i = 0; i < nMEM; i = i + 1)
  {
    sMEMA[i] = s_cnt + 2 * i;
    sMEMB[i] = sMEMA[i] + 1;
    sMEMA_p[i] = sMEMA[i] + p_offset;
    sMEMB_p[i] = sMEMB[i] + p_offset;
  }
  s_cnt += 2 * nMEM;
#endif

#if nVI > 0
  //Vread
  for (i = 0; i < nVI; i = i + 1)
  {
    sVI[i] = s_cnt + i;
    sVI_p[i] = sVI[i] + p_offset;
  }
  s_cnt += nVI;
#endif

#if nALU > 0
  //ALUs
  for (i = 0; i < nALU; i = i + 1)
  {
    sALU[i] = s_cnt + i;
    sALU_p[i] = sALU[i] + p_offset;
  }
  s_cnt += nALU;
#endif

#if nALULITE > 0
  //ALULITEs
  for (i = 0; i < nALULITE; i = i + 1)
  {
    sALULITE[i] = s_cnt + i;
    sALULITE_p[i] = sALULITE[i] + p_offset;
  }
  s_cnt += nALULITE;
#endif

#if nMUL > 0
  //MULTIPLIERS
  for (i = 0; i < nMUL; i = i + 1)
  {
    sMUL[i] = s_cnt + i;
    sMUL_p[i] = sMUL[i] + p_offset;
  }
  s_cnt += nMUL;
#endif

#if nMULADD > 0
  //MULADDS
  for (i = 0; i < nMULADD; i = i + 1)
  {
    sMULADD[i] = s_cnt + i;
    sMULADD_p[i] = sMULADD[i] + p_offset;
  }
  s_cnt += nMULADD;
#endif

#if nBS > 0
  //BARREL SHIFTERS
  for (i = 0; i < nBS; i = i + 1)
  {
    sBS[i] = s_cnt + i;
    sBS_p[i] = sBS[i] + p_offset;
  }
#endif
}

inline void run()
{
  MEMSET(base, (RUN_DONE), 1);
}

inline int done()
{
  return MEMGET(base, (RUN_DONE));
}

inline void globalClearConf()
{
  MEMSET(base, (CONF_BASE + GLOBAL_CONF_CLEAR), 0);
}
