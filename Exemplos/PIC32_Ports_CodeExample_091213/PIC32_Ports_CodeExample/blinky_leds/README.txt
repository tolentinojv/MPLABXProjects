	Readme File for Code Example:
--------------------------------------------------

PIC32_Ports\blinky_leds

--------------------------------------------------
Code Example Description:
--------------------------------------------------
The purpose of this example code is to demonstrate the PIC32MX peripheral library macros and functions supporting IOPORT module and its various features.

This program blinks the LEDs on the Explorer-16 demo board.

--------------------------------------------------
Suggested Development Resources:
--------------------------------------------------
Explorer-16 demo board with 

Processors:	PIC32MX###F512L
		       360
		       460
		       795
		PIC32MX430F064L 
		PIC32MX450F256L 
		PIC32MX470F512L
            	PIC32MX220F032D (LEDs must be soldered to the PIM)
		PIC32MX250F128D (LEDs must be soldered to the PIM)
		

Complier:	MPLAB C32 v1 or higher
		MPLAB XC32 

IDE:		MPLAB IDE v8 or higher
            	MPLAB X IDE

--------------------------------------------------
Notes:
--------------------------------------------------
For PIC32MX 360/460/795/430 PIMS, Explorer-16 LEDs are on lower 8-bits of PORTA and to use all LEDs, JTAG port must be disabled.

For PIC32MX 220/250 PIMS, Explorer-16 LEDs are unconnected and must be soldered to the test points on top of the PIM as stated below.

--------------------------------------------------
Soldering Instructions for PIC32MX 220/250 PIMS:
--------------------------------------------------
The test points (TP) are located on top of the PIM and must be connected as follows to the Explorer-16 for proper LED functionality.

TP19 to D3  (RA0 to LED0)
TP20 to D4  (RA1 to LED1)	
TP30 to D5  (RA2 to LED2)
TP31 to D6  (RA3 to LED3)

