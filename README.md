# Room-Monitoring-System
This project is an **audio visualizer** built with a PIC24 microcontroller. It samples microphone input, processes sound levels in real time, and drives a WS2812B LED strip with colorful patterns. An I²C LCD is included for live display of the measured sound and room temperature level.  

---

## Features
- **Real-time audio sampling** using ADC with interrupt-driven design  
- **Signal processing** with max/min envelope detection and a circular buffer  
- **WS2812B LED control** via precise assembly timing routines  
- **I²C LCD driver** for live text output  
- **Timers and interrupts** for 50 ms and 100 ms periodic updates  

---

## Hardware
- **Microcontroller:** PIC24 (tested on PIC24FJ series)  
- **Input:** Electret microphone + amplifier stage  
- **Output:** WS2812B NeoPixel LED strip (8 LEDs, configurable)  
- **Display:** I²C LCD (DOGS104-A or similar)  
- **Programmer/Debugger:** PICkit 3/4  

---

Core files:
- `main.c` – system setup, ADC sampling, timers, interrupts  
- `led.c/h` – WS2812B LED control  
- `I2C.c/h` – LCD driver via I²C  
- `circularBuffer.c/h` – buffer for sample storage and averaging  
- `asm.s/h` – assembly routines for precise LED timing  

---

## How It Works
1. **ADC interrupt** samples the microphone input and tracks min/max per frame.  
2. **Timer interrupts** compute the sound envelope every 50 ms and update the LED strip every 100 ms.  
3. **LED strip** lights up in green → yellow → red to show increasing sound level.  
4. **LCD display** prints the current sound level as text.  

---

## Build & Flash
1. Open the project in **MPLAB X**.  
2. Select the correct PIC24 device and **XC16 compiler**.  
3. Build with `Makefile` or MPLAB GUI.  
4. Flash using a PICkit 3/4.  

---
