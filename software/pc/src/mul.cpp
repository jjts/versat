#include "mul.hpp"

#if nMUL > 0

CMul::CMul()
{
}

CMul::CMul(int versat_base, int i, versat_t *databus)
{
    this->versat_base = versat_base;
    this->mul_base = i;
    this->databus = databus;
}

void CMul::start_run()
{
    //CMul has no delay
}

void CMul::update()
{
    int i = 0;

    //update databus
    databus[sMUL[mul_base]] = output_buff[MUL_LAT - 1];
    //special case for stage 0
    if (versat_base == 0)
    {
        //2nd copy at the end of global databus
        global_databus[nSTAGE * (1 << (N_W - 1)) + sMUL[mul_base]] = output_buff[MUL_LAT - 1];
    }

    //trickle down all outputs in buffer
    for (i = 1; i < MUL_LAT; i++)
    {
        output_buff[i] = output_buff[i - 1];
    }
    //insert new output
    output_buff[0] = out;
}

versat_t CMul::output()
{
    //select inputs
    opa = databus[sela];
    opb = databus[selb];

    mul_t result_mult = opa * opb;
    if (fns == MUL_HI)
    {
        result_mult = result_mult << 1;
        out = (versat_t)(result_mult >> (sizeof(versat_t) * 8));
    }
    else if (fns == MUL_DIV2_HI)
    {
        out = (versat_t)(result_mult >> (sizeof(versat_t) * 8));
    }
    else // MUL_LO
    {
        out = (versat_t)result_mult;
    }

    return out;
}

void CMul::setSelA(int sela)
{
    this->sela = sela;
}
void CMul::setSelB(int selb)
{
    this->selb = selb;
}
void CMul::setFNS(int fns)
{
    this->fns = fns;
}
void CMul::copy(CMul that)
{
    this->versat_base = that.versat_base;
    this->mul_base = that.mul_base;
    this->sela = that.sela;
    this->selb = that.selb;
    this->fns = that.fns;
}
string CMul::info()
{
    string ver = "mul[" + to_string(mul_base) + "]\n";
    ver += "SelA=       " + to_string(sela) + "\n";
    ver += "SelB=       " + to_string(selb) + "\n";
    ver += "FNS =       " + to_string(fns) + "\n";
    ver += "\n";
    return ver;
}
string CMul::info_iter()
{
    string ver = "mul[" + to_string(mul_base) + "]\n";
    ver += "SelA=       " + to_string(sela) + "\n";
    ver += "SelB=       " + to_string(selb) + "\n";
    ver += "inA =       " + to_string(opa) + "\n";
    ver += "inB =       " + to_string(opb) + "\n";
    ver += "Out=       " + to_string(out) + "\n";

    ver += "\n";
    return ver;
}

#endif