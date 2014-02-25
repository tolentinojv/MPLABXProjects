/*
 * File:   main.c
 * Author: João
 *
 * Created on 3 de Novembro de 2013, 12:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <p32xxxx.h>


// Clock da CPU: utiliza um cristal de 8 MHz (8MHz/2 = 4MHz --> 4MHz x 20 = 80MHz --> 80MHz * 1 = 80MHz)
// SYSCLK = 80 MHz (8MHz Cristal * FPLLIDIV * FPLLMUL * FPLLODIV)
// SYSCLK = 80 MHz (8MHz * 1/2 * 20 * 1/1)
#pragma config FPLLIDIV   = DIV_2
#pragma config FPLLMUL    = MUL_20
#pragma config FPLLODIV   = DIV_1

#pragma config FNOSC      = PRIPLL  // Primary Osc w/PLL (XT+,HS+,EC+PLL)
#pragma config FSOSCEN    = OFF     // Oscilador secundario desligado
#pragma config POSCMOD    = HS      // Configuração do oscilador primario: HS
#pragma config OSCIOFNC   = OFF     // Desabilita função clko output signal
#pragma config FPBDIV     = DIV_1   // Configura a velocidade do barrmento: 80 NHz
#pragma config FWDTEN     = OFF       // Watchdog Timer
#pragma config WDTPS      = PS1       // Watchdog Timer Postscale


#define LED LATBbits.LATB3
#define RESOLUTION  (3.3/1023.0) // Resolução do CAD de 10bits = 3,3V/(2^10-1

void SystemSetup(void);
float ReadADConverter(void);
/*
 *
 */
void main(void) {
    SystemSetup();

    float adcVoltageResult = 0;	// Guarda o valor da conversão AD em VOLTS.

    LED = 1;

    while(1){
        adcVoltageResult = ReadADConverter();

        // Tensão alta - pouca insidência de luz
        // Tensão baixa - muita insidência de luz
        if(adcVoltageResult < 1.5){ // Se resultado for maior que metade, apaga LED
            LED = 0;
        } else{
            LED = 1;
        }
    }
}

void SystemSetup(void){
    // Configura saidas digitais
    TRISBbits.TRISB3 = 0; // Configura LED como saída

    // Definindo os pinos de portA e portB como digitais
    // Por padrão, o uC é configurado como analógico, por isso a alteração se fez necessária
    // 0 - Configura como entrada DIGITAL
    // 1 - Configura como entrada ANALOGICA
    ANSELA = 0;
    ANSELB = 0;
    // Configura entradas analógicas
    // configure and enable the ADC
    CloseADC10();    // ensure the ADC is off before setting the configuration

    OpenADC10(  ADC_MODULE_ON | // Turn module on
                ADC_FORMAT_INTG16 | // Ouput is integer
                ADC_CLK_MANUAL | // Trigger mode auto
                ADC_AUTO_SAMPLING_OFF |
                ADC_SAMP_ON, // Enable autosample

                ADC_VREF_AVDD_AVSS | // ADC ref external
                ADC_OFFSET_CAL_DISABLE | // Disable offset test
                ADC_SCAN_OFF | // Disable scan mode
                ADC_SAMPLES_PER_INT_1 | // Perform 1 samples
                ADC_ALT_BUF_OFF | // Don't use dual buffers
                ADC_ALT_INPUT_OFF, // Don't use alternate mode

                ADC_CONV_CLK_SYSTEM | // Use ADC internal clock
                ADC_CONV_CLK_32Tcy,

                ENABLE_AN3_ANA, // set AN3 as analog input

                SKIP_SCAN_ALL // Do not assign channels to scan
            ); // configure ADC using parameter define above

    EnableADC10(); // Enable the ADC

    // configure to sample AN3
    SetChanADC10(   ADC_CH0_NEG_SAMPLEA_NVREF | // Use ground as neg ref for A
                    ADC_CH0_POS_SAMPLEA_AN3 // Use AN3 for input A
                ); // configure to sample AN3
}

float ReadADConverter(void){
    float binaryResult;
    float voltageResult;

    AcquireADC10();
    ConvertADC10();
    while(BusyADC10())
        DelayMsx(1);
    binaryResult = ReadADC10(0);      //Leitura do bufferi
    voltageResult = binaryResult * RESOLUTION;  // transforma para volts

    return voltageResult;
}