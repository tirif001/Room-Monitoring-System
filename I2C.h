/* 
 * File:   I2C.h
 * Author: eyobtirfia
 *
 * Created on April 30, 2025, 8:40 AM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
void lcd_setup(void);
void delay_ms(unsigned int ms);
void lcd_reset(void);
void lcd_cmd(char command);
void lcd_init(void);
void lcd_setCursor(char row, char col);
void lcd_printChar(char myChar);
void lcd_printStr(const char * str);



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

