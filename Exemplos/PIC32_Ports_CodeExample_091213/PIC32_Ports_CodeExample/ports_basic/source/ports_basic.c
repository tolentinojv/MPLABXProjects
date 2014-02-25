/*********************************************************************
 *
 *      PIC32MX PORTS API Example
 *
 *********************************************************************
 * FileName:        ports_basic.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32MX
 *
 * Complier:        MPLAB XC32
 *                  MPLAB-X IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *********************************************************************
 * $Id: ports_basic.c 9390 2008-06-16 23:43:04Z rajbhartin $
 *********************************************************************
 * The purpose of this example code is to demonstrate the PIC32MX
 * peripheral library macros and functions supporting the IOPORT
 * module and its various features.
 *
 * Platform: Explorer-16 with PIC32MX PIM
 *
 * Features demonstrated:
 *    - Port pin configurations
 *    - Output Pin toggle
 *    - Interrupt on Change Notice
 *    - Weak pullups
 *    - Interrupt configuration
 *    - ISR syntax
 *
 * Description:
 *	When SW4 (the right most switch on Explorer-16) is pressed,
 *      it causes a change notice interrupt, blinking the lower four
 *      LEDs on the Explorer-16.
 *      
 * Notes:
 *    - PIC32MX 2xx PIMS are unconnected to the Explorer-16 LEDs and 
 *      switches. They must be soldered to the test points on top of
 *      the PIM for proper functionality. The README file contains a
 *      list of the connections that need to be made.
 *    - To clear a mismatch condition, always read the port first,
 *      then clear the change notice interrupt flag.
 *    - No resistor pullups are provided for SW4. Internal pullups
 *      should be enabled.
 ********************************************************************/
#include <plib.h>

#if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__) || (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 80 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#define SYS_FREQ (80000000L)

#elif defined (__32MX220F032D__) || (__32MX250F128D__)
// Configuration Bit settings
// SYSCLK = 48 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 48 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_24, FPLLIDIV = DIV_2, FPLLODIV = DIV_2, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#define SYS_FREQ (48000000L)
#endif

// SW4 is connected to PORTD, Bit 13
// which uses change notification input CN19
#if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__)
#define CONFIG          (CN_ON | CN_IDLE_CON)
#define PINS            (CN19_ENABLE)
#define PULLUPS         (CN19_PULLUP_ENABLE)

// SW4 is connected to PORTD, Bit 13
// On these series of devices the change notification inputs
// are set by the specific pin name
#elif defined (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
#define CONFIG         (CND_ON | CND_IDLE_CON)
#define PINS           (CND13_ENABLE)
#define PULLUPS        (CND13_PULLUP_ENABLE)

// SW4 is connected to PORTC, Bit 0 (by soldering)
// On these series of devices the change notification inputs
// are set by the specific pin name
#elif defined (__32MX220F032D__) || (__32MX250F128D__)
#define CONFIG         (CNC_ON | CNC_IDLE_CON)
#define PINS           (CNC0_ENABLE)
#define PULLUPS        (CNC0_PULLUP_ENABLE)
#endif

int main(void)
{
    unsigned int temp;

    // Configure cache, wait states and peripheral bus clock
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // Explorer-16 LEDs are on lower 8-bits of PORTA and to use all LEDs, JTAG port must be disabled.
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    // Clear PORTA bits so there are no unexpected flashes when setting
    // them to output in the next step
    mPORTAClearBits(BIT_3 | BIT_2 | BIT_1 | BIT_0);

    // Set lower four bits of PORTA as output
    mPORTASetPinsDigitalOut(BIT_3 | BIT_2 | BIT_1 | BIT_0);

    #if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__)|| (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    // Set RD13 (switch S4, the right-most switch on Explorer 16) as input
    mPORTDSetPinsDigitalIn(BIT_13);
    #elif defined (__32MX220F032D__) || (__32MX250F128D__)
    // Set lowest bit of PORTC to input (for the switch)
    mPORTCSetPinsDigitalIn(BIT_0);
    #endif

    // Note: It is recommended to disable vector interrupts prior to
    // configuring the change notice module, if they are enabled.
    // The user must read one or more IOPORTs to clear the IO pin
    // change notice mismatch condition, then clear the change notice
    // interrupt flag before re-enabling the vector interrupts.

    // Enable change notice, enable discrete pins and weak pullups
    #if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__)
    mCNOpen(CONFIG, PINS, PULLUPS);
    #elif defined (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    mCNDOpen(CONFIG, PINS, PULLUPS);
    #elif defined (__32MX220F032D__) || (__32MX250F128D__)
    mCNCOpen(CONFIG, PINS, PULLUPS);
    #endif

    // Read port to clear mismatch on change notice pins
    #if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__) || (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    temp = mPORTDRead();
    #elif defined (__32MX220F032D__) || (__32MX250F128D__)
    temp = mPORTCRead();
    #endif

    // Enable change notice interrupt
    #if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__)
    INTEnable(INT_CN, INT_ENABLED);
    #elif defined (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    INTEnable(INT_CND, INT_ENABLED);
    #elif defined (__32MX220F032D__) || (__32MX250F128D__)
    INTEnable(INT_CNC, INT_ENABLED);
    #endif

    // Set priority levels
    INTSetVectorPriority(INT_CHANGE_NOTICE_VECTOR, INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_CHANGE_NOTICE_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    // Enable multi-vector interrupts
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    while (1);
}

// Configure the CN interrupt handler
void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void)
{
    unsigned int temp;

    // Read port to clear mismatch on change notice pins
    #if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__) || (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    temp = mPORTDRead();
    #elif defined (__32MX220F032D__) || (__32MX250F128D__)
    temp = mPORTCRead();
    #endif

    // Clear the interrupt flag
    #if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__)
    INTClearFlag(INT_CN);
    #elif defined (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    INTClearFlag(INT_CND);
    #elif defined (__32MX220F032D__) || (__32MX250F128D__)
    INTClearFlag(INT_CNC);
    #endif

    // Toggle  LEDs on the Explorer-16
    mPORTAToggleBits(BIT_3 | BIT_2 | BIT_1 | BIT_0);
}
