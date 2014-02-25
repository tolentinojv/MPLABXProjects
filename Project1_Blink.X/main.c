/*
 * File:   main.c
 * Author: João
 *
 * Created on 3 de Novembro de 2013, 12:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>

#define LED LATBbits.LATB3

#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale

void SystemSetup(void);
/*
 *
 */
void main(void) {
    SystemSetup();
    int i;

    LATBbits.LATB3 = 1;

    while(1){
        for(i=0; i<500000; i++);
        LATBbits.LATB3 = !LATBbits.LATB3;
    }
}

void SystemSetup(void){
    // Configura saidas digitais
    TRISBbits.TRISB3 = 0; // Configura LED como saída
}