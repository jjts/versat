#include "vread.hpp"
#if nVI > 0

CRead::CRead()
{
}
CRead::CRead(int versat_base, int base, versat_t *databus)
{
    this->base = base;
    this->versat_base = versat_base;
    this->databus = databus;
}
CRead::CRead(int versat_base, int base)
{
    this->base = base;
    this->versat_base = versat_base;
}

void CRead::setExtAddr(int extAddr)
{
    this->extAddr = extAddr;
}
#if YOLO_VERSAT == 1
void CRead::setOffset(int offset)
{
    this->offset = offset;
}
#endif
void CRead::setIntAddr(int intAddr)
{
    this->intAddr = intAddr;
}
void CRead::setExtIter(int iter)
{
    this->extIter = iter;
}
void CRead::setExtPer(int per)
{
    this->extPer = per;
}
void CRead::setExtShift(int shift)
{
    this->extShift = shift;
}
void CRead::setExtIncr(int incr)
{
    this->extIncr = incr;
}
void CRead::setIntStart(int start)
{
    this->intStart = start;
}
void CRead::setIntIter(int iter)
{
    this->intIter = iter;
}
void CRead::setIntPer(int per)
{
    this->intPer = per;
}
void CRead::setIntShift(int shift)
{
    this->intShift = shift;
}
void CRead::setIntIncr(int incr)
{
    this->intIncr = incr;
}
void CRead::setIntIter2(int iter)
{
    this->intIter2 = iter;
}
void CRead::setIntPer2(int per)
{
    this->intPer2 = per;
}
void CRead::setIntShift2(int shift)
{
    this->intShift2 = shift;
}
void CRead::setIntIncr2(int incr)
{
    this->intIncr2 = incr;
}
void CRead::setDelay(int delay)
{
    this->delay = delay;
}
void CRead::setDuty(int duty)
{
    this->duty = duty;
}

void CRead::copy(CRead that)
{
    this->versat_base = that.versat_base;
    this->base = that.base;
    this->intIncr = that.intIncr;
    this->intIncr2 = that.intIncr2;
    this->intShift = that.intShift;
    this->intStart = that.intStart;
    this->intShift2 = that.intShift2;
    this->intPer = that.intPer;
    this->intPer2 = that.intPer2;
    this->intIter = that.intIter;
    this->intIter2 = that.intIter2;
    this->delay = that.delay;
    this->duty = that.duty;
}
void CRead::copy_ext(CRead that)
{
    this->versat_base = that.versat_base;
    this->base = that.base;
    this->extAddr = that.extAddr;
    this->extIncr = that.extIncr;
    this->extIter = that.extIter;
    this->extPer = that.extPer;
    this->extShift = that.extShift;
    this->intAddr = that.intAddr;
}
string CRead::info()
{
    string ver = "vread[" + to_string(base) + "]\n";
    ver += "extAddr=       " + to_string(extAddr) + "\n";
    ver += "extIncr=      " + to_string(extIncr) + "\n";
    ver += "extIter=       " + to_string(extIter) + "\n";
    ver += "extPer=         " + to_string(extPer) + "\n";
    ver += "extShift=       " + to_string(extShift) + "\n";
    ver += "intIncr=        " + to_string(intIncr) + "\n";
    ver += "intIncr2=       " + to_string(intIncr2) + "\n";
    ver += "intShift=       " + to_string(intShift) + "\n";
    ver += "intStart=       " + to_string(intStart) + "\n";
    ver += "intShift2=      " + to_string(intShift2) + "\n";
    ver += "intPer=         " + to_string(intPer) + "\n";
    ver += "intPer2=        " + to_string(intPer2) + "\n";
    ver += "intIter=        " + to_string(intIter) + "\n";
    ver += "intIter2=       " + to_string(intIter2) + "\n";
    ver += "intAddr=        " + to_string(intAddr) + "\n";
    ver += "\n";
    return ver;
}
string CRead::info_iter()
{
    string ver = "CRead[" + to_string(base) + "]\n" + "]\n" + "VI MEM DATA[\n";
    for (int i = 0; i < MEM_SIZE; i++)
    {
        ver += "mem[" + to_string(i) + "]=" + to_string(mem[i]) + "\n";
    }
    ver += "]\n";
    ver += "Out=    " + to_string(out) + "\n";
    ver += "Addr=    " + to_string(aux) + "\n";
    ver += "OUTPUT_BUFFER (LATENCY SIM)\n";
    for (int z = 0; z < MEMP_LAT; z++)
    {
        ver += "Output[" + to_string(z) + "]=" + to_string(output_port[z]) + "\n";
    }
    ver += "\n";
    return ver;
}

void CRead::start_run()
{

    done = 0;
    pos = intStart;
    pos2 = intStart;
    if (duty == 0)
        duty = intPer;
    //set run_delay
    run_delay = delay;

    loop1 = 0;
    loop2 = 0;
    loop3 = 0;
    loop4 = 0;
    duty_cnt = 0;
    enable = 0;

    if ((intIter != 0 || intPer != 0) && (extPer != 0 || extIter != 0))
    {
        next_half = !next_half;
    }

    // (extPer != 0 || extIter != 0)
    getdatafromDDR();

    if ((intIter != 0 || intPer != 0) && (extPer != 0 || extIter != 0))
    {
        next_half = !next_half;
    }

    for (int i = 0; i < MEMP_LAT; i++)
        output_port[i] = 0;
}

void CRead::getdatafromDDR()
{
    addrgen2loop();
}

void CRead::update()
{
    int i = 0;
    //check for delay
    if (run_delay > 0)
    {
        run_delay--;
    }
    else
    {
        versat_t aux_output = output_port[0];
        versat_t aux_output2 = 0;
        //trickle down all outputs in buffer
        for (i = 1; i < MEMP_LAT; i++)
        {
            aux_output2 = output_port[i];
            output_port[i] = aux_output;
            aux_output = aux_output2;
        }
        //insert new output
        output_port[0] = out; //TO DO: change according to output()

        //update databus
        databus[sVI[base]] = output_port[MEMP_LAT - 1];
        //special case for stage 0
        if (versat_base == 0)
        {
            //2nd copy at the end of global databus
            global_databus[nSTAGE * (1 << (N_W - 1)) + sVI[base]] = output_port[MEMP_LAT - 1];
        }
    }
}

versat_t CRead::AGU()
{
    uint32_t aux_acc = 0;
    if (done == 0)
        aux_acc = addrgen4loop();
    if (done == 1)
    {
        pos2 = intStart;
        pos = intStart;
        if ((intIter != 0 || intPer != 0) && (extPer != 0 || extIter != 0))
        {
            next_half = !next_half;
        }
    }
    return aux_acc;
}

versat_t CRead::addrgen2loop()
{
    versat_t dat = 0;
    int addr = extAddr / (DATAPATH_W / 8);
    int iaddr = intAddr;
    for (int i = 0; i < extIter; i++)
    {
        for (int j = 0; j < extPer; j++)
        {
            dat = FPGA_mem[addr];
            if (next_half == 0)
            {
                mem[iaddr] = dat;
            }
            else
            {
                mem[iaddr + next_half] = dat;
            }
            addr += extIncr;
            iaddr += 1;
        }
        addr += extShift;
    }
    return 0;
}

versat_t CRead::addrgen4loop()
{
    if (intIter2 == 0 && intPer2 == 0)
    {
        if (loop2 < intIter)
        {
            if (loop1 < intPer)
            {
                loop1++;
                enable = 0;
                if (duty_cnt < duty)
                {
                    enable = 1;
                    aux = pos;
                    duty_cnt++;
                    pos += intIncr;
                }
            }
            if (loop1 == intPer)
            {
                loop1 = 0;
                duty_cnt = 0;
                loop2++;
                pos += intShift;
            }
        }
        if (loop2 == intIter)
        {
            loop2 = 0;
            done = 1;
        }
    }
    else
    {
        if (loop4 < intIter2)
        {
            if (loop3 < intPer2)
            {
                if (loop2 < intIter)
                {
                    if (loop1 < intPer)
                    {
                        loop1++;
                        enable = 0;
                        if (duty_cnt < duty)
                        {
                            enable = 1;
                            aux = pos;
                            duty_cnt++;
                            pos += intIncr;
                        }
                    }
                    if (loop1 == intPer)
                    {
                        loop1 = 0;
                        loop2++;
                        duty_cnt = 0;
                        pos += intShift;
                    }
                }
                if (loop2 == intIter)
                {
                    loop2 = 0;
                    pos2 += intIncr2;
                    pos = pos2;
                    loop3++;
                }
            }
            if (loop3 == intPer2)
            {
                pos2 += intShift2;
                pos = pos2;
                loop3 = 0;
                loop4++;
            }
        }
        if (loop4 == intIter2)
        {
            done = 1;
        }
    }
    return aux;
}

versat_t CRead::output()
{
    if (run_delay > 0)
        return 0;

    int addr = 0;
    if (done == 0)
    {
        addr = AGU();
    }
    else
    {
        return out;
    }

    versat_t data;
    data = read(addr);
    out = data;
    return 0;
}

extern versat_t *FPGA_mem;
versat_t CRead::read(int addr)
{
    if (next_half == true)
        return mem[addr + MEM_SIZE / 2];
    else
    {
        return mem[addr];
    }
}

#endif
