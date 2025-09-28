#include "xc.h"
#include "I2C.h"


#define ctrlByte 0b00000000
#define LCDaddy 0b0111100 // 7-bit base I2C address with SA0 line pulled low 
#define LCDaddy_writ (LCDaddy << 1) & 0b11111110 // Address + R/nW (0)
#define LCDaddy_read (LCDaddy << 1) | 0b00000001 // Address + R/nW (1)

void delay_ms(unsigned int ms) {    
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_reset(void) {
    delay_ms(5);
    PORTBbits.RB6 = 0;
    delay_ms(10);
    PORTBbits.RB6 = 1;
    delay_ms(1);
}

void lcd_cmd(char command) {
    I2C1CONbits.SEN = 1; 
    while(I2C1CONbits.SEN);
    
    _MI2C1IF = 0;
    I2C1TRN = LCDaddy_writ; //sends target address
    while(!_MI2C1IF && _TRSTAT);
    
    _MI2C1IF = 0;
    I2C1TRN = ctrlByte; //sends control byte
    while(!_MI2C1IF && _TRSTAT);
    
    _MI2C1IF = 0;
    I2C1TRN = command; //sends data byte
    while(!_MI2C1IF && _TRSTAT);
    
    I2C1CONbits.PEN = 1; //initiate stop condition
    while(I2C1CONbits.PEN); //clears when stop condition is complete
}

void lcd_init(void) {
    
    lcd_cmd(0x3A); //RE=1; REV=0
    lcd_cmd(0x09); //4 line display
    lcd_cmd(0x06); //bottome view
    lcd_cmd(0x1E); //BS1=1
    lcd_cmd(0x39); //RE=0; IS=1
    lcd_cmd(0x1B); //BS0=1 -> Bias=1/6
    lcd_cmd(0x6E); //Devider on and set value
    lcd_cmd(0x56); // Bosster on and set contrast
    lcd_cmd(0x7A); //set contrast
    lcd_cmd(0x38); //RE=0; IS=0
    lcd_cmd(0x0F); //display on, cursor on, blink on
    
    /* Function set (RE=1 version) */
    lcd_cmd(0x3a); /* DL, N, ~BE, enter extended mode RE=1, ~REV */ 
    /* Extended function set (assumes RE=1) */
    lcd_cmd(0x09); /* NW, ~FW, ~B/W */
    /* Double-height/bias/dot-shift (assumes RE=1) */
    lcd_cmd(0x1a); /* UD2, ~UD1, BS1, ~DH? */
    /* Function set (RE=0 version, IS=0) */
    lcd_cmd(0x3c); /* DL, N, DH, return to RE=0, ~IS */
    
    //clearing the display
    lcd_cmd(0x01);

}

void lcd_setCursor(char row, char col) {
    lcd_cmd(((0x20)*row+col) | 0x80);
}

void lcd_printChar(char myChar) {
    I2C1CONbits.SEN = 1; 
    while(I2C1CONbits.SEN);
    
    _MI2C1IF = 0;
    I2C1TRN = LCDaddy_writ; //sends target address
    while(!_MI2C1IF && _TRSTAT);
    
    _MI2C1IF = 0;
    I2C1TRN = 0b01000000; //sends control byte
    while(!_MI2C1IF && _TRSTAT);
    
    _MI2C1IF = 0;
    I2C1TRN = myChar; //sends data byte
    while(!_MI2C1IF && _TRSTAT);
    
    I2C1CONbits.PEN = 1; //initiate stop condition
    while(I2C1CONbits.PEN); //clears when stop condition is complete
}

void lcd_printStr(const char * str) {
    while(*str != '\0') {
        lcd_printChar(*str);
        str++;
    }
}

void lcd_setup(void){ 
 //setting up the baud rate
    I2C1CONbits.I2CEN = 0;
    I2C1BRG = 157;
    I2C1CONbits.I2CEN = 1;
    
    //ressetting LCD
    lcd_reset();
    
}



