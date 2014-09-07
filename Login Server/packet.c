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
	writeHeader(SEND_MENSAGEM_PACKET); // 4
	writeInt(0x0);
	writeString(name, size); // 8 + size
	writeString("NULL", 4); // 13 + size
	writeShort((packet_count + 2) ^ checkSum); // 16 + size
	writeSize(packet_count);
}

void sendChannels() {
    u8 t_channel = SQLITE_getChannels();
    u16 packetSize = 18 + strlen(nChannel[0].name) + strlen(nAccount[0].name);

    clearPacket();

    writeHeader(SEND_CHANNEL_PACKET);
    writeSize(packetSize);
    writeByte(0x01);
    writeInt(nChannel[0].ID);
    writeString(nChannel[0].name, strlen(nChannel[0].name));
    writeShort(0x0);
    /* Here is the total players on server, i will implement it later :3 */
    writeByte(0x0);
    writeString(nAccount[0].name, strlen(nAccount[0].name));
    writeShort(packetSize ^ checkSum); // 2

}

void sendSelectedChannel() {
    writeHeader(SEND_INFO_PACKET);
    writeSize(0x1D);
    writeInt(nAccount[0].ID);
    writeInt(nAccount[0].uID);
    writeString("127.0.0.1", strlen("127.0.0.1"));
    writeShort(0x1B57);
    writeShort(0x0000);
    writeShort(0x1D ^ checkSum);
}

void verifyAccounts(u8 *packet) {
    nAccount[0].nLen = packet[12];
    memcpy(nAccount[0].name, packet + 13, nAccount[0].nLen);
    nAccount[0].sLen = packet[nAccount[0].nLen + 12];
    memcpy(nAccount[0].senha, packet + (packet[12] + 15), nAccount[0].sLen);

    #ifdef DEBUG
    printf("%s\n", nAccount[0].name);
    printf("%s\n", nAccount[0].senha);
    #endif // DEBUG

    int res = SQLITE_verifyAccountDB(&nAccount[0]);

    switch(res) {
        case 0:
            sendMensagem("Login ou Senha Incorretas !", strlen("Login ou Senha Incorretas !"));
            break;

        case 1:
            printf("Aqui \n");
            sendChannels();
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

void handlePacket(u8 *packet) {
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
            verifyAccounts(packet);
		break;

		case(RECV_REFRESH_PACKET):
            sendChannels();
        break;

		default:
			printf("Invalid Packet %X\n", HEADER);
			exit(0);
		break;
	}
}


