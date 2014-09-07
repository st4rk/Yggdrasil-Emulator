#ifndef YGGDRASIL_H
#define YGGDRASIL_H

#define MAX_PLAYERS 100
#define MAX_CHANNEL 2
#define BUFFER_SIZE 2048
#define checkSum 6716
#define DEBUG 1

/* Typedef's */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct PLAYER_STRUCT {
  u8 name[20];
  u32 nLen;
  u8 senha[20];
  u32 sLen;
  u32 ID;
  u32 uID;
} PLAYER_STRUCT;

typedef struct CHANNEL_STRUCT {
    u32 ID;
    u8 name[20];
    u32 players;
} CHANNEL_STRUCT;

CHANNEL_STRUCT nChannel[MAX_CHANNEL];
PLAYER_STRUCT nAccount[MAX_PLAYERS];

#endif
