#include "xc.h"
#include "asm.h"
#include <stdlib.h>
#define PERIOD 5

void setup(void) {
    AD1PCFG = 0x9FFF;
    TRISA = 0x0000;
    LATA = 0x0000;
    CLKDIVbits.RCDIV = 0;
}



void write_color(int r, int g, int b) {
    int i;
    for (i = 7; i >= 0; i--) (g & (1 << i)) ? write_1() : write_0(); // Send Green color bits one by one to the NeoPixel
    for (i = 7; i >= 0; i--) (r & (1 << i)) ? write_1() : write_0(); // Red    
    for (i = 7; i >= 0; i--) (b & (1 << i)) ? write_1() : write_0(); // Blue
}



void delay(int delay_in_ms) {
    for (int i = 0; i < delay_in_ms; i++) {
        wait_1ms();
    }
}



void lightUpPixels(int num_on, int r, int g, int b) {
    for (int i = 0; i < 8; i++) {
        if (i < num_on) // Light up certain number of LED segments with  given color 
            write_color(r, g, b);
        else
            write_color(0, 0, 0);
    }
    wait_100us();
}



void lightUpPixelsColorful(int num_on) {
    for (int i = 0; i < 8; i++) {
        if (i < num_on) {
            if (i >= 5) { 
                write_color(255, 0, 0); // Light top LEDs red
            } else if (i >= 2) {
                write_color(255, 255, 0); // Yellow
            } else {
                write_color(0, 255, 0); // Green
            }
        } else {
            write_color(0, 0, 0); 
        }
    }
    wait_100us();
}

