/*
 * File:   main.c
 * Author: eyobtirfia
 *
 * Created on April 30, 2025, 8:35 AM
 */
#include "xc.h"
#include "stdio.h"
#include "circularBuffer.h"
#include "I2C.h"
#include "led.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

unsigned int maxSignal = 0;
unsigned int minSignal = 1024;
unsigned int sum = 0;
unsigned int count = 0;




void setup_ADC(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;            //sets all pins to digital I/O
    TRISA = 0b0000000000011111;  //set port A to inputs, 
    TRISB = 0b0000000000000011;  //and port B to outputs
    
    
    //Setting up timer 3
    T3CON = 0; 
    T3CONbits.TCKPS = 0b11; // Sets pre-scaler of 256
    TMR3 = 0;     // Initialize to zero (also best practice)
    PR3 = 62499 / 44100; // samples ADC 31250 times a second
    
    T3CONbits.TON = 1;
    
    AD1PCFGbits.PCFG0 = 0;
    
    //Setting up ADC  
    AD1CON1bits.FORM = 0b00; //integer format
    AD1CON1bits.SSRC = 0b010; //uses timer 3 as conversion trigger
    AD1CON1bits.ASAM = 1;
    //setting A/D Control Register 2
    AD1CON2 = 0; 
    //setting A/D Control Register 3
    AD1CON3 = 0;
    AD1CON3bits.SAMC = 1;//setting up sampling time
    AD1CON3bits.ADCS = 1;// Conversion Clock period
    //turning on A/D
    AD1CON1bits.ADON = 1;
    
    _AD1IF = 0;
    _AD1IE = 1;
    
    
}

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt() {
    IFS0bits.AD1IF = 0; //clear ADC interrupt flag
    //putVal(ADC1BUF0); //put value to the buffer
    unsigned int sample = ADC1BUF0;
    if (sample < 1024)  // toss out spurious readings
      {
         if (sample > maxSignal)
         {
            maxSignal = sample;  // save just the max levels
         }
         else if (sample < minSignal)
         {
            minSignal = sample;  // save just the min levels
         }
      }
    
}

void timerTo50ms(void) {
    //needs to count to exactly 32.768ms
    T1CON = 0; //set up timer one to count to 32 ms
    T1CONbits.TCKPS = 0b10;// 64 pre-scaler
    TMR1 = 0;
    PR1 = 12499;   
    
    _T1IF = 0;
    _T1IE = 1;
    
    T1CONbits.TON = 1;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt() {
  
    //does the conversion from ADC to the LCD
    IFS0bits.T1IF = 0;
    sum += maxSignal - minSignal;
    count++;
    maxSignal = 0;       // Reset for next period
    minSignal = 1024;    // Reset for next period
    
}


void timerTo100ms(void) {
    //needs to count to exactly 500ms
    T2CON = 0; //set up timer one to count to 32 ms
    T2CONbits.TCKPS = 0b11;// 256 pre-scaler
    TMR2 = 0;
    PR2 = 4249;   
    
    _T2IF = 0;
    _T2IE = 1;
    
    T2CONbits.TON = 1;
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt() {

    IFS0bits.T2IF = 0;
    
    if (count > 0) {  // Prevent division by zero
        unsigned int adValue = sum / count;
//        unsigned int soundLevel = (1000 * adValue) / 1024;
        unsigned int soundLevel = adValue;
        
        lightUpPixelsColorful(soundLevel/100);
        
        char adStr[20];
        sprintf(adStr, "Level: %d", soundLevel);
        
        sum = 0;
        count = 0;
    }
}


int main(void){
    setup();
    setup_ADC();
    initBuffer();
    lcd_setup();
    lcd_init();
    
    timerTo50ms();
    timerTo100ms();
    while(1) {
        //write to the led
        
    }
    
}
