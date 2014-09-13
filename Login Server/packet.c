/*****************************************************************************
**  Esse arquivo é parte do YggdrasilEMU                                    **
**  YggdrasilEMU é um emulador de Digimon Masters Online, ele é open-source **
**  dessa forma você pode utilizar esses arquivos para estudo ou para base  **
**  em outros projetos de emuladores de servidores ou até mesmo do DMO      **
**  Desenvolvido por St4rk                                                  **
******************************************************************************/

#include "packet.h"
#include "sqlite.h"

void startSQLite() {
    SQLITE_start();
}

void writeHeader(u16 packetHeader) {
	memcpy(packetBuffer + 2, &packetHeader, 2);

	packet_count = 4;
}

void writeByte(u8 b) {
	packetBuffer[packet_count] = b;
	packet_count += 1;
}

void writeShort(u16 s) {
	memcpy(packetBuffer + packet_count, &s, 2);
	packet_count += 2;
}

void writeInt(u32 i) {
	memcpy(packetBuffer + packet_count, &i, 4);
	packet_count += 4;
}

void writeString(u8* T, u32 size) {
    packetBuffer[packet_count] = size;
    packet_count += 1;
    memcpy(packetBuffer + packet_count, T, size);
	packet_count += size;
	writeByte(0x0);
}

void writeSize(u16 packetSize) {
	packetBuffer[1] = packetSize & 0xFF00;
	packetBuffer[0] = packetSize & 0x00FF;
}

void clearPacket() {
	memset(packetBuffer, 0, BUFFER_SIZE);
	packet_count = 0;
}

void addCount(int i) {
	packet_count += i;
}

int getCount() { return packet_count; }

/* Server Authentication */
void sendAuth() {
	clearPacket();
	writeHeader(SEND_AUTH_PACKET);
	writeInt(0x1A3463EE);
	writeSize(0x08);
	#ifdef DEBUG
	printf("Authentication...\n");
    #endif // DEBUG
}

/* Server Confirmation */
void sendConfirm() {
	clearPacket();
	writeHeader(SEND_CONFIRM_PACKET);
	writeInt(0x63EF1DAF);
	writeInt(0x1A304EC2);
	writeSize(0x0C);
	#ifdef DEBUG
	printf("Confirmation...\n");
    #endif // DEBUG
}

/* Mensagem to Client */
void sendMensagem(char *name, int size) {
	clearPacket();
	writeHeader(SEND_MENSAGEM_PACKET);
	writeInt(0x0);
	writeString(name, size);
	writeString("NULL", 4);
	writeShort((packet_count + 2) ^ checkSum);
	writeSize(packet_count);
}

void sendChannels(int nIndex) {
    u8 t_channel = SQLITE_getChannels();
    int i;

    clearPacket();

    writeHeader(SEND_CHANNEL_PACKET);
    writeByte(t_channel);

    for(i = 0; i < t_channel; i++) {
        writeInt(nChannel[i].ID);
        writeString(nChannel[i].name, strlen(nChannel[i].name));
        writeByte(nChannel[i].manu);
        /* Quantidade de Players no Servidor */
        writeByte(0x5); // 04 = Medio, 05 = full
        /* Quantidade de Personagens que você tem no servidor */
        writeByte(0x0);
    }

    writeString(nAccount[nIndex].name, strlen(nAccount[nIndex].name));
    writeShort(getCount() + 2 ^ checkSum);
    writeSize(getCount());

}

void sendSelectedChannel(int nIndex, u8* Packet) {
    int ChannelID = 0;
    /* Qual o channel selecionado ? */
    memcpy(&ChannelID, Packet + 4, 4);

    //if(nChannel[ChannelID].players >= nChannel[ChannelID].max_players)
    //    sendMensagem("Servidor Lotado !", strlen("Servidor Lotado !"));

    writeHeader(SEND_INFO_PACKET);
    writeSize(0x1D);
    writeInt(nAccount[nIndex].ID);
    writeInt(nAccount[nIndex].uID);
    writeString("127.0.0.1", strlen("127.0.0.1"));
    writeShort(0x1B57);
    writeShort(0x0000);
    writeShort(0x1D ^ checkSum);
}

void verifyAccounts(int nIndex, u8 *packet) {
    nAccount[nIndex].nLen = packet[12];
    memcpy(nAccount[nIndex].name, packet + 13, nAccount[nIndex].nLen);
    nAccount[nIndex].sLen = packet[nAccount[nIndex].nLen + 12];
    memcpy(nAccount[nIndex].senha, packet + (packet[12] + 15), nAccount[nIndex].sLen);

    #ifdef DEBUG
    printf("%s\n", nAccount[nIndex].name);
    printf("%s\n", nAccount[nIndex].senha);
    #endif // DEBUG

    int res = SQLITE_verifyAccountDB(&nAccount[nIndex]);

    switch(res) {
        case 0:
            sendMensagem("Login ou Senha Incorretas !", strlen("Login ou Senha Incorretas !"));
            break;

        case 1:
            sendChannels(nIndex);
            break;

        case 2:
            sendMensagem("Conta ja esta em uso !", strlen("Conta ja esta em uso !"));
            break;

        case(-1):
            sendMensagem("Conta nao existe", strlen("Conta nao existe"));
            break;

        default:

            printf("Numero Invalido %d:\n", res);
            exit(0);
            break;
    }

}

void handlePacket(int nIndex, u8 *packet) {
	/* Decode Packet Header */

	u16 HEADER = packet[2] << 8 | packet[3];

	switch(HEADER) {
		case(RECV_AUTH_PACKET):
			printf("Enviando Autenticação!\n");
			sendAuth();
		break;

		case(RECV_CONFIRM_PACKET):
			printf("Enviando Confirmação !\n");
			sendConfirm();
		break;

		case(RECV_ACCOUNT_PACKET):
            printf("Conta Recebida !\n");
            verifyAccounts(nIndex, packet);
		break;

		case(RECV_REFRESH_PACKET):
            sendChannels(nIndex);
        break;

        case(RECV_SCHANNEL_PACKET):
            sendSelectedChannel(nIndex, packet);
        break;

		default:
			printf("Invalid Packet %X\n", HEADER);
			exit(0);
		break;
	}
}


