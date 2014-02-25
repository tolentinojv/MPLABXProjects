/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>	// Necessário para que o compilador adicione a biblioteca
						// com as definições do PIC selecionado no projeto, neste
						// caso, o modelo 18F2550.

#include <delays.h>		// Biblioteca com funções de delay do compilador C18

#include <usart.h>              // Biblioteca com funções para comunicação serial do C18

#include <timers.h>             // Biblioteca com funções de timer do compilador C18


/** D E F I N E S ************************************************************/
// Saídas Digitais
#define ENGINE                  LATCbits.LATC2
#define	BUZZER			LATCbits.LATC1
#define	RELE			LATCbits.LATC0
#define	LED_AMARELO		LATDbits.LATD0

//Entradas digitais
#define PIR_SENSOR              PORTBbits.RB0
#define VIBRATION_SENSOR        PORTBbits.RB1
#define CAR_START_BUTTON        PORTBbits.RB4

//Outros
#define COMMAND_SIZE 2

/** V A R I A V E I S   G L O B A I S ****************************************/
char carState; // Indica o estado atual do carro
char systemStatus;  //Indica o estado atual do sistema
char command;       //Recebe o comando proveniente da porta serial


/**  P R O T O T I P O S   P R I V A D O S ***********************************/
void ConfiguraSistema(void);
void GetCommand(void);
void TurnDefenderOn(void);
void TurnDefenderOff(void);
void SetAlarmON(void);
void SetAlarmOff(void);
void StartCarEngine(void);
void StopCarEngine(void);

/**  P R O T O T I P O S   D A S   F U N Ç Õ E S   D E   I N T E R R U P Ç Ã O*/
void ConfiguraInterrupcao(void);
void Tratamento_High_Interrupt(void);
void Tratamento_Low_Interrupt(void);

/** COMANDOS DO SISTEMA ******************************************************/

// \% - Nada deve ser feito
// \0x00 - Desativar alarme
// \0x01 - Ativar alarme
// \0x03 - Solicita status do veículo
//
//
//

/** F U N C O E S ************************************************************/

/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Função principal do programa.
 *****************************************************************************/
void main(void)
{

	//Função que faz configurações do firmware
	ConfiguraSistema();

	//Escolhe estado inicial das saidas digitais
        ENGINE = 0;
        BUZZER = 0;

	// Habilita vetores de Alta e Baixa Prioridade
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;

        command = '%';  //Comando inicial
        carState = 0;

	// Laço de repetição infinito que executa aplicação do firmware
	while(1)
	{
            if(DataRdyUSART()){ command = ReadUSART(); }

            if(command != '%'){
                switch(command){
                    case '0': TurnDefenderOff();
                        break;
                    case '1': TurnDefenderOn();
                        break;
                }

                command = '%';
            }

            if(systemStatus == 1){
                if(PIR_SENSOR == 1 || VIBRATION_SENSOR == 1)
                    SetAlarmON();
            }

	}//end while(1)

}//end main


/******************************************************************************
 * Funcao:		void ConfiguraSistema(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	ConfiguraSistema é a rotina de configuração principal do PIC.
 *		Seu objetivo é configurar as portas de I/O e os periféricos
 *		do microcontrolador para que os mesmos trabalhem da maneira
 *		desejada no projeto.
 *****************************************************************************/
void ConfiguraSistema(void){
	// Desabilita pinos analógicos
	ADCON1 = ADCON1 | 0x0f;

	// Configura saidas digitais
        TRISC = 0x00;           // Define todos os pinos do PORT_C como saída
	TRISCbits.TRISC2 = 0;	//RC2 (LED_VERDE)
	TRISCbits.TRISC1 = 0;	//RC1 (BUZZER)

	// Configura entradas digitais
	TRISBbits.TRISB4 = 1; 	//RB4 (BOTAO_BOOT)
        TRISBbits.TRISB0 = 1;   //RB0 (SENSOR_PIR)
        TRISBbits.TRISB1 = 1;   //RB1 (SENSOR_VIBRACAO)

	// Sobre-escreve pino RB4 como entrada digital, após ConfiguraDisplay7seg()
	//te-lo configurado como saida digital para o ponto decimal.
	TRISBbits.TRISB4 = 1; 	//Configura pino RE2 (BOTAO_BOOT) como entrada digital

	// Função que configura as interrupções do firmware
	ConfiguraInterrupcao();

        // Configuração do USART
       OpenUSART( USART_TX_INT_OFF &    //Transmit interrupt OFF
                  USART_RX_INT_OFF &    //Receive interrupt OFF
                  USART_ASYNCH_MODE &   //Asynchronous Mode
                  USART_EIGHT_BIT &     //8-bit transmit/receive
                  USART_CONT_RX  &       //Continuous reception
                  USART_BRGH_LOW,       //Low baud rate
                  2                   // Procurar no DATASHEET -This is the value that is written to the baud rate generator register which determines he baud rate at which the USART operates
                );

}//end ConfiguraSistema


/******************************************************************************
 * Funcao:		void GetCommand(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Função que lê comandos provenientes da porta serial
 *****************************************************************************/
/*void GetCommand(void){
    char character;
    char i;

    character = ReadUSART();
    command[0] = character;
    for(i=1; character != '%'; i++){
        while(!DataRdyUSART());
        command[i] = ReadUSART();
    }
    command[i] = '\0';
}// end GetCommand
*/

/******************************************************************************
 * Funcao:		void SetAlarmOn(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Aciona o buzzer (RC1) com dois bip's curtos
 *****************************************************************************/
void SetAlarmON(void){
	BUZZER=1;
        Delay10KTCYx(10000);
	BUZZER=0;
	Delay10KTCYx(10000);
	BUZZER=1;
	Delay10KTCYx(10000);
	BUZZER=0;
	Delay10KTCYx(10000);
	BUZZER=1;
}


/******************************************************************************
 * Funcao:		void SetAlarmOff(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Aciona o buzzer (RC1) com dois bip's curtos
 *****************************************************************************/
void SetAlarmOff(void){
	BUZZER=0;
}


/******************************************************************************
 * Funcao:		void StartCarEngine(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Liga o motor do carro
 *****************************************************************************/
void StartCarEngine(void){
	ENGINE = 1;
        carState = 1;
}


/******************************************************************************
 * Funcao:		void StopCarEngine(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Desliga o motor do carro
 *****************************************************************************/
void StopCarEngine(void){
	ENGINE = 0;
        carState = 0;
}


/******************************************************************************
 * Funcao:		void TurnDefenderOn(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Aciona o sistema de segurança
 *****************************************************************************/
void TurnDefenderOn(void){
    systemStatus = 1;
    LED_AMARELO = 1;
}


/******************************************************************************
 * Funcao:		void TurnDefenderOff(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Desliga o sistema de segurança
 *****************************************************************************/
void TurnDefenderOff(void){
    systemStatus = 0;
    LED_AMARELO = 0;
    SetAlarmOff();
}


/******************************************************************************
 * Funcao:		void ConfiguraInterrupcao(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Função que configura as interrupções utilizadas no projeto
 *****************************************************************************/
void ConfiguraInterrupcao(void)
{
	//Configuração Global das Interrupções
	RCONbits.IPEN = 1;		// Habilita prioridades de interrupcao
	INTCONbits.GIEH = 0;	// Desabilita vetor de alta prioridade
	INTCONbits.GIEL = 0;	// Desabilita vetor de baixa prioridade

	//Configurações da Interrupção de RB (PORTB4:7)
	INTCONbits.RBIE = 1;		// Habilita interrupcao de RB (PORTB4:7)
	INTCONbits.RBIF = 0;		// Limpa a sinalização da  interrupcao,
                                            //para garantir que nao entra na interrupcao
                                            //por sujeira na memoria
	INTCON2bits.RBIP = 0;		// Tratamento de interrupcao no vetor de Baixa prioridade
	INTCON2bits.RBPU = 1;		// Desabilita os resistores de pull-up internos
//						//do PORTB

        INTCONbits.TMR0IE = 1;          // Habilita interrupcao do timer 0
        INTCONbits.T0IF = 0;            // Limpa a sinalização da  interrupcao,
                                            //para garantir que nao entra na interrupcao
                                            //por sujeira na memoria
        INTCON2bits.T0IP = 1;		// Tratamento de interrupcao no vetor de ALTA prioridade

}// end ConfiguraInterrupcao

/******************************************************************************
 * Funcao:		void Tratamento_High_Interrupt(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Função de tratamento das interrupções de ALTA prioridade
 *		Nessa função deve-se lembrar de fazer a seguinte lista:
 *			1- verificar qual foi a causa da interrupção, comparando
 *		os flags de cada tipo de interrupção.
 *			2- tratar a interrupção selecionada.
 *			3- limpar o flag que causou a interrupção!!! Importante
 *		para garantir que não ocorrerá uma chamada indesejada ao sair
 *		do tratamento da interrupção.
 *
 *		Ao sair dessa função é usado o retorno do tipo "retfie fast",
 *		pois esta função é declarada como ALTA prioridade com a diretiva
 *		#pragma interrupt
 *****************************************************************************/
// ATENÇÃO NA SINTAXE de declaração com #pragma interrupt = Alta prioridade
#pragma interrupt Tratamento_High_Interrupt
void Tratamento_High_Interrupt(void)
{
	// Escreva a função de tratamento de interrupção de Alta prioridade aqui
    WriteTimer0(28036);
    if(INTCONbits.T0IF){
        if(CAR_START_BUTTON == 1 && carState==0){
            if(systemStatus == 1){ SetAlarmON(); }
            else{  StartCarEngine(); }
        }
        else{
            StopCarEngine();
            if(systemStatus == 1){ SetAlarmON(); }
        }

        INTCONbits.TMR0IF = 0;
    }
}// end Tratamento_High_Interrupt

/******************************************************************************
 * Funcao:		void Tratamento_Low_Interrupt(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Função de tratamento das interrupções de BAIXA prioridade
 *		Nessa função deve-se lembrar de fazer a seguinte lista:
 *			1- verificar qual foi a causa da interrupção, comparando
 *		os flags de cada tipo de interrupção.
 *			2- tratar a interrupção selecionada.
 *			3- limpar o flag que causou a interrupção!!! Importante
 *		para garantir que não ocorrerá uma chamada indesejada ao sair
 *		do tratamento da interrupção.
 *
 *		Ao sair dessa função é usado o retorno do tipo "retfie",
 *		pois esta função é declarada como BAIXA prioridade com a diretiva
 *		#pragma interruptlow
 *****************************************************************************/
// ATENÇÃO NA SINTAXE de declaração com #pragma interruptlow = Baixa prioridade
#pragma interruptlow Tratamento_Low_Interrupt
void Tratamento_Low_Interrupt(void)
{
	// Verifica o flag para confirmar se o motivo da chamada da interrupção
	// foi mudança de estado no PORTB
	if(INTCONbits.RBIF)
	{
		

		//Limpa flag da interrupção do PORTB
		INTCONbits.RBIF = 0;

	}// end tratamento da interrupção do PORTB (INTCONbits.RBIF)

}//end Tratamento_Low_Interrupt




/** V E C T O R   R E M A P P I N G ******************************************/
// Seção necessária para informar ao compilador C18 onde são os novos endereços
//da memória de programa que ele deve alocar as rotinas de tratamento do "reset"
//do microcontrolador e das rotinas de tratamento de interrupção.

//
// ATENÇÃO: Copiar esta parte do código dentro do arquivo "main.c" dos
// projetos usados com o Bootloader USB-HID para gravação in-circuit.

// protótipo usado pelo compilador C18
extern void _startup (void);        // See c018i.c in your C18 compiler dir

// Alocação da função de tratamento do "reset" da aplicação principal
// no endereço 0x1000 da memória de programa
#pragma code REMAPPED_RESET_VECTOR = 0x1000
void _reset (void)
{
    _asm goto _startup _endasm
}

// Alocação da função de tratamento das interrupções de ALTA prioridade
// no endereço 0x1008 da memória de programa.
//
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = 0x1008
void _high_ISR (void)
{
    _asm goto Tratamento_High_Interrupt _endasm
}

// Alocação da função de tratamento das interrupções de BAIXA prioridade
// no endereço 0x1018 da memória de programa
#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = 0x1018
void _low_ISR (void)
{
    _asm goto Tratamento_Low_Interrupt _endasm
}

#pragma code	// Diretiva que retorna a alocação dos endereços
				// da memória de programa para seus valores padrão

/** F I M  D A  S E Ç Ã O  D E   V E C T O R   R E M A P P I N G *************/

/** EOF main.c ***************************************************************/
