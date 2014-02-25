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
#define BTN PORTBbits.RB2

#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale

void SystemSetup(void);
/*
 *
 */
void main(void) {
    SystemSetup();

    char buttonPrevState = 0;
    LED = 0;

    while(1){
        // Neste primeiro caso, quando o botão é pressionado o LED acende
        // caso contrário (quando ele não está pressionado), ele apaga
        /*if(BTN){
            LED = 1;
        } else{
            LED = 0;
        }*/

        // Neste segundo caso, quando o botão é pressionado (quando muda de
        // estado de 0 para 1), o estado do LED é invertido
        if(BTN && BTN != buttonPrevState){
            // Necessário debounce no botão
            buttonPrevState = BTN;
            LED = !LED;
        } else if(!BTN)
            buttonPrevState = BTN;
    }
}

void SystemSetup(void){
    // Configura saidas digitais
    TRISBbits.TRISB3 = 0; // Configura LED como saída

    // Configura entradas digitais

    // Definindo os pinos de portA e portB como digitais
    // Por padrão, o uC é configurado como analógico, por isso a alteração se fez necessária
    // 0 - Configura como entrada DIGITAL
    // 1 - Configura como entrada ANALOGICA
    ANSELA = 0;
    ANSELB = 0;

    TRISBbits.TRISB2 = 1;
}