#include <stdio.h>
#include "sqlite3.h"

int sqlite3_retorno(void *NotUsed, int argc, char **argv, char **coluna) 
{ // funcao de Callback
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", coluna[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

int createTable()
{
    sqlite3 * db = 0;
    char * mensagem_erro = 0;

    printf("versao: %s\n", sqlite3_libversion());

    int rc = sqlite3_open("database.db", &db);

    if (rc != SQLITE_OK) 
    {
        printf("ERRO ao abrir: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // criando uma tabela dentro do arquivo

    // Tabela usuarios
    char create[] = "CREATE TABLE IF NOT EXISTS usuarios ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "username TEXT UNIQUE NOT NULL"
                    "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, create, sqlite3_retorno, 0, &mensagem_erro);


    sqlite3_close(db);
    return 0;
}

int main(void) 
{
    createTable();
}
 