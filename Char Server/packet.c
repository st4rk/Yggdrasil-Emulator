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
    memcpy(packetBuffer, &packetSize, 2);
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

void sendChar(int nIndex, u8* packet) {
    int i = 0;
    /* Pegar AccountID e AccountUID */

    memcpy(&nPlayer[nIndex].AccountID, packet + 8, 4);
    memcpy(&nPlayer[nIndex].AccountUID, packet + 12, 4);

    SQLITE_getPlayer(&nPlayer[nIndex]);

    clearPacket();
    writeHeader(SEND_CHAR_PACKET);
    /* ???? */
    writeShort(0x6900);
    /* ID do Mapa que o personagem se encontra */
    writeShort(0x8100);
    /* ID Do Modelo do Personage */
    writeShort((unsigned short)nPlayer[nIndex].nChar[0].modelo);
    /* ?????? */
    writeByte(0x0);
    /* Player Level */
    writeByte(nPlayer[nIndex].nChar[0].level);
    writeString(nPlayer[nIndex].nChar[0].nome, strlen(nPlayer[nIndex].nChar[0].nome));

    /* Itens do Personagem, ainda precisa ser implementado */
    for(i = 0; i < 140; i++) {
        writeInt(0x0);
    }

    /* ID do Digimon */
    writeInt(nPlayer[nIndex].nChar[0].Digimon.ID);
    /* Level do Digimon */
    writeByte(nPlayer[nIndex].nChar[0].Digimon.level);
    writeString(nPlayer[nIndex].nChar[0].Digimon.nome, strlen(nPlayer[nIndex].nChar[0].Digimon.nome));
    writeShort(0x2710);
    writeByte(0x00);
    writeShort(0x6300);
    writeShort(getCount() + 2 ^ checkSum);
    writeSize(getCount());
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

		case(RECV_CHAR_PACKET):
            printf("Pedido de Personage...\n");
            sendChar(nIndex, packet);
        break;

		default:
			printf("Invalid Packet %X\n", HEADER);
			exit(0);
		break;
	}
}


