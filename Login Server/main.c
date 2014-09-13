/*****************************************************************************
**  Esse arquivo é parte do YggdrasilEMU                                    **
**  YggdrasilEMU é um emulador de Digimon Masters Online, ele é open-source **
**  dessa forma você pode utilizar esses arquivos para estudo ou para base  **
**  em outros projetos de emuladores de servidores ou até mesmo do DMO      **
**  Desenvolvido por St4rk                                                  **
******************************************************************************/

#include "socket.h"
#include <stdio.h>
#include <stdlib.h>

char* SERVER_IP = "127.0.0.1";
int SERVER_PORTA = 7029;

int main( void ) {
	printf("Yggdrasil Emulator - Login Server\n");
	printf("Written by St4rk - Rev 1.03\n");
    startSocket();
    startHost(SERVER_IP, SERVER_PORTA);
    startHandle();
    killSocket();
	return 0;
}
