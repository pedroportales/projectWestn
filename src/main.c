#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sqlite3.h"

int sqlite3_retorno(void *NotUsed, int argc, char **argv, char **coluna) 
{ // funcao de Callback
    /*for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", coluna[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");*/
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
                    "username TEXT UNIQUE NOT NULL,"
                    "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, create, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_free(mensagem_erro);
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

    char *create = "CREATE TABLE IF NOT EXISTS emprestimo( "
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "usuario_id INTEGER,"
                    "valor REAL NOT NULL,"
                    "taxa_juros REAL NOT NULL,"
                    "meses INTEGER NOT NULL,"
                    "FOREIGN KEY (usuario_id) REFERENCES usuarios(id))";

    rc = sqlite3_exec(db, create, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_free(mensagem_erro);
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
    sprintf(sql, "INSERT INTO usuarios (username, password) VALUES (\"%s\", \"%s\")", username, password);

    rc = sqlite3_exec(db, sql, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_free(mensagem_erro);
    sqlite3_close(db);

    return 0;
}

int inserirEmprestimo(int usuario_id, float valor, float taxa_juros, int meses)
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
    sprintf(sql, "INSERT INTO emprestimo (usuario_id, valor, taxa_juros, meses) VALUES (%d, %f, %f, %d);", usuario_id, valor, taxa_juros, meses);

    rc = sqlite3_exec(db, sql, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_free(mensagem_erro);
    sqlite3_close(db);

    return 0;
}

int consultarUsuario()
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
    sprintf(sql, "SELECT * FROM usuarios;");

    rc = sqlite3_exec(db, sql, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_free(mensagem_erro);
    sqlite3_close(db);

    return 0;
}

int consultarEmprestimo()
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
    sprintf(sql, "SELECT * FROM emprestimo;");

    rc = sqlite3_exec(db, sql, sqlite3_retorno, 0, &mensagem_erro);

    sqlite3_free(mensagem_erro);
    sqlite3_close(db);

    return 0;
}

/*bool pesquisarEmUsuario( sqlite3 * p_db )
{
    sqlite3_stmt * handle_sql = 0;
    char comando_sql[] = "SELECT * FROM funcionario WHERE nascimento=?";

    int rc = sqlite3_prepare_v2( p_db, comando_sql, -1, &handle_sql, 0 );

    if ( rc != SQLITE_OK )
    {
        printf( "ERRO no prepare: %s\n", sqlite3_errmsg( p_db ) );
        return false;
    }

    rc = sqlite3_bind_text( handle_sql, 1, "14/11/1922", -1, NULL );

    if ( rc != SQLITE_OK )
    {
        printf( "ERRO no bind: %s\n", sqlite3_errmsg( p_db ) );
        return false;
    }

    rc = sqlite3_step( handle_sql );

    if ( rc == SQLITE_ROW)
    {
        // retornar os valores dos campos se precisar
        printf("id........: %s\n", sqlite3_column_text(handle_sql, 0 ) );
        printf("nome......: %s\n", sqlite3_column_text(handle_sql, 1 ) );
        printf("nascimento: %s\n", sqlite3_column_text(handle_sql, 2 ) );
    }

    sqlite3_free( handle_sql );
    return true;
}*/

int realizarLogin(char *username, char *password)
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

    char sql[200];
    sprintf(sql, "SELECT * FROM usuarios WHERE username = '%s';", username);

    rc = sqlite3_exec(db, sql, sqlite3_retorno, 0, &mensagem_erro);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", mensagem_erro);
        sqlite3_free(mensagem_erro);
        sqlite3_close(db);
        return 1;  // Indica falha no login
    }

    // Se chegamos aqui, o usuário existe, e agora verificamos a senha
    // (Este exemplo não inclui a verificação do sal para simplificar)
    sprintf(sql, "SELECT * FROM usuarios WHERE username = '%s' AND password = '%s';", username, password);
    rc = sqlite3_exec(db, sql, sqlite3_retorno, 0, &mensagem_erro);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", mensagem_erro);
        sqlite3_free(mensagem_erro);
        sqlite3_close(db);
        return 0;  // Indica falha no login
    }

    // Se chegamos aqui, as credenciais são válidas
    printf("Login bem-sucedido!\n");

    sqlite3_free(mensagem_erro);
    sqlite3_close(db);

    return 0;
}

int main(void) 
{
    createUsuarios();
    createEmprestimos();
    inserirUsuario("usuario_teste", "senha123");
    inserirEmprestimo(1, 1000.0, 12.75, 12);
    consultarUsuario();
    consultarEmprestimo();
    realizarLogin("usuario_teste", "senha123");
}
 