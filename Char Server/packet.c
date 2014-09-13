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

void sendChar() {
    clearPacket();
    writeHeader(0x0515);
    writeSize(0x025C);
    writeInt(0x81006900);
    writeInt(0x01000138);
    writeString("CharTest", strlen("CharTest"));
    addCount(560);
    writeInt(0x00007919);
    writeByte(0xFF);
    writeString("DigiTest", strlen("DigiTest"));
    writeShort(0x2710);
    writeByte(0x00);
    writeInt(0x18606300);
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
		    printf("Enviando Personagem de Teste !\n");
            sendChar();
        break;

		default:
			printf("Invalid Packet %X\n", HEADER);
			exit(0);
		break;
	}
}


