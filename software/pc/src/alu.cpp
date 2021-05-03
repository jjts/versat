#include "alu.hpp"
#if nALU > 0

CALU::CALU()
{
}

CALU::CALU(int versat_base, int i, versat_t *databus)
{
    this->versat_base = versat_base;
    this->alu_base = i;
    this->databus = databus;
}

void CALU::start_run()
{
    //CAlu has no delay
}

void CALU::update()
{
    int i = 0;

    //update databus
    databus[sALU[alu_base]] = output_buff[ALU_LAT - 1];
    //special case for stage 0
    if (versat_base == 0)
    {
        //2nd copy at the end of global databus
        global_databus[nSTAGE * (1 << (N_W - 1)) + sALU[alu_base]] = output_buff[ALU_LAT - 1];
    }

    //trickle down all outputs in buffer
    versat_t aux_output = output_buff[0];
    versat_t aux_output2 = 0;
    //trickle down all outputs in buffer
    for (i = 1; i < ALU_LAT; i++)
    {
        aux_output2 = output_buff[i];
        output_buff[i] = aux_output;
        aux_output = aux_output2;
    }
    //insert new output
    output_buff[0] = out;
}

versat_t CALU::output()
{
    inb = databus[opa];
    ina = databus[opb];
    bitset<DATAPATH_W> aux_sext;
    bitset<DATAPATH_W> aux_cmp;
    uint8_t aux_a = ina;
    bool val;
    shift_t aux = ina;
    shift_t ina_uns = ina;
    shift_t inb_uns = inb;

    switch (fns)
    {
    case ALU_OR:
        out = inb | ina;
        break;
    case ALU_AND:
        out = inb & ina;
        break;
    case ALU_XOR:
        out = inb ^ ina;
        break;
    case ALU_SEXT8:
        aux_a = ina;
        val = MSB(aux_a, 8);
        SET_BITS_SEXT8(aux_sext, val, DATAPATH_W);
        out = aux_sext.to_ulong() + aux_a;
        break;
    case ALU_SEXT16:
        val = MSB(aux_a, 16);
        SET_BITS_SEXT16(aux_sext, val, DATAPATH_W);
        out = aux_sext.to_ulong() + aux_a;
        break;
    case ALU_SHIFTR_ARTH:
        out = ina >> 1;
        break;
    case ALU_SHIFTR_LOG:
        aux = aux >> 1;
        out = (versat_t)aux;
        break;
    case ALU_CMP_SIG:
        aux_cmp.set(DATAPATH_W - 1, ina > inb ? 1 : 0);
        out = (versat_t)aux_cmp.to_ulong();
        break;
    case ALU_CMP_UNS:
        ina_uns = ina;
        inb_uns = inb;
        aux_cmp.set(DATAPATH_W - 1, ina_uns > inb_uns ? 1 : 0);
        out = (versat_t)aux_cmp.to_ulong();
        break;
    case ALU_MUX:
        out = ina < 0 ? inb : 0;
        break;
    case ALU_ADD:
        out = ina + inb;
        break;
    case ALU_SUB:
        out = inb - ina;
        break;
    case ALU_MAX:
        out = ina > inb ? ina : inb;
        break;
    case ALU_MIN:
        out = ina < inb ? ina : inb;
        break;
    default:
        break;
    }
    return out;
}

void CALU::setOpA(int opa)
{
    this->opa = opa;
}

void CALU::setOpB(int opb)
{
    this->opb = opb;
}

void CALU::setFNS(int fns)
{
    this->fns = fns;
}
void CALU::copy(CALU that)
{
    this->versat_base = that.versat_base;
    this->alu_base = that.alu_base;
    this->opa = that.opa;
    this->opb = that.opb;
    this->fns = that.fns;
}
string CALU::info()
{
    string ver = "alu[" + to_string(alu_base) + "]\n";
    ver += "SetOpA=       " + to_string(opa) + "\n";
    ver += "SelOpB=       " + to_string(opb) + "\n";
    ver += "FNS =       " + to_string(fns) + "\n";
    ver += "\n";
    return ver;
}
string CALU::info_iter()
{
    string ver = "alu[" + to_string(alu_base) + "]\n";
    ver += "opa=" + to_string(ina) + "\n";
    ver += "SetOpA=       " + to_string(opa) + "\n";
    ver += "SetOpB=       " + to_string(opb) + "\n";
    ver += "opb=" + to_string(inb) + "\n";
    ver += "out=" + to_string(out) + "\n";
    ver += "OUTPUT_BUFFER (LATENCY SIM)\n";
    for (int z = 0; z < ALU_LAT; z++)
    {
        ver += "Output[" + to_string(z) + "]=" + to_string(output_buff[z]) + "\n";
    }
    ver += "\n";
    return ver;
}
#endif