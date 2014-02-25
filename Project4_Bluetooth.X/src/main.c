#include <plib.h>
#include "header/delays.h"
#include "header/uart.h"


// Define setup Configuration 1 for OpenUARTx
    // Module Enable
    // Work in IDLE mode
    // Communication through usual pins
    // Disable wake-up
    // Loop back disabled
    // Input to Capture module from ICx pin
    // no parity 8 bit
    // 1 stop bit
    // IRDA encoder and decoder disabled
    // CTS and RTS pins are disabled
    // UxRX idle state is '1'
    // 16x baud clock - normal speed
#define CONFIGUART1 	UART_EN | UART_IDLE_CON | UART_RX_TX | UART_DIS_WAKE | UART_DIS_LOOPBACK | UART_DIS_ABAUD | UART_NO_PAR_8BIT | UART_1STOPBIT | UART_IRDA_DIS | UART_DIS_BCLK_CTS_RTS| UART_NORMAL_RX | UART_BRGH_SIXTEEN

// Define setup Configuration 2 for OpenUARTx
    // IrDA encoded UxTX idle state is '0'
    // Enable UxRX pin
    // Enable UxTX pin
    // Interrupt on transfer of every character to TSR
    // Interrupt on every char received
    // Disable 9-bit address detect
    // Rx Buffer Over run status bit clear
#define CONFIGUART2		UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_TX | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS | UART_RX_OVERRUN_CLEAR

// System definitions
#define LED_SYSTEMSTATUS LATAbits.LATA1
#define LED LATBbits.LATB3

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


// Protótipo de funções de configuração
void SystemSetup();
void UARTSetup();


void main(){
    SystemSetup();
    UARTSetup();
    unsigned char command = '2';

    LED_SYSTEMSTATUS = 1;
    LED = 0;

    while(1){
        // Espera uart ficar preparada para receber dado
        //while(!DataRdyUART1()); /* Wait for data in the UARTRx. */

        // Lê comando em uart
        if(DataRdyUART1())
            command = (char)ReadUART1(); /* Read data from Rx. */

        // Trata comando de acordo com a necessidade
        if(command == '1'){
            LED = 1;
        } else if(command == '0'){
            LED = 0;
        }

        while(BusyUART1()); /* Wait till the UART transmitter is free. */
        //putcUART1(command); /* Write data into Tx. */
    } // end while
}


void SystemSetup(void){
    // Configura saidas digitais
    TRISBbits.TRISB3 = 0; // Configura LED_SYSTEMISBLOCKED como saída
    TRISAbits.TRISA1 = 0; // Configura LED_SYSTEMSTATUS como saída


    // Configura entradas digitais

    // Definindo os pinos de portA e portB como digitais
    // Por padrão, o uC é configurado como analógico, por isso a alteração se fez necessária
    // 0 - Configura como entrada DIGITAL
    // 1 - Configura como entrada ANALOGICA
    ANSELA = 0;
    ANSELB = 0;

    TRISBbits.TRISB1 = 1; // RB1 é o RX do uC, precisando ser configurado como entrada digital
}

/******************************************************************************
 * Funcao:	void UARTSetup(void)
 * Entrada:	Nenhuma (void)
 * Saída:	Nenhuma (void)
 * Descrição: 	
 *****************************************************************************/
void UARTSetup(void){
    /*OpenUART1(
        UART_EN,

        UART_RX_ENABLE |
        UART_TX_ENABLE,

        (pbClk/16/DESIRED_BAUDRATE)-1   // formula do data sheet
    );*/

    OpenUART1(
            CONFIGUART1, // Configuração definida no define
            CONFIGUART2, // Configuração definida no define
            (pbClk/16/DESIRED_BAUDRATE)-1   // Formula do data sheet
    );

    // (3)Assign UART1 Pin (Rx1:RP13,Tx1:RP15)
    //  Rx:U1RX==>RPB13
    U1RXR  = 0b0011;
    //  Tx:RPB15R(PORTB15)==>U1TX
    RPB15R = 0b0001;
}

/******************************************************************************
* Funcao:     _U2Handler(void)
* Entrada:	nenhuma
* Saída:	nenhuma
* Descrição: 	handler de interrupcao UART2, priority level 5, subpriority level 0
 *****************************************************************************/
/*void __ISR(_UART2_VECTOR, ipl5) UART2InterruptVector(void){
    unsigned char dado;

    dado = ReadUART2();
    //buffRx[ptrBuffRx++] = dado;
    //if(ptrBuffRx == TXBUFFSIZE)
    if(ptrBuffRx == RXBUFFSIZE){
        ptrBuffRx = 0;
        ibfull = 1;
    }
    IFS1bits.U2RXIF=0;  // limpa flag de interrupção de rx
}*/