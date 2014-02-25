/******************************************************************************
 *
 *            Biblioteca PICMINAS (PIC32MX775F256L)
 *
 ******************************************************************************
 * Nome do Arquivo:     Delays.h
 * Dependencias:    	Veja seção INCLUDES
 * Processador:       	PIC32MX775F256L
 * Opção de Clock:		HS 10MHz (externo) - CPU: 80MHz	
 * Compilador:        	C32
 * Empresa:         	PICMinas
 *
 * Plataforma:			PICMinas32
 *
 * Autor:				Bruno Silveira Avelar
 * 						
 * Revisão:				v1 - 21/Fev/2011 - Bruno Silveira Avelaros Corrêa
 *
 * Descrição:        	Conjunto de funções para fazer delays através de
 *						espera ocupada. Estas funções foram desenvolvidas para
 *						o microcntrolador PIC32MX e funcionam em um clock de 80MHz.
 *
 *****************************************************************************/
#ifndef __DELAYS_H
#define __DELAYS_H

#include <plib.h> // Biblioteca para uso dos periféricos do PIC32.


/******************************************************************************
 * Funcao:        	void DelayMsx (unsigned int ms)
 * Entrada:         unsigned int ms - Tempo em milesegundos de delay
 * Saída:          	Nenhuma (void)
 * Descrição:       Esta função faz espera ocupada com a CPU por ms milesegundos.
 *					Onde ms é recebido como parametro da função.
 *					Para o correto funcionamento da função o clock deve ser definido
 *					através da função ConfiguraDelays(unsigned char FOSC).
 *
 *****************************************************************************/
void DelayMsx(unsigned int ms);

/******************************************************************************
 * Funcao:        	void DelayUsx(unsigned int us);
 * Entrada:         unsigned int us - Tempo em microsegundos de delay
 * Saída:          	Nenhuma (void)
 * Descrição:       Esta função faz espera ocupada com a CPU por us microsegundos.
 *					Onde us é recebido como parametro da função.
 *					Para valores pequenos passados como parametro a precisão da funçao
 *					será baixa. Caso você precise de precisão de poucos microsegundos
 *					considere o não uso desta função.
 *					Para o correto funcionamento da função o clock deve ser definido
 *					através da função ConfiguraDelays(unsigned char FOSC).
 *
 *****************************************************************************/
 void DelayUsx(unsigned int us);

#endif

