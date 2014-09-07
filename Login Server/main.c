#include "socket.h"
#include <stdio.h>
#include <stdlib.h>


int main( void ) {
	printf("Yggdrasil Emulator - Login Server\n");
	printf("Written by St4rk - Rev 1.00.0\n");
    startSocket();
    startHost("127.0.0.1", 7029);
    startHandle();
    killSocket();
	return 0;
}
