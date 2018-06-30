#include <iostream>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "othelloV7.h"
#include "muloth.h"
#include <chrono>
#include <inttypes.h>
#include "io_helper.h"
using namespace std;
typedef unsigned long long keyT;
typedef uint8_t valueT;

vector<keyT> keys;
vector<valueT> values;


void add(char * fname) 
{
    printf("Reading file %s \n" , fname);
    FILE *pFile;
    pFile = fopen (fname,"r");
    uint64_t cnt = 0 ;
    while (true) 
    {
        char buf[1024];
        if (fgets(buf,1024,pFile)==NULL) break;
        keyT k;
        valueT v;
        if (!lineToKVpair(buf, &k , &v)) break;
        keys.push_back(k);
        values.push_back(v);
        cnt++;
    }
    fclose(pFile);
    printf("key cnt %lld; tot key %lld \n", cnt, keys.size());
}

int main(int argc, char * argv[]) {
    if (argc < 4) 
    {
        printf(" splitbits keyInputFile OutputFile .... \n");
        printf(" splitbits: a number <=16, divide the keys into 2^(splitbits) sets, according to the Least (splitbit) significant bits. \n");
        return 0;
    }
    int splitbit;
    sscanf(argv[1],"%d",&splitbit);
    MulOth<keyT,valueT> * moth;
    printf("Split %d groups\n",1U<< splitbit);
    moth = new MulOth<keyT,valueT>(argv[2],  splitbit);   
    printf("Build Succ, write to file %s\n", argv[3]);
    //moth.printall();
    moth->writeToFile(argv[3]);
    delete moth;
    return 0;
}
