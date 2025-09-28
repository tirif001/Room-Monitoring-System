/* 
 * File:   led.h
 * Author: eyobtirfia
 *
 * Created on April 30, 2025, 8:44 AM
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif
void write_color(int r, int g, int b);
void delay(int delay_in_ms);
void lightUpPixels(int num_on, int r, int g, int b);
void lightUpPixelsColorful(int num_on);



#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

