#include "vwrite.hpp"
#if nVO > 0

CWrite::CWrite()
{
}
CWrite::CWrite(int versat_base, int base, versat_t *databus)
{
    this->base = base;
    this->versat_base = versat_base;
    this->databus = databus;
}
CWrite::CWrite(int versat_base, int base)
{
    this->base = base;
    this->versat_base = versat_base;
}

void CWrite::setExtAddr(int extAddr)
{
    this->extAddr = extAddr;
}
#if YOLO_VERSAT == 1
void CWrite::setOffset(int offset)
{
    this->offset = offset;
}
#endif
void CWrite::setIntAddr(int intAddr)
{
    this->intAddr = intAddr;
}
void CWrite::setExtIter(int iter)
{
    this->extIter = iter;
}
void CWrite::setExtPer(int per)
{
    this->extPer = per;
}
void CWrite::setExtShift(int shift)
{
    this->extShift = shift;
}
void CWrite::setExtIncr(int incr)
{
    this->extIncr = incr;
}
void CWrite::setIntStart(int start)
{
    this->intStart = start;
}
void CWrite::setIntIter(int iter)
{
    this->intIter = iter;
}
void CWrite::setIntPer(int per)
{
    this->intPer = per;
}
void CWrite::setIntShift(int shift)
{
    this->intShift = shift;
}
void CWrite::setIntIncr(int incr)
{
    this->intIncr = incr;
}
void CWrite::setIntIter2(int iter)
{
    this->intIter2 = iter;
}
void CWrite::setIntPer2(int per)
{
    this->intPer2 = per;
}
void CWrite::setIntShift2(int shift)
{
    this->intShift2 = shift;
}
void CWrite::setIntIncr2(int incr)
{
    this->intIncr2 = incr;
}
void CWrite::setSel(int sel)
{
    this->sel = sel;
}

void CWrite::setDuty(int duty)
{
    this->duty = duty;
}
void CWrite::setDelay(int delay)
{
    this->delay = delay;
}

void CWrite::copy(CWrite that)
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
    this->sel = that.sel;
    this->duty = that.duty;
    this->delay = that.delay;
}
void CWrite::copy_ext(CWrite that)
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
string CWrite::info()
{
    string ver = "vwrite[" + to_string(base) + "]\n";
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
    ver += "next_half=        " + to_string(next_half) + "\n";
    ver += "Sel=        " + to_string(sel) + "\n";
    ver += "Duty=        " + to_string(duty) + "\n";

    ver += "\n";
    return ver;
}
string CWrite::info_iter()
{
    string ver = "CWrite[" + to_string(base) + "]\n" + "VO MEM DATA[\n";
    for (int i = 0; i < MEM_SIZE; i++)
    {
        ver += "mem[" + to_string(i) + "]=" + to_string(mem[i]) + "\n";
    }
    ver += "]\n";
    ver += "duty_cnt=    " + to_string(duty_cnt) + "\n";
    ver += "Addr=    " + to_string(aux) + "\n";
    ver += "OUTPUT_BUFFER (LATENCY SIM)\n";
    for (int z = 0; z < MEMP_LAT; z++)
    {
        ver += "Output[" + to_string(z) + "]=" + to_string(output_port[z]) + "\n";
    }
    return ver;
}

void CWrite::start_run()
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

    //if (extPer != 0 || extIter != 0)
    getdatatoDDR();

    if ((intIter != 0 || intPer != 0) && (extPer != 0 || extIter != 0))
    {
        next_half = !next_half;
    }
}

void CWrite::update()
{
    if (run_delay > 0)
    {
        run_delay--;
    }
}

void CWrite::getdatatoDDR()
{
    addrgen2loop();
}

versat_t CWrite::AGU()
{
    uint32_t aux_acc = 0;
    if (done == 0)
        aux_acc = addrgen4loop();
    if (done == 1)
    {
        pos2 = intStart;
        pos = intStart;
    }

    return aux_acc;
}

versat_t CWrite::addrgen2loop()
{
    int addr = extAddr / (DATAPATH_W / 8);
    int iaddr = intAddr;
    for (int i = 0; i < extIter; i++)
    {
        for (int j = 0; j < extPer; j++)
        {
            if (next_half == true)
            {
                FPGA_mem[addr] = mem[iaddr + MEM_SIZE / 2];
            }
            else
            {
                FPGA_mem[addr] = mem[iaddr];
            }
            addr += extIncr;
            iaddr += 1;
        }
        addr += extShift;
    }
    return 0;
}

versat_t CWrite::addrgen4loop()
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

versat_t CWrite::output()
{
    if (run_delay > 0)
        return 0;

    out = AGU();
    if (enable == 1)
        write(out, databus[sel]);
    return 0;
}
extern versat_t *FPGA_mem;

void CWrite::write(int addr, versat_t data)
{
    if (next_half == false)
        mem[addr] = data;
    else
    {
        mem[addr + MEM_SIZE / 2] = data;
    }
}

#endif