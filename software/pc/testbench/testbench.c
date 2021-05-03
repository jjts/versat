//import custom libraries

#include "versat.hpp"

//import c libraries
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#if nVI > 0 && nVO > 0
// Use Xreads and xwrites instead of nMEM
#define IO_VERSAT 1
extern versat_t *FPGA_mem;
#else
#define IO_VERSAT 0
#endif

//define peripheral base addresses
#define VERSAT 0
#if IO_VERSAT == 0
int main(int argc, char **argv)
{
    //local variables
    int i, j, k, l, m;
    int16_t pixels[25 * nSTAGE], weights[9 * nSTAGE], bias = 0, res;
    clock_t start, end;

    //send init message
    printf("\nVERSAT TEST \n\n");

    //init VERSAT
    start = clock();
    versat_init(VERSAT);
    end = clock();
    printf("Deep versat initialized in %ld us\n", (end - start));

    //write data in versat mems
    start = clock();
    for (j = 0; j < nSTAGE; j++)
    {

        //write 5x5 feature map in mem0
        for (i = 0; i < 25; i++)
        {
            pixels[25 * j + i] = rand() % 50 - 25;
            stage[j].memA[0].write(i, pixels[25 * j + i]);
        }

        //write 3x3 kernel and bias in mem1
        for (i = 0; i < 9; i++)
        {
            weights[9 * j + i] = rand() % 10 - 5;
            stage[j].memA[1].write(i, weights[9 * j + i]);
        }

        //write bias after weights of VERSAT 0
        if (j == 0)
        {
            bias = rand() % 20 - 10;
            stage[j].memA[1].write(9, bias);
        }
    }
    end = clock();
    printf("\nData stored in versat mems in %ld us\n", (end - start));
    //expected result of 3D convolution
    printf("\nExpected result of 3D convolution\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            res = bias;
            for (k = 0; k < nSTAGE; k++)
            {
                for (l = 0; l < 3; l++)
                {
                    for (m = 0; m < 3; m++)
                    {
                        res += pixels[i * 5 + j + k * 25 + l * 5 + m] * weights[9 * k + l * 3 + m];
                    }
                }
            }
            printf("%d\t", res);
        }
        printf("\n");
    }
    /////////////////////////////////////////////////////////////////////////////////
    // 3D CONVOLUTION WITH 2-LOOP ADDRGEN
    /////////////////////////////////////////////////////////////////////////////////

    printf("\n3D CONVOLUTION WITH 2-LOOP ADDRGEN\n");

    //loop to configure versat stages
    int delay = 0, in_1_alulite = sMEMA[1];
    start = clock();
    for (i = 0; i < nSTAGE; i++)
    {

        //configure mem0A to read 3x3 block from feature map
        stage[i].memA[0].setIter(3);
        stage[i].memA[0].setIncr(1);
        stage[i].memA[0].setDelay(delay);
        stage[i].memA[0].setPer(3);
        stage[i].memA[0].setDuty(3);
        stage[i].memA[0].setShift(5 - 3);

        //configure mem1A to read kernel
        stage[i].memA[1].setIter(1);
        stage[i].memA[1].setIncr(1);
        stage[i].memA[1].setDelay(delay);
        stage[i].memA[1].setPer(10);
        stage[i].memA[1].setDuty(10);

        //configure muladd0
        stage[i].muladd[0].setSelA(sMEMA[0]);
        stage[i].muladd[0].setSelB(sMEMA[1]);
        stage[i].muladd[0].setFNS(MULADD_MACC);
        stage[i].muladd[0].setIter(1);
        stage[i].muladd[0].setPer(9);
        stage[i].muladd[0].setDelay(MEMP_LAT + delay);

        //configure ALULite0 to add bias to muladd result
        stage[i].alulite[0].setOpA(in_1_alulite);
        stage[i].alulite[0].setOpB(sMULADD[0]);
        stage[i].alulite[0].setFNS(ALULITE_ADD);

        //update variables
        if (i == 0)
            in_1_alulite = sALULITE_p[0];
        if (i != nSTAGE - 1)
            delay += 2;
    }

    //config mem2A to store ALULite output
    stage[nSTAGE - 1].memA[2].setIter(1);
    stage[nSTAGE - 1].memA[2].setIncr(1);
    stage[nSTAGE - 1].memA[2].setDelay(MEMP_LAT + 8 + MULADD_LAT + ALULITE_LAT + delay);
    stage[nSTAGE - 1].memA[2].setPer(1);
    stage[nSTAGE - 1].memA[2].setDuty(1);
    stage[nSTAGE - 1].memA[2].setSel(sALULITE[0]);
    stage[nSTAGE - 1].memA[2].setInWr(1);
    end = clock();
    printf("\nConfigurations (except start) made in %ld us\n", (end - start));
    printf("\nExpected Versat Clock Cycles for this run %d\n", MEMP_LAT + 8 + MULADD_LAT + ALULITE_LAT + delay + 1);

    //perform convolution
    start = clock();
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {

            //configure start values of memories
            for (k = 0; k < nSTAGE; k++)
                stage[k].memA[0].setStart(i * 5 + j);
            stage[nSTAGE - 1].memA[2].setStart(i * 3 + j);

            //run configurations
            run();

            //wait until done is done
            while (done() == 0)
                ;
        }
    }
    end = clock();
    printf("\n3D convolution done in %ld us\n", (end - start));
    printf("Simulation took %d Versat Clock Cycles\n", versat_iter);
    //display results
    printf("\nActual convolution result\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
            printf("%d\t", (int16_t)stage[nSTAGE - 1].memA[2].read(i * 3 + j));
        printf("\n");
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 3D CONVOLUTION WITH 4-LOOP ADDRGEN
    /////////////////////////////////////////////////////////////////////////////////

    printf("\n3D CONVOLUTION WITH 4-LOOP ADDRGEN\n");

    //loop to configure versat stages
    delay = 0, in_1_alulite = sMEMB[1];
    start = clock();

    //configure mem1B to read bias
    stage[0].memB[1].setStart(9);
    stage[0].memB[1].setIter(9);
    stage[0].memB[1].setPer(9);
    stage[0].memB[1].setDuty(9);

    for (i = 0; i < nSTAGE; i++)
    {

        //configure mem0A to read all 3x3 blocks from feature map
        stage[i].memA[0].setIter2(3);
        stage[i].memA[0].setPer2(3);
        stage[i].memA[0].setShift2(5 - 3);
        stage[i].memA[0].setIncr2(1);
        stage[i].memA[0].setStart(0);

        //configure mem1A to read kernel
        stage[i].memA[1].setIter(9);
        stage[i].memA[1].setIncr(1);
        stage[i].memA[1].setDelay(delay);
        stage[i].memA[1].setPer(9);
        stage[i].memA[1].setDuty(9);
        stage[i].memA[1].setShift(-9);

        //configure muladd0
        stage[i].muladd[0].setIter(9);

        //configure ALULite0 to add bias to muladd result
        stage[i].alulite[0].setOpA(in_1_alulite);

        //update variables
        if (i == 0)
            in_1_alulite = sALULITE_p[0];
        if (i != nSTAGE - 1)
            delay += 2;
    }

    //config mem2A to store ALULite output
    //start, iter, incr, delay, per, duty, sel, shift, in_wr
    stage[nSTAGE - 1].memA[2].setStart(10);
    stage[nSTAGE - 1].memA[2].setIter(9);
    stage[nSTAGE - 1].memA[2].setIncr(1);
    stage[nSTAGE - 1].memA[2].setDelay(MEMP_LAT + 8 + MULADD_LAT + ALULITE_LAT + delay);
    stage[nSTAGE - 1].memA[2].setPer(9);
    stage[nSTAGE - 1].memA[2].setDuty(1);
    stage[nSTAGE - 1].memA[2].setSel(sALULITE[0]);
    stage[nSTAGE - 1].memA[2].setInWr(1);
    end = clock();
    printf("\nConfigurations (except start) made in %ld us\n", (end - start));
    printf("\nExpected Versat Clock Cycles for this run %d\n", MEMP_LAT + 8 + MULADD_LAT + ALULITE_LAT + delay + 9 * 9);
    // Expected Versat Clock Cycles = Mem.Delay+Mem.Iter2*Mem.Per2*Mem.Iter*Mem.Per where Mem is the Mem where final results are written on
    //perform convolution
    start = clock();
    run();
    while (done() == 0)
        ;
    end = clock();
    printf("\n3D convolution done in %ld us\n", (end - start));
    printf("Simulation took %d Versat Clock Cycles\n", versat_iter);

    //display results
    printf("\nActual convolution result\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
            printf("%d\t", (int16_t)stage[nSTAGE - 1].memA[2].read(i * 3 + j + 10));
        printf("\n");
    }
    print_versat_info();
    //clear conf_reg of VERSAT 0
    stage[0].clearConf();

#ifdef CONF_MEM_USE
    //store conf_reg of VERSAT 1 in conf_mem (addr 0)
    stage[1].confMemWrite(0);
#endif

    //global conf clear
    globalClearConf();

#ifdef CONF_MEM_USE
    //store conf_mem (addr 0) in conf_reg of VERSAT2
    stage[1].confMemRead(0);
#endif

    //return data
    printf("\n");
    return 0;
}
#endif
int main(int argc, char **argv)
{
    //local variables
    int i, j, k, l, m;
    int addr = 0;
    int16_t pixels[25 * nSTAGE], weights[9 * nSTAGE], bias = 0, res;
    clock_t start, end;

    //send init message
    printf("\nVERSAT TEST \n\n");

    //init VERSAT
    start = clock();
    versat_init(VERSAT);
    end = clock();
    printf("Deep versat initialized in %ld us\n", (end - start));

    //write data in versat mems
    start = clock();
    for (j = 0; j < nSTAGE; j++)
    {

        //write 5x5 feature map in xread0
        stage[j].vi[0].setExtAddr(addr * (DATAPATH_W / 8));
        stage[j].vi[0].setIntAddr(0);
        stage[j].vi[0].setExtPer(25);
        stage[j].vi[0].setExtIter(1);
        stage[j].vi[0].setExtIncr(1);
        stage[j].vi[0].setExtShift(0);
        for (i = 0; i < 25; i++)
        {
            pixels[25 * j + i] = rand() % 50 - 25;
            FPGA_mem[addr] = pixels[25 * j + i];
            addr++;
        }
        stage[j].vi[1].setExtAddr(addr * (DATAPATH_W / 8));
        stage[j].vi[1].setIntAddr(0);
        stage[j].vi[1].setExtPer(9);
        stage[j].vi[1].setExtIter(1);
        stage[j].vi[1].setExtIncr(1);
        stage[j].vi[1].setExtShift(0);
        //write 3x3 kernel and bias in xread1
        for (i = 0; i < 9; i++)
        {
            weights[9 * j + i] = rand() % 10 - 5;
            FPGA_mem[addr] = weights[9 * j + i];
            addr++;
        }

        //write bias after weights of VERSAT 0
        if (j == 0)
        {
            bias = rand() % 20 - 10;
            FPGA_mem[addr] = bias;
            stage[j].vi[2].setExtAddr(addr * (DATAPATH_W / 8));
            stage[j].vi[2].setIntAddr(0);
            stage[j].vi[2].setExtPer(1);
            stage[j].vi[2].setExtIter(1);
            stage[j].vi[2].setExtIncr(1);
            stage[j].vi[2].setExtShift(0);
            addr++;
        }
    }

    end = clock();
    printf("\nData stored in versat mems in %ld us\n", (end - start));
    //expected result of 3D convolution
    printf("\nExpected result of 3D convolution\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            res = bias;
            for (k = 0; k < nSTAGE; k++)
            {
                for (l = 0; l < 3; l++)
                {
                    for (m = 0; m < 3; m++)
                    {
                        res += pixels[i * 5 + j + k * 25 + l * 5 + m] * weights[9 * k + l * 3 + m];
                    }
                }
            }
            printf("%d\t", res);
        }
        printf("\n");
    }

    printf("\n3D CONVOLUTION WITH 4-LOOP ADDRGEN\n");

    //loop to configure versat stages
    int delay = 0, in_1_alulite = sVI[2];
    start = clock();

    //configure mem1B to read bias
    stage[0].vi[2].setIntStart(0);
    stage[0].vi[2].setIntIter(9);
    stage[0].vi[2].setIntPer(9);
    stage[0].vi[2].setDuty(9);

    for (i = 0; i < nSTAGE; i++)
    {

        //configure mem0A to read all 3x3 blocks from feature map
        stage[i].vi[0].setIntIter2(3);
        stage[i].vi[0].setIntPer2(3);
        stage[i].vi[0].setIntShift2(5 - 3);
        stage[i].vi[0].setIntIncr2(1);
        stage[i].vi[0].setIntStart(0);
        stage[i].vi[0].setIntIter(3);
        stage[i].vi[0].setIntIncr(1);
        stage[i].vi[0].setDelay(delay);
        stage[i].vi[0].setIntPer(3);
        stage[i].vi[0].setDuty(3);
        stage[i].vi[0].setIntShift(5 - 3);

        //configure mem1A to read kernel
        stage[i].vi[1].setIntIter(9);
        stage[i].vi[1].setIntIncr(1);
        stage[i].vi[1].setDelay(delay);
        stage[i].vi[1].setIntPer(9);
        stage[i].vi[1].setDuty(9);
        stage[i].vi[1].setIntShift(-9);

        //configure muladd0
        stage[i].muladd[0].setSelA(sVI[0]);
        stage[i].muladd[0].setSelB(sVI[1]);
        stage[i].muladd[0].setFNS(MULADD_MACC);
        stage[i].muladd[0].setPer(9);
        stage[i].muladd[0].setDelay(MEMP_LAT + delay);
        stage[i].muladd[0].setIter(9);

        //configure ALULite0 to add bias to muladd result
        stage[i].alulite[0].setOpB(sMULADD[0]);
        stage[i].alulite[0].setFNS(ALULITE_ADD);
        stage[i].alulite[0].setOpA(in_1_alulite);

        //update variables
        if (i == 0)
            in_1_alulite = sALULITE_p[0];
        if (i != nSTAGE - 1)
            delay += 2;
    }

    //config mem2A to store ALULite output
    //start, iter, incr, delay, per, duty, sel, shift, in_wr
    stage[nSTAGE - 1].vo[0].setIntStart(0);
    stage[nSTAGE - 1].vo[0].setIntIter(9);
    stage[nSTAGE - 1].vo[0].setIntIncr(1);
    stage[nSTAGE - 1].vo[0].setDelay(MEMP_LAT + 8 + MULADD_LAT + ALULITE_LAT + delay);
    stage[nSTAGE - 1].vo[0].setIntPer(9);
    stage[nSTAGE - 1].vo[0].setDuty(1);
    stage[nSTAGE - 1].vo[0].setSel(sALULITE[0]);
    stage[nSTAGE - 1].vo[0].setExtAddr(addr * (DATAPATH_W / 8));
    stage[nSTAGE - 1].vo[0].setExtPer(9);
    stage[nSTAGE - 1].vo[0].setExtIter(1);
    stage[nSTAGE - 1].vo[0].setExtIncr(1);
    stage[nSTAGE - 1].vo[0].setExtShift(0);
    stage[nSTAGE - 1].vo[0].setIntAddr(0);

    end = clock();
    printf("\nConfigurations (except start) made in %ld us\n", (end - start));
    printf("\nExpected Versat Clock Cycles for this run %d\n", MEMP_LAT + 8 + MULADD_LAT + ALULITE_LAT + delay + 9 * 9);
    // Expected Versat Clock Cycles = Mem.Delay+Mem.Iter2*Mem.Per2*Mem.Iter*Mem.Per where Mem is the Mem where final results are written on
    //perform convolution
    start = clock();
    run();
    while (done() == 0)
        ;
    //print_versat_info();
    globalClearConf();

    run();
    while (done() == 0)
        ;

    run();
    //print_versat_info();

    while (done() == 0)
        ;

    end = clock();
    //print_versat_info();
    printf("\n3D convolution done in %ld us\n", (end - start));
    printf("Simulation took %d Versat Clock Cycles\n", versat_iter);

    //display results
    printf("\nActual convolution result\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
            printf("%d\t", (int16_t)FPGA_mem[addr + i * 3 + j]);
        printf("\n");
    }
    //clear conf_reg of VERSAT 0
    stage[0].clearConf();
    delete[] FPGA_mem;
}
