#include "xc.h"
#include "assert.h"
#include "circularBuffer.h"
#define BUFFER_SIZE 1024
#define SAMPLE_SIZE 128

volatile int sampleToDelete, writeIdx=0, numElemsInBuff=0;
volatile unsigned long int buffer[BUFFER_SIZE] = {0};


void initBuffer(void) {
    writeIdx = SAMPLE_SIZE;
    numElemsInBuff = 0;
    sampleToDelete = 0;
}

void putVal(int newValue) {
    if (numElemsInBuff < BUFFER_SIZE){
        numElemsInBuff++;
    }
        buffer[writeIdx++] = newValue;
        writeIdx %= BUFFER_SIZE;
        sampleToDelete = (sampleToDelete + 1) % BUFFER_SIZE;
        
}

 
 int getAvg(void) {
    
     unsigned long int sum  = 0;
     for(int i = 0; i < BUFFER_SIZE; i++ ) {//originally looped to SAMPLE_SIZE
         //sum += buffer[(sampleToDelete+i)% BUFFER_SIZE];
         sum += buffer[i];
     }
     return sum / SAMPLE_SIZE;
 }

