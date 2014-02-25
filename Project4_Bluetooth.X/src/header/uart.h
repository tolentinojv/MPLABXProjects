/*
 * File:   uart.h
 * Author: JULIO CONWAY
 *
 * Created on 18 de Julho de 2013, 14:37
 */
#include <plib.h>

#define DESIRED_BAUDRATE (115200) //9600
#define pbClk 80000000





/******************************************************************************
 * Funcao:	void putcharUART(void)
 * Entrada:	caracter a ser transmitirdo
 * Saída:	Nenhuma (void)
 * Descrição: 	transmite um caracter pela porta serial
 *****************************************************************************/
void putcharUART(unsigned char ch){
    WriteUART2(ch);
}

/******************************************************************************
 * Funcao:	void sendDataBuffer(void)
 * Entrada:	string a ser transmitirda
 * Saída:	Nenhuma (void)
 * Descrição: 	transmite um buffer pela porta serial
 *****************************************************************************/
void sendDataBuffer(const char *buffer, UINT32 size){
    while(size){
        while(BusyUART2());
        putcharUART(*buffer);
        buffer++;
        size--;
    }
    putcharUART('\n');
}


/******************************************************************************
 * Funcao:	void sendDataBuffer(void)
 * Entrada:	buffer a ser transmitirda
 * Saída:	Nenhuma (void)
 * Descrição: 	transmite um buffer pela porta serial
 *****************************************************************************/
void sendByteBuffer(int *buffer, UINT32 size){
    while(size){
        while(BusyUART2());
        putcharUART(*buffer);
        buffer++;
        size--;
    }
}