#include "mul_add.hpp"
#if nMULADD > 0
class CStage;

CMulAdd::CMulAdd()
{
}

CMulAdd::CMulAdd(int versat_base, int i, versat_t *databus)
{
    this->versat_base = versat_base;
    this->muladd_base = i;
    this->databus = databus;
}

//start run
void CMulAdd::start_run()
{
    //set run_delay
    run_delay = delay;

    //set addrgen counter variables
    loop1 = 0;
    duty = per;
    loop2 = 0;
    pos = 0;
    shift_addr = -per;
    duty_cnt = 0;
    cnt_addr = 0;
    done = 0;

    for (int i = 0; i < MULADD_LAT; i++)
        output_buff[i] = 0;
}

//update output buffer, write results to databus
void CMulAdd::update()
{
    int i = 0;
    //check for delay
    if (run_delay > 0)
    {
        run_delay--;
    }
    else
    {
        //trickle down all outputs in buffer
        versat_t aux_output = output_buff[0];
        versat_t aux_output2 = 0;
        //trickle down all outputs in buffer
        for (i = 1; i < MULADD_LAT; i++)
        {
            aux_output2 = output_buff[i];
            output_buff[i] = aux_output;
            aux_output = aux_output2;
        }
        //insert new output
        output_buff[0] = out;
        //update databus
        databus[sMULADD[muladd_base]] = output_buff[MULADD_LAT - 1];
        //special case for stage 0
        if (versat_base == 0)
        {
            //2nd copy at the end of global databus
            global_databus[nSTAGE * (1 << (N_W - 1)) + sMULADD[muladd_base]] = output_buff[MULADD_LAT - 1];
        }
    }
}

versat_t CMulAdd::output() //implemented as PIPELINED MULADD
{
    //wait for delay to end
    if (run_delay > 0)
    {
        return 0;
    }

    //select inputs
    opa = databus[sela];
    opb = databus[selb];

    cnt_addr = acumulator();

    //select acc_w value
    acc_w = (cnt_addr == 0) ? 0 : acc;

    //perform MAC operation
    mul_t result_mult = opa * opb;
    if (fns == MULADD_MACC)
    {
        acc = acc_w + result_mult;
    }
    else
    {
        acc = acc_w - result_mult;
    }
    out = (versat_t)(acc >> shift);

    return out;
}

uint32_t CMulAdd::acumulator()
{
    if (loop2 < iter)
    {
        if (loop1 < per)
        {
            loop1++;
            enable = 0;
            if (duty_cnt < duty)
            {
                enable = 1;
                aux = pos;
                duty_cnt++;
                pos += incr;
            }
        }
        if (loop1 == per)
        {
            loop1 = 0;
            duty_cnt = 0;
            loop2++;
            pos += shift_addr;
        }
    }
    if (loop2 == iter)
    {
        loop2 = 0;
        done = 1;
    }
    return aux;
}

void CMulAdd::setSelA(int sela)
{
    this->sela = sela;
}
void CMulAdd::setSelB(int selb)
{
    this->selb = selb;
}
void CMulAdd::setFNS(int fns)
{
    this->fns = fns;
}
void CMulAdd::setIter(int iter)
{
    this->iter = iter;
}
void CMulAdd::setPer(int per)
{
    this->per = per;
}
void CMulAdd::setDelay(int delay)
{
    this->delay = delay;
}
void CMulAdd::setShift(int shift)
{
    this->shift = shift;
}
void CMulAdd::copy(CMulAdd that)
{
    this->versat_base = that.versat_base;
    this->muladd_base = that.muladd_base;
    this->sela = that.sela;
    this->selb = that.selb;
    this->fns = that.fns;
    this->iter = that.iter;
    this->per = that.per;
    this->delay = that.delay;
    this->shift = that.shift;
}

string CMulAdd::info()
{
    string ver = "mul_add[" + to_string(muladd_base) + "]\n";
    ver += "SelA=     " + to_string(sela) + "\n";
    ver += "SelB=     " + to_string(selb) + "\n";
    ver += "FNS =     " + to_string(fns) + "\n";
    ver += "Iter=     " + to_string(iter) + "\n";
    ver += "Per =     " + to_string(per) + "\n";
    ver += "Delay=    " + to_string(delay) + "\n";
    ver += "Shift=    " + to_string(shift) + "\n";
    ver += "\n";
    return ver;
}

string CMulAdd::info_iter()
{
    string ver = "mul_add[" + to_string(muladd_base) + "]\n";
    ver += "OpA=     " + to_string(opa) + "\n";
    ver += "OpB=     " + to_string(opb) + "\n";
    ver += "SelA=     " + to_string(sela) + "\n";
    ver += "SelB=     " + to_string(selb) + "\n";
    ver += "Addr=     " + to_string(cnt_addr) + "\n";
    ver += "Finished=     " + to_string(done) + "\n";
    ver += "Out=     " + to_string(out) + "\n";
    ver += "OUTPUT_BUFFER (LATENCY SIM)\n";
    for (int z = 0; z < MULADD_LAT; z++)
    {
        ver += "Output[" + to_string(z) + "]=" + to_string(output_buff[z]) + "\n";
    }
    ver += "\n";
    return ver;
}
#endif