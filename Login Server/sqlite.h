#ifndef MYSQL_H
#define MYSQL_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "yggdrasil.h"

/* mysql não vai ser implementado agora, vou implementar depois que termiar o charserver u_u'' */

sqlite3 *db;
sqlite3_stmt *stmt;
char *errMsg = 0;

extern CHANNEL_STRUCT nChannel[MAX_CHANNEL];

void SQLITE_start() {
    int rc;
    printf("Inicializando SQLite...\n");

    rc = sqlite3_open("D:\\database\\dmo.db", &db);

    if( rc ) {
        printf("Erro na database do Sqlite: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }

    printf("SQLite inicializado com sucesso !\n");
}

void SQLITE_close() {
    sqlite3_close(db);
}

u8 SQLITE_getChannels() {
    int rc;
    int nCanais = 0;
    char *sql = "SELECT * FROM canais";

    rc = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

    if( rc != SQLITE_OK ) {
        printf("Erro ao acessar canais: %s !\n", sqlite3_errstr(rc));
    }

    do {
        rc = sqlite3_step(stmt);

        if ( rc == SQLITE_ROW ) {
            nChannel[nCanais].ID = sqlite3_column_int(stmt, 0);
            strcpy(nChannel[nCanais].name, sqlite3_column_text(stmt, 1));
            nChannel[nCanais].players = sqlite3_column_int(stmt, 2);

            nCanais++;
        }
    }  while( rc != SQLITE_DONE );

    return nCanais;
}

int SQLITE_verifyAccountDB(PLAYER_STRUCT* A) {
    int rc;
    char *sql = "SELECT * FROM contas";

    rc = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

    if( rc != SQLITE_OK ) {
        printf("Erro no prepare_v2 %s\n", sqlite3_errstr(rc));
    }

    do {
       rc = sqlite3_step(stmt);

       if ( rc == SQLITE_ROW ) {
            if ( strcmp(sqlite3_column_text(stmt, 2), A->name) == 0 ) {
                if(strcmp(sqlite3_column_text(stmt, 3), A->senha) == 0) {
                    /* Carrega conta */
                    A->ID = sqlite3_column_int(stmt, 0);
                    A->uID = sqlite3_column_int(stmt, 1);
                    return 1;
                } else {
                    return 0;
                }
            }
       }

    } while( rc != SQLITE_DONE );

    return -1;
}

#endif // MYSQL_H


