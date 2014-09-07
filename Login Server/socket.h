#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>


WSADATA wsaData;
SOCKET playerSocket;
struct sockaddr_in Address;

void startSocket();
void startHost(char *IP, int porta);
void startHandle();
void killSocket();

#endif
