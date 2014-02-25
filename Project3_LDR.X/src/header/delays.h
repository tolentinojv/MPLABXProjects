/******************************************************************************
 *
 *            Biblioteca PICMINAS (PIC32MX775F256L)
 *
 ******************************************************************************
 * Nome do Arquivo:     Delays.h
 * Dependencias:    	Veja se��o INCLUDES
 * Processador:       	PIC32MX775F256L
 * Op��o de Clock:		HS 10MHz (externo) - CPU: 80MHz	
 * Compilador:        	C32
 * Empresa:         	PICMinas
 *
 * Plataforma:			PICMinas32
 *
 * Autor:				Bruno Silveira Avelar
 * 						
 * Revis�o:				v1 - 21/Fev/2011 - Bruno Silveira Avelaros Corr�a
 *
 * Descri��o:        	Conjunto de fun��es para fazer delays atrav�s de
 *						espera ocupada. Estas fun��es foram desenvolvidas para
 *						o microcntrolador PIC32MX e funcionam em um clock de 80MHz.
 *
 *****************************************************************************/
#ifndef __DELAYS_H
#define __DELAYS_H

#include <plib.h> // Biblioteca para uso dos perif�ricos do PIC32.


/******************************************************************************
 * Funcao:        	void DelayMsx (unsigned int ms)
 * Entrada:         unsigned int ms - Tempo em milesegundos de delay
 * Sa�da:          	Nenhuma (void)
 * Descri��o:       Esta fun��o faz espera ocupada com a CPU por ms milesegundos.
 *					Onde ms � recebido como parametro da fun��o.
 *					Para o correto funcionamento da fun��o o clock deve ser definido
 *					atrav�s da fun��o ConfiguraDelays(unsigned char FOSC).
 *
 *****************************************************************************/
void DelayMsx(unsigned int ms);

/******************************************************************************
 * Funcao:        	void DelayUsx(unsigned int us);
 * Entrada:         unsigned int us - Tempo em microsegundos de delay
 * Sa�da:          	Nenhuma (void)
 * Descri��o:       Esta fun��o faz espera ocupada com a CPU por us microsegundos.
 *					Onde us � recebido como parametro da fun��o.
 *					Para valores pequenos passados como parametro a precis�o da fun�ao
 *					ser� baixa. Caso voc� precise de precis�o de poucos microsegundos
 *					considere o n�o uso desta fun��o.
 *					Para o correto funcionamento da fun��o o clock deve ser definido
 *					atrav�s da fun��o ConfiguraDelays(unsigned char FOSC).
 *
 *****************************************************************************/
 void DelayUsx(unsigned int us);

#endif

