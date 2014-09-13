/*****************************************************************************
**  Esse arquivo é parte do YggdrasilEMU                                    **
**  YggdrasilEMU é um emulador de Digimon Masters Online, ele é open-source **
**  dessa forma você pode utilizar esses arquivos para estudo ou para base  **
**  em outros projetos de emuladores de servidores ou até mesmo do DMO      **
**  Desenvolvido por St4rk                                                  **
******************************************************************************/

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

int SQLITE_getPlayer(PLAYER_STRUCT* A) {
    int rc;
    char *sql = "SELECT * FROM chars";

    rc = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

    if( rc != SQLITE_OK ) {
        printf("Erro no prepare_v2 %s\n", sqlite3_errstr(rc));
    }

    do {
       rc = sqlite3_step(stmt);

       if ( rc == SQLITE_ROW ) {
            if(A->AccountID == sqlite3_column_int(stmt, 0) && A->AccountUID == sqlite3_column_int(stmt, 1)) {

                strcpy(A->nChar[0].nome, sqlite3_column_text(stmt, 2));
                A->nChar[0].level = sqlite3_column_int(stmt, 3);
                A->nChar[0].modelo = sqlite3_column_int(stmt, 4);
                strcpy(A->nChar[0].Digimon.nome, sqlite3_column_text(stmt, 5));
                A->nChar[0].Digimon.level = sqlite3_column_int(stmt, 6);
                A->nChar[0].Digimon.ID = sqlite3_column_int(stmt, 7);
                return 0;
            }
       }

    } while( rc != SQLITE_DONE );

    return -1;
}

#endif // MYSQL_H


