/******************************************************************************
 * Funcao:        	void DelayMsx (unsigned int ms)
 * Entrada:         unsigned int ms - Tempo em milesegundos de delay de 1 a 1000 ms
 * Sa�da:          	Nenhuma (void)
 * Descri��o:       Esta fun��o faz espera ocupada com a CPU por ms milesegundos.
 *					Onde ms � recebido como parametro da fun��o.
 *					Para o correto funcionamento da fun��o o clock deve ser definido
 *					atrav�s da fun��o ConfiguraDelays(unsigned char FOSC).
 *
 *****************************************************************************/
#include "header/delays.h"

void DelayMsx(unsigned int ms)
{
	unsigned int PR2Value;
	
	//Configura��o do timer p/ 1 ms
	// Tempo = 1/PBCK x Preescaler x PRx
	// Onde:
	// PBCK - Frequencia do barramento de perif�rico
	// Preescaler - Divis�o do clock (1:1, 1:2, 1:4, 1:8, 1:16, 1:32, 1:64, 1:256)
	// PRx - Periodo do timer (Valor a ser calculado na formula a cima).

	// Tempo 1 - 200 ms -> preescaler = 1:256
	// Tempo 1 > 200 ms N�o existe preescaler para ele.
	// 
	
	if(ms>0 & ms<201)
	{	
		mT2ClearIntFlag(); // Limpa a flag do timer2
		PR2Value = ms * 80000 / 256;
		
		OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
				   T2_PS_1_256,   // Preescaler de 1:256
				   PR2Value); 	  // Periodo do timer calculado acima
		//Aguarda o estouro do timer
		 while(IFS0bits.T2IF == 0);

	}
	else
	{
		if(ms>200 & ms<401)
		{
			// Aguarda 200 ms:
			mT2ClearIntFlag(); 		  // Limpa a flag do timer2
			OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
					   T2_PS_1_256,   // Preescaler de 1:256
					   62500); 	  // Periodo do timer calculado acima
			//Aguarda o estouro do timer
		 	while(IFS0bits.T2IF == 0);
		 	// Calcula PR2Value:
		 	
			mT2ClearIntFlag(); // Limpa a flag do timer2
			PR2Value = (ms - 200) * 80000 / 256;
			
			OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
					   T2_PS_1_256,   // Preescaler de 1:256
					   PR2Value); 	  // Periodo do timer calculado acima
			//Aguarda o estouro do timer
		 	while(IFS0bits.T2IF == 0);
		}
		else
		{
			if(ms>400 & ms<601)
			{
				// Aguarda 200 ms:
				mT2ClearIntFlag(); 		  // Limpa a flag do timer2
				OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
						   T2_PS_1_256,   // Preescaler de 1:256
						   62500); 	  // Periodo do timer calculado acima
				//Aguarda o estouro do timer
			 	while(IFS0bits.T2IF == 0);
			 	// Aguarda 200 ms:
				mT2ClearIntFlag(); 		  // Limpa a flag do timer2
				OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
						   T2_PS_1_256,   // Preescaler de 1:256
						   62500); 	  // Periodo do timer calculado acima
				//Aguarda o estouro do timer
			 	while(IFS0bits.T2IF == 0);
		 		// Calcula PR2Value:
		 	
				mT2ClearIntFlag(); // Limpa a flag do timer2
				PR2Value = (ms - 400) * 80000 / 256;
			
				OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
						   T2_PS_1_256,   // Preescaler de 1:256
						   PR2Value); 	  // Periodo do timer calculado acima
				//Aguarda o estouro do timer
		 		while(IFS0bits.T2IF == 0);
			}
			else
			{
				if(ms>600 & ms<801)
				{
					// Aguarda 200 ms:
					mT2ClearIntFlag(); 		  // Limpa a flag do timer2
					OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
							   T2_PS_1_256,   // Preescaler de 1:256
							   62500); 	  // Periodo do timer calculado acima
					//Aguarda o estouro do timer
				 	while(IFS0bits.T2IF == 0);
				 	// Aguarda 200 ms:
					mT2ClearIntFlag(); 		  // Limpa a flag do timer2
					OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
							   T2_PS_1_256,   // Preescaler de 1:256
							   62500); 	  // Periodo do timer calculado acima
					//Aguarda o estouro do timer
				 	while(IFS0bits.T2IF == 0);
				 	// Aguarda 200 ms:
					mT2ClearIntFlag(); 		  // Limpa a flag do timer2
					OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
							   T2_PS_1_256,   // Preescaler de 1:256
							   62500); 	  // Periodo do timer calculado acima
					//Aguarda o estouro do timer
				 	while(IFS0bits.T2IF == 0);
				 	// Calcula PR2Value:
			 	
					mT2ClearIntFlag(); // Limpa a flag do timer2
					PR2Value = (ms - 600) * 80000 / 256;
				
					OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
							   T2_PS_1_256,   // Preescaler de 1:256
							   PR2Value); 	  // Periodo do timer calculado acima
					//Aguarda o estouro do timer
			 		while(IFS0bits.T2IF == 0);
				}
				else
				{
					if(ms>800 & ms<1001)
					{
							// Aguarda 200 ms:
						mT2ClearIntFlag(); 		  // Limpa a flag do timer2
						OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
								   T2_PS_1_256,   // Preescaler de 1:256
								   62500); 	  // Periodo do timer calculado acima
						//Aguarda o estouro do timer
					 	while(IFS0bits.T2IF == 0);
					 	// Aguarda 200 ms:
						mT2ClearIntFlag(); 		  // Limpa a flag do timer2
						OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
								   T2_PS_1_256,   // Preescaler de 1:256
								   62500); 	  // Periodo do timer calculado acima
						//Aguarda o estouro do timer
					 	while(IFS0bits.T2IF == 0);
					 	// Aguarda 200 ms:
						mT2ClearIntFlag(); 		  // Limpa a flag do timer2
						OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
								   T2_PS_1_256,   // Preescaler de 1:256
								   62500); 	  // Periodo do timer calculado acima
						//Aguarda o estouro do timer
					 	while(IFS0bits.T2IF == 0);
					 	// Aguarda 200 ms:
						mT2ClearIntFlag(); 		  // Limpa a flag do timer2
						OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
								   T2_PS_1_256,   // Preescaler de 1:256
								   62500); 	  // Periodo do timer calculado acima
						//Aguarda o estouro do timer
					 	while(IFS0bits.T2IF == 0);
					 	// Calcula PR2Value:
				 	
						mT2ClearIntFlag(); // Limpa a flag do timer2
						PR2Value = (ms - 800) * 80000 / 256;
					
						OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
								   T2_PS_1_256,   // Preescaler de 1:256
								   PR2Value); 	  // Periodo do timer calculado acima
						//Aguarda o estouro do timer
				 		while(IFS0bits.T2IF == 0);	
					}	
				}
			}		
		}	
	}				
}	
	
/******************************************************************************
 * Funcao:        	void DelayUsx(char us);
 * Entrada:         unsigned int us - Tempo em microsegundos de delay
 * Sa�da:          	Nenhuma (void)
 * Descri��o:       Esta fun��o faz espera ocupada com a CPU por us microsegundos.
 *					Onde us � recebido como parametro da fun��o.
 *					Para o correto funcionamento da fun��o o clock deve ser definido
 *					atrav�s da fun��o ConfiguraDelays(unsigned char FOSC).
 *
 *****************************************************************************/
 void DelayUsx(unsigned int us)
 {
	unsigned int PR2Value;
	
	if(us>0 & us<1001)
	{	
		mT2ClearIntFlag(); // Limpa a flag do timer2
		PR2Value = us * 80 / 64;// -1 para ajuste
		
		OpenTimer2(T2_ON  	    | // M�dulo Timer Habilitado
				   T2_PS_1_64,   // Preescaler de 1:256
				   PR2Value); 	  // Periodo do timer calculado acima
		//Aguarda o estouro do timer
		 while(IFS0bits.T2IF == 0);
	}	 
 }
	
	
