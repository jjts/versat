#include "versat.hpp"
#include <stdio.h>
#include <fstream>

#if INFO == 1
#if nMEM > 0
void print_versat_mems()
{
    FILE *f = fopen("versat_mem.hex", "w");
    for (int j = 0; j < nSTAGE; j++)
    {
        for (int l = 0; l < nMEM; l++)
        {
            fprintf(f, "\nnStage=%d \nnMEM=%d\n\n", j, l);
            for (int i = 0; i < MEM_SIZE; i++)
            {
                fprintf(f, "%d\n", stage[j].memA[l].read(i));
            }
        }
    }
    fclose(f);
}
#else
void print_versat_mems() {}
#endif
void print_versat_info()
{
    ofstream inf;
    inf.open("versat_info.txt");
    for (int i = 0; i < nSTAGE; i++)
    {
        inf << shadow_reg[i].info();
    }
    inf.close();
}

ofstream inff;
int auxx = 0;
void print_versat_iter()
{
    string ola;
    static int i = 0;

    printf("PRINTING ITER %d \n", versat_iter);
    if (i == 0 && versat_iter == 0)
    {
        ola = "./iter/versat_iter" + to_string(versat_iter) + ".txt";
        inff.open(ola);
    }
    if (i != 0 && versat_iter != 0)
    {
        inff.close();
        ola = "./iter/versat_iter" + to_string(versat_iter) + ".txt";
        inff.open(ola);
    }
    i++;
    string ver = "\nIteration " + to_string(versat_iter) + "\n";
    for (int i = 0; i < nSTAGE; i++)
    {
        ver += shadow_reg[i].info_iter();
        ver += "DATABUS  \n";
        for (int j = 0; j < 2 * (1 << (N_W - 1)); j++)
        {
            ver += "databus[" + to_string(j) + "]=" + to_string(stage[i].databus[j]) + "\n";
        }
    }
    inff << ver;
    //inf.close();
}
#endif