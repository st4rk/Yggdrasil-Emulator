/*****************************************************************************
**  Esse arquivo é parte do YggdrasilEMU                                    **
**  YggdrasilEMU é um emulador de Digimon Masters Online, ele é open-source **
**  dessa forma você pode utilizar esses arquivos para estudo ou para base  **
**  em outros projetos de emuladores de servidores ou até mesmo do DMO      **
**  Desenvolvido por St4rk                                                  **
******************************************************************************/

#include "socket.h"
#include "packet.h"

extern u32 packet_count;
extern u8 packetBuffer[BUFFER_SIZE];

int nClient = -1;

/* Thread Socket */

DWORD WINAPI thread_socket(LPVOID param) {
    int res = 0;


    printf("Conexao Recebida ! \n");

    int cIndex = nClient;
    SOCKET cClient = (SOCKET)param;

	/* Auth Packet */
    sendAuth();

   #ifdef DEBUG
    printf("Packet Size: %d\n", packet_count);
    int i = 0;
    printf("Packet: ");

    for(i = 0; i < packet_count; i++)
        printf(" %X ", packetBuffer[i]);

    printf("\n");
   #endif // DEBUG

    send(cClient, packetBuffer, packet_count, 0);

	while(1) {
        /* Sleep 10ms */
        Sleep(10);

        /* Get Packet's */
        res = recv(cClient, packetBuffer, BUFFER_SIZE, 0);

        if(res == 0) {
            printf("Um player foi desconectado...\n");
            nClient--;
            closesocket(cClient);
            ExitThread(0);
        }

        if(res > 0) {

        	/* Decode Packet */
            handlePacket(cIndex, packetBuffer);
            #ifdef DEBUG
                for(i = 0; i < packet_count; i++)
                printf(" %X ", packetBuffer[i]);

            printf("\n");

                printf("Packet Size: %d\n", packet_count);
            #endif // DEBUG
    		send(cClient, (char*)packetBuffer, packet_count, 0);

    		/* Clear */
	        res = 0;
	        memset(packetBuffer, 0, BUFFER_SIZE);
	        packet_count = 0;
        }
	}

}

void startSocket() {

	// Inicialização do Socket
	if( WSAStartup( MAKEWORD(2, 2), &wsaData ) != NO_ERROR )
    {
        printf("Erro na Inicialização do WSAStartup\n");
        system("pause");
        WSACleanup();
        exit(1);
    }

    // Criando Socket TCP_IP
    playerSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( playerSocket == INVALID_SOCKET )
    {
        printf("Erro na Criação do Socket\n");
        system("pause");
        WSACleanup();
        exit(1);
    }
}

void startHost(char *IP, int porta) {
 	Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = inet_addr( IP ); // Inicia com o IP que digitamos
    Address.sin_port = htons( porta ); // Porta que Digitamos, no caso é a porta do Login Server do DMO

    if ( bind ( playerSocket, (SOCKADDR*) &Address, sizeof( Address) ) == SOCKET_ERROR ) {
        printf("Erro na Conexão do Socket");
        system("pause");
        WSACleanup();
        exit(1);
    }


     if ( listen ( playerSocket, 1 ) == SOCKET_ERROR ) {
        printf("Erro no Listening do Socket");
        system("pause");
        WSACleanup();
        exit(1);
    }

	printf("Servidor Iniciado ! IP: %s Porta: %d\n", IP, porta);

    /* Inicializa o SQLite */
	startSQLite();
}

void startHandle() {
 	printf("Aguardando Conexoes...\n");

    SOCKADDR_IN from;
	SOCKET client;

    DWORD threadID = 0;

	int fromlen = sizeof(from);

    while(1) {
        if(nClient < MAX_PLAYERS) {
            /* Aceita Conexão */
            client = accept(playerSocket,(struct sockaddr*)&from, &fromlen);

            /* Aumenta o número de clientes */
            nClient++;

            /* Cria a Thread para o novo Player Conectado ! */
            CreateThread(NULL, 0, thread_socket,(LPVOID)client, 0, &threadID);
        }
    }
}

void killSocket() {
 	closesocket(playerSocket);
    WSACleanup();
}

