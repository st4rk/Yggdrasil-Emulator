/*****************************************************************************
**  Esse arquivo é parte do YggdrasilEMU                                    **
**  YggdrasilEMU é um emulador de Digimon Masters Online, ele é open-source **
**  dessa forma você pode utilizar esses arquivos para estudo ou para base  **
**  em outros projetos de emuladores de servidores ou até mesmo do DMO      **
**  Desenvolvido por St4rk                                                  **
******************************************************************************/

#ifndef YGGDRASIL_H
#define YGGDRASIL_H

#define MAX_PLAYERS 100
#define MAX_CHAR 3
#define BUFFER_SIZE 2048
#define checkSum 6716
#define DEBUG 1

/* Typedef's */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


typedef struct DIGI_STRUCT {
    u8 nome[20];
    u8 level;
    u32 ID;

} DIGI_STRUCT;

typedef struct CHAR_STRUCT {
    u8 nome[20];
    u8 level;
    u32 modelo;

    DIGI_STRUCT Digimon;
} CHAR_STRUCT;

typedef struct PLAYER_STRUCT {
    u32 AccountID;
    u32 AccountUID;
    CHAR_STRUCT nChar[MAX_CHAR];
} PLAYER_STRUCT;

PLAYER_STRUCT nPlayer[MAX_PLAYERS];


#endif
