#include "alu_lite.hpp"
#if nALULITE > 0

CALULite::CALULite()
{
}

CALULite::CALULite(int versat_base, int i, versat_t *databus)
{
    this->versat_base = versat_base;
    this->alulite_base = i;
    this->databus = databus;
}

void CALULite::start_run()
{
    //CALULite has no delay
    for (int i = 0; i < ALULITE_LAT; i++)
        output_buff[i] = 0;
}

void CALULite::update()
{
    int i = 0;

    //update databus
    //trickle down all outputs in buffer
    versat_t aux_output = output_buff[0];
    versat_t aux_output2 = 0;
    //trickle down all outputs in buffer
    for (i = 1; i < ALULITE_LAT; i++)
    {
        aux_output2 = output_buff[i];
        output_buff[i] = aux_output;
        aux_output = aux_output2;
    }
    //insert new output
    output_buff[0] = out;

    databus[sALULITE[alulite_base]] = output_buff[ALULITE_LAT - 1];
    //special case for stage 0
    if (versat_base == 0)
    {
        //2nd copy at the end of global databus
        global_databus[nSTAGE * (1 << (N_W - 1)) + sALULITE[alulite_base]] = output_buff[ALULITE_LAT - 1];
    }
}

versat_t CALULite::output()
{
    bitset<DATAPATH_W + 1> ai;
    bitset<DATAPATH_W + 1> bz;
    SET_BITS(ai, 0, DATAPATH_W + 1);
    SET_BITS(bz, 1, DATAPATH_W + 1);
    bitset<DATAPATH_W> aux_cmp;

    //cast to int cin.to_ulong();
    ina = databus[opa];
    loop = fns < 0 ? 1 : 0;
    inb = databus[opb];
    ina_loop = loop ? out : ina;

    versat_t op_a_reg = ina;
    versat_t self_loop = loop;
    versat_t op_b_reg = inb;
    versat_t op_a_int = ina_loop;

    switch (fns)
    {
    case ALULITE_OR:
        out = op_a_int | op_b_reg;
        break;
    case ALULITE_AND:
        out = op_a_int & op_b_reg;
        break;
    case ALULITE_CMP_SIG:
        aux_cmp.set(DATAPATH_W - 1, op_a_int > op_b_reg ? 1 : 0);
        out = (versat_t)aux_cmp.to_ulong();
        break;
    case ALULITE_MUX:
        out = op_a_reg < 0 ? op_b_reg : self_loop == 1 ? out : 0;
        break;
    case ALULITE_SUB:
        out = op_a_reg < 0 ? op_b_reg : op_a_int - op_b_reg;
        break;
    case ALULITE_ADD:
        out = op_a_int + op_b_reg;
        if (self_loop)
            out = op_a_reg < 0 ? op_b_reg : out;
        break;
    case ALULITE_MAX:
        out = op_a_reg < 0 ? out : max(op_a_int, op_b_reg);
        break;
    case ALULITE_MIN:
        out = op_a_reg < 0 ? out : min(op_a_int, op_b_reg);
        break;
    default:
        break;
    }

    return out;
}

void CALULite::setOpA(int opa)
{
    this->opa = opa;
}

void CALULite::setOpB(int opb)
{
    this->opb = opb;
}

void CALULite::setFNS(int fns)
{
    this->fns = fns;
}
void CALULite::copy(CALULite that)
{
    this->versat_base = that.versat_base;
    this->alulite_base = that.alulite_base;
    this->opa = that.opa;
    this->opb = that.opb;
    this->fns = that.fns;
}
string CALULite::info()
{
    string ver = "alu_lite[" + to_string(alulite_base) + "]\n";
    ver += "SetOpA=       " + to_string(opa) + "\n";
    ver += "SetOpB=       " + to_string(opb) + "\n";
    ver += "FNS =       " + to_string(fns) + "\n";
    ver += "\n";
    return ver;
}

string CALULite::info_iter()
{
    string ver = "alu_lite[" + to_string(alulite_base) + "]\n";
    ver += "opa=" + to_string(ina) + "\n";
    ver += "SetOpA=       " + to_string(opa) + "\n";
    ver += "SetOpB=       " + to_string(opb) + "\n";
    ver += "opa_loop=" + to_string(ina_loop) + "\n";
    ver += "opb=" + to_string(inb) + "\n";
    ver += "self_loop" + to_string(loop) + "\n";
    ver += "out=" + to_string(out) + "\n";
    ver += "OUTPUT_BUFFER (LATENCY SIM)\n";
    for (int z = 0; z < ALULITE_LAT; z++)
    {
        ver += "Output[" + to_string(z) + "]=" + to_string(output_buff[z]) + "\n";
    }
    ver += "\n";
    return ver;
}
#endif