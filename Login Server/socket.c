#include "socket.h"
#include "packet.h"

extern u32 packet_count;
extern u8 packetBuffer[BUFFER_SIZE];

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

	int fromlen = sizeof(from);
    int res = 0;

    client = accept(playerSocket,(struct sockaddr*)&from, &fromlen);

	printf("Conexao Recebida ! \n");

	/* Auth Packet */
    sendAuth();
    printf("Packet Size: %d\n", packet_count);
    int i = 0;
    printf("Packet: ");

    for(i = 0; i < packet_count; i++)
        printf(" %X ", packetBuffer[i]);

    printf("\n");
    send(client, packetBuffer, packet_count, 0);


	while(1) {

        /* Get Packet's */
         res = recv(client, packetBuffer, BUFFER_SIZE, 0);

        if(res > 0) {

        	/* Decode Packet */
            handlePacket(packetBuffer);
            #ifdef DEBUG
                for(i = 0; i < packet_count; i++)
                printf(" %X ", packetBuffer[i]);

            printf("\n");

                printf("Packet Size: %d\n", packet_count);
            #endif // DEBUG
    		send(client, (char*)packetBuffer, packet_count, 0);

    		/* Clear */
	        res = 0;
	        memset(packetBuffer, 0, BUFFER_SIZE);
	        packet_count = 0;
        }
	}
}

void killSocket() {
 	closesocket(playerSocket);
    WSACleanup();
}

