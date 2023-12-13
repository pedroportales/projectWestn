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

int createUsuarios()
{
    sqlite3 * db = 0;
    char * mensagem_erro = 0;

    printf("versao: %s\n", sqlite3_libversion());

    int rc = sqlite3_open("database.sqlite", &db);

    if (rc != SQLITE_OK) 
    {
        printf("ERRO ao abrir: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // criando uma tabela dentro do arquivo

    // Tabela usuario    
    char *create = "CREATE TABLE IF NOT EXISTS usuarios( "
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "usuario UNIQUE TEXT NOT NULL,"
                    "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, create, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_close(db);
    return 0;
}

int createEmprestimos()
{
    sqlite3 * db = 0;
    char * mensagem_erro = 0;

    int rc = sqlite3_open("database.sqlite", &db);

    if (rc != SQLITE_OK) 
    {
        printf("ERRO ao abrir: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *create = "CREATE TABLE emprestimo( "
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "usuario_id INTEGER,"
                    "valor REAL NOT NULL,"
                    "taxa_juros REAL NOT NULL,"
                    "meses INTEGER NOT NULL,"
                    "FOREIGN KEY (usuario_id) REFERENCES usuarios(id))";

    rc = sqlite3_exec(db, create, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_close(db);
    return 0;

}  

int inserirUsuario(char *username, char *password)
{
    sqlite3 * db = 0;
    char * mensagem_erro = 0;

    int rc = sqlite3_open("database.sqlite", &db);

    if (rc != SQLITE_OK) 
    {
        printf("ERRO ao abrir: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char sql[100];
    sprintf(sql, "INSERT INTO usuarios (username, password) VALUES");

    return 0;
}

int main(void) 
{
    createUsuarios();
    createEmprestimos();
    inserirUsuario("usuario_teste", "senha123");
}
 