#include "stage.hpp"

CStage::CStage()
{
}
//Default Constructor
CStage::CStage(int versat_base)
{

    //Define control and databus base address
    this->versat_base = versat_base;

    //set databus pointer
    this->databus = &(global_databus[(1 << (N_W - 1)) * (nSTAGE - versat_base)]);
    //special case for first stage
    if (versat_base == 0)
    {
        this->databus = global_databus;
    }

    //Init functional units
    int i;
#if nMEM > 0
    for (i = 0; i < nMEM; i++)
        memA[i] = CMemPort(versat_base, i, 0, databus);
    for (i = 0; i < nMEM; i++)
        memB[i] = CMemPort(versat_base, i, 1, databus);
#endif
#if nVI > 0
    for (i = 0; i < nVI; i++)
        vi[i] = CRead(versat_base, i, databus);
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
        vo[i] = CWrite(versat_base, i, databus);
#endif
#if nALU > 0
    for (i = 0; i < nALU; i++)
        alu[i] = CALU(versat_base, i, databus);
#endif
#if nALULITE > 0
    for (i = 0; i < nALULITE; i++)
        alulite[i] = CALULite(versat_base, i, databus);
#endif
#if nBS > 0
    for (i = 0; i < nBS; i++)
        bs[i] = CBS(versat_base, i, databus);
#endif
#if nMUL > 0
    for (i = 0; i < nMUL; i++)
        mul[i] = CMul(versat_base, i, databus);
#endif
#if nMULADD > 0
    for (i = 0; i < nMULADD; i++)
        muladd[i] = CMulAdd(versat_base, i, databus);
#endif
}

//clear Versat config
void CStage::clearConf()
{
    int i = versat_base;
    stage[i] = CStage(i);
}

#ifdef CONF_MEM_USE
//write current config in conf_mem
void CStage::confMemWrite(int addr)
{
    if (addr < CONF_MEM_SIZE)
        MEMSET(versat_base, (CONF_BASE + CONF_MEM + addr), 0);
}

//set addressed config in conf_mem as current config
void CStage::confMemRead(int addr)
{
    if (addr < CONF_MEM_SIZE)
        MEMGET(versat_base, (CONF_BASE + CONF_MEM + addr));
}
#endif

//set run start on all FUs
void CStage::start_all_FUs()
{
    int i = 0;
#if nMEM > 0
    for (i = 0; i < nMEM; i++)
        memA[i].start_run();
    for (i = 0; i < nMEM; i++)
        memB[i].start_run();
#endif
#if nVI > 0
    for (i = 0; i < nVI; i++)
        vi[i].start_run();
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
        vo[i].start_run();
#endif
#if nALU > 0
    for (i = 0; i < nALU; i++)
        alu[i].start_run();
#endif
#if nALULITE > 0
    for (i = 0; i < nALULITE; i++)
        alulite[i].start_run();
#endif
#if nBS > 0
    for (i = 0; i < nBS; i++)
        bs[i].start_run();
#endif
#if nMUL > 0
    for (i = 0; i < nMUL; i++)
        mul[i].start_run();
#endif
#if nMULADD > 0
    for (i = 0; i < nMULADD; i++)
        muladd[i].start_run();
#endif
}

//set update output buffers on all FUs
void CStage::update_all_FUs()
{
    int i = 0;
#if nMEM > 0
    for (i = 0; i < nMEM; i++)
        memA[i].update();
    for (i = 0; i < nMEM; i++)
        memB[i].update();
#endif
#if nVI > 0
    for (i = 0; i < nVI; i++)
        vi[i].update();
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
        vo[i].update();
#endif
#if nALU > 0
    for (i = 0; i < nALU; i++)
        alu[i].update();
#endif
#if nALULITE > 0
    for (i = 0; i < nALULITE; i++)
        alulite[i].update();
#endif
#if nBS > 0
    for (i = 0; i < nBS; i++)
        bs[i].update();
#endif
#if nMUL > 0
    for (i = 0; i < nMUL; i++)
        mul[i].update();
#endif
#if nMULADD > 0
    for (i = 0; i < nMULADD; i++)
        muladd[i].update();
#endif
}

//calculate new output on all FUs
void CStage::output_all_FUs()
{
    int i = 0;
#if nMEM > 0
    for (i = 0; i < nMEM; i++)
        memA[i].output();
    for (i = 0; i < nMEM; i++)
        memB[i].output();
#endif
#if nVI > 0
    for (i = 0; i < nVI; i++)
        vi[i].output();
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
        vo[i].output();
#endif
#if nALU > 0
    for (i = 0; i < nALU; i++)
        alu[i].output();
#endif
#if nALULITE > 0
    for (i = 0; i < nALULITE; i++)
        alulite[i].output();
#endif
#if nBS > 0
    for (i = 0; i < nBS; i++)
        bs[i].output();
#endif
#if nMUL > 0
    for (i = 0; i < nMUL; i++)
        mul[i].output();
#endif
#if nMULADD > 0
    for (i = 0; i < nMULADD; i++)
        muladd[i].output();
#endif
}

void CStage::copy(CStage that)
{
    int i = 0;
#if nMEM > 0
    //Memories
    for (i = 0; i < nMEM; i = i + 1)
    {
        this->memA[i].copy(that.memA[i]);
        this->memB[i].copy(that.memB[i]);
    }
#endif
#if nVI > 0
    for (i = 0; i < nVI; i++)
        this->vi[i].copy(that.vi[i]);
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
        this->vo[i].copy(that.vo[i]);
#endif
#if nALU > 0
    //ALUs
    for (i = 0; i < nALU; i = i + 1)
    {
        this->alu[i].copy(that.alu[i]);
    }
#endif

#if nALULITE > 0
    //ALULITEs
    for (i = 0; i < nALULITE; i = i + 1)
    {
        this->alulite[i].copy(that.alulite[i]);
    }
#endif

#if nMUL > 0
    //MULTIPLIERS
    for (i = 0; i < nMUL; i = i + 1)
    {
        this->mul[i].copy(that.mul[i]);
    }
#endif

#if nMULADD > 0
    //MULADDS
    for (i = 0; i < nMULADD; i = i + 1)
    {
        this->muladd[i].copy(that.muladd[i]);
    }
#endif

#if nBS > 0
    //BARREL SHIFTERS
    for (i = 0; i < nBS; i = i + 1)
    {
        this->bs[i].copy(that.bs[i]);
    }
#endif
}

bool CStage::done()
{
    bool auxA = true;
#if nMEM > 0
    bool auxB;
    auxA = memA[0].done;
    auxB = memB[0].done;
    for (int i = 1; i < nMEM; i++)
    {
        auxA = auxA && memA[i].done;
        auxB = auxB && memB[i].done;
    }
    auxA = auxA && auxB;
#endif
#if nVI > 0
    for (int i = 0; i < nVI; i++)
        auxA = auxA && vi[i].done;
#endif
#if nVO > 0
    for (int i = 0; i < nVO; i++)
        auxA = auxA && vo[i].done;
#endif
    return auxA;
}

void CStage::reset()
{
    for (int i = 0; i < 2 * N; i++)
    {
        databus[i] = 0;
    }
#if nMEM > 0
    for (int i = 0; i < nMEM; i++)
    {
        memA[i].done = 0;
        memB[i].done = 0;
    }
#endif
#if nVI > 0
    for (int i = 0; i < nVI; i++)
        vi[i].done = 0;
#endif
#if nVO > 0
    for (int i = 0; i < nVO; i++)
        vo[i].done = 0;
#endif
}
string CStage::info()
{
    int i = 0;
    string ver = "nStage " + to_string(versat_base) + "\n";
#if nMEM > 0
    for (i = 0; i < nMEM; i++)
    {
        ver += memA[i].info();
        ver += memB[i].info();
    }
#endif
#if nVI > 0
    for (i = 0; i < nVI; i++)
        ver += vi[i].info();
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
        ver += vo[i].info();
#endif
#if nALU > 0
    for (i = 0; i < nALU; i++)
    {
        ver += alu[i].info();
    }
#endif
#if nALULITE > 0
    for (i = 0; i < nALULITE; i++)
    {
        ver += alulite[i].info();
    }
#endif
#if nBS > 0
    for (i = 0; i < nBS; i++)
    {
        ver += bs[i].info();
    }
#endif
#if nMUL > 0
    for (i = 0; i < nMUL; i++)
    {
        ver += mul[i].info();
    }
#endif
#if nMULADD > 0
    for (i = 0; i < nMULADD; i++)
    {
        ver += muladd[i].info();
    }
#endif

    return ver;
}

string CStage::info_iter()
{
    int i = 0;
    string ver = "nStage " + to_string(versat_base) + "\n";
#if nMEM > 0
    for (i = 0; i < nMEM; i++)
    {
        ver += memA[i].info_iter();
        ver += memB[i].info_iter();
    }
#endif
#if nVI > 0
    for (i = 0; i < nVI; i++)
        ver += vi[i].info_iter();
#endif
#if nVO > 0
    for (i = 0; i < nVO; i++)
        ver += vo[i].info_iter();
#endif
#if nALU > 0
    for (i = 0; i < nALU; i++)
    {
        ver += alu[i].info_iter();
    }
#endif
#if nALULITE > 0
    for (i = 0; i < nALULITE; i++)
    {
        ver += alulite[i].info_iter();
    }
#endif
#if nBS > 0
    for (i = 0; i < nBS; i++)
    {
        ver += bs[i].info_iter();
    }
#endif
#if nMUL > 0
    for (i = 0; i < nMUL; i++)
    {
        ver += mul[i].info_iter();
    }
#endif
#if nMULADD > 0
    for (i = 0; i < nMULADD; i++)
    {
        ver += muladd[i].info_iter();
    }
#endif
    return ver;
}
