#include <plib.h>
#include "header/delays.h"
#include "header/uart.h"


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
    // Interrupt on transfer of every character to TSR -- UART_INT_TX
    // Interrupt on every char received
    // Disable 9-bit address detect
    // Rx Buffer Over run status bit clear
#define CONFIGUART2		UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_RX_CHAR | UART_ADR_DETECT_DIS | UART_RX_OVERRUN_CLEAR


// System definitions
#define LED_SYSTEMSTATUS LATAbits.LATA1
#define LED LATBbits.LATB3

#define TXBUFFSIZE  16
#define RXBUFFSIZE  8


// Protótipo de funções de configuração
void SystemSetup();
void UARTSetup();


void main(){
    SystemSetup();
    UARTSetup();

    LED_SYSTEMSTATUS = 1;
    LED = 0;

    while(1){

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
}

/******************************************************************************
 * Funcao:	void UARTSetup(void)
 * Entrada:	Nenhuma (void)
 * Saída:	Nenhuma (void)
 * Descrição: 	
 *****************************************************************************/
void UARTSetup(void){
    OpenUART1(
            UART_EN                | // modo UART2 habilitado
            UART_IDLE_CON          | // continua operacao em modo idle
            UART_ALTRX_ALTTX       | // usa pinos alternativos
            UART_DIS_WAKE          | // desabilita wakeup on start
            UART_DIS_LOOPBACK      | // desabilita loopback
            UART_DIS_ABAUD         | // desabilita auto baud rate
            UART_NO_PAR_8BIT       | // sem paridade, 8 bits
            UART_1STOPBIT          | // um stop bit
            UART_IRDA_DIS          | // desabilita IRDA
            UART_DIS_BCLK_CTS_RTS  | // desabilita CTS e RTS
            UART_NORMAL_RX         | // RX normal (não invertido)
            UART_BRGH_SIXTEEN      , // baud rate speed padrao

            UART_TX_PIN_LOW        | // break bit disable
            UART_RX_ENABLE         | // habilita recepcao
            UART_TX_ENABLE         | // habilita transmissao
            //UART_INT_TX          | // gera interrupcao com um espaco vazio no buffer tx
            UART_INT_RX_CHAR       | // interrupcao é gerada com pelo menos um char no buffer rx
            UART_ADR_DETECT_DIS    | // Address Detect mode desbilitado
            UART_RX_OVERRUN_CLEAR  , // Rx buffer Over run status bit clear


            (pbClk/16/DESIRED_BAUDRATE)-1   // Formula do data sheet
    );

    // (3)Assign UART1 Pin (Rx1:RP13,Tx1:RP15)
    //  Rx:U1RX==>RPB13
    U1RXR  = 0b0011;
    //  Tx:RPB15R(PORTB15)==>U1TX
    RPB15R = 0b0001;

    IPC8bits.U1IP=2;    // prioridade 2
    IPC8bits.U1IS=0;    // sub-prioridade  0
    IFS1bits.U1RXIF=0;  // limpa flag de interrupcao de rx
    IFS1bits.U1TXIF=0;  // limpa flag de interrupcao de tx
    IEC1bits.U1RXIE=1;  // habilita interrupção de rx

    INTEnable(INT_U1RX, INT_ENABLED);  // RX Interrupt is enabled
    INTEnable(INT_U1TX, INT_DISABLED);  // TX Interrupt is disabled, no need to fire into it now, only when we need to send out somethings!!


    // Enable the interrupt handling from the core
    INTEnableInterrupts();
}

/******************************************************************************
* Funcao:     _U1Handler(void)
* Entrada:	nenhuma
* Saída:	nenhuma
* Descrição: 	handler de interrupcao UART1, priority level 5, subpriority level 0
 *****************************************************************************/
void __ISR(_UART1_VECTOR, ipl2) UART1InterruptVector(void){
    unsigned char command;

    LED = 1;
    command = ReadUART1();
    if(command == '1')
        LED = 1;
    else if(command == '0')
        LED = 0;
    IFS1bits.U1RXIF = 0;  // limpa flag de interrupção de rx
}