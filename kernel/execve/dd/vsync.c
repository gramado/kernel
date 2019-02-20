/*
 * File: vsync.c
 *
 * Descri��o:
 *     Sincronisa o retra�o vertical.
 *
 *     Na hora de enviar os dados para o LFB �
 * necess�rio sincronizar com o retra�o vertical do monitor.
 *
 *     Pertence ao hal.
 *     N�o sei se pertence a x86. ???!!!
 *
 * History:
 *     2015 - Created by Fred Nora.
 */

 
#include <kernel.h>


// Defini��es.
#define VSYNC_INPORT 0x3DA



// Fun��es internas.
char vsync_inb(int port);


 
void vsync (){
	
	while ( ( vsync_inb (0x3DA) & 8 ) != 8 );
	
	while ( ( vsync_inb (0x3DA) & 8 ) == 8 );
}


/*
 * vsync_inb: 
 *     Pega um byte na porta. */

char vsync_inb (int port){
	
    char Value = 0;
	
    Value = (char) inportb (port); 
	
	asm (" nop \n");
	asm (" nop \n");
	asm (" nop \n");
	asm (" nop \n");
	
	return (char) Value;    
}


//
// End.
//

