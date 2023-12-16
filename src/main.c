#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "sqlite3.h"

int usuarioLogadoID = -1;

int sqlite3_retorno(void *NotUsed, int argc, char **argv, char **coluna) 
{ // funcao de Callback
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", coluna[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    
    return 0;
}

int false_callback()
{
    return 0;
}

static int verificaRegistroCallback(void *data, int argc, char **argv, char **azColName)
{
    int *temRegistro = (int *)data;
    *temRegistro = 1; // Sinaliza que há pelo menos um registro
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

    rc = sqlite3_exec(db, sql, false_callback, 0, &mensagem_erro);

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

// Função para verificar se há registros na tabela de usuários
void verificarUsuarios(char *username, char *password) 
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;
    int rc;

    //int temRegistro = 0; // Variável para sinalizar se há pelo menos um registro

    rc = sqlite3_open("database.sqlite", &db);

    if (rc) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Consultar se há algum registro na tabela de usuários
    /*char sql[200];
    sprintf(sql, "SELECT 1 FROM usuarios WHERE username = '%s' AND password = '%s' LIMIT 1;", username, password);
    
    rc = sqlite3_exec(db, sql, verificaRegistroCallback, &temRegistro, &zErrMsg);*/

    char sql[200];
    sprintf(sql, "SELECT id FROM usuarios WHERE username = ? AND password = ?;");

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Bind dos parâmetros
    rc = sqlite3_bind_text(stmt, 1, username, strlen(username), SQLITE_STATIC);
    rc = sqlite3_bind_text(stmt, 2, password, strlen(password), SQLITE_STATIC);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao fazer bind dos parâmetros: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    // Executar a consulta
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        // Obtém o ID do usuário diretamente usando sqlite3_column_int
        usuarioLogadoID = sqlite3_column_int(stmt, 0);
    }

    // Finalizar o statement
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Função para desativar o modo de eco do terminal
void desativarEcoTerminal() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Função para reativar o modo de eco do terminal
void reativarEcoTerminal() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void menu()
{
    int opt = 0;

    system("clear");

    while(opt != 5)
    {
        printf("\n-----MENU-----\n");
        printf("1. Pedir empréstimo\n");
        printf("2. Consultar empréstimo\n");
        printf("3. Consultar usuário\n");
        printf("4. Listar Prestações\n");
        printf("5. Sair do sistema...\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            int pedirEmprestimo();
            break;
        case 2:
            consultarEmprestimo();
            break;
        case 3:
            // code
            break;
        case 4:
            // code
            break;
        case 5:
            printf("Finalizando...\n");
            break;
        default:
            break;
        }
    }
}

int telaLogin()
{
    char user[50], passwd[50];

    setbuf(stdin, NULL);
    printf("Insira seu usuário: ");
    fgets(user, sizeof(user), stdin);
    printf("Insira sua senha: ");
    desativarEcoTerminal();
    fgets(passwd, sizeof(passwd), stdin);
    reativarEcoTerminal();

    user[strcspn(user, "\n")] = 0;
    passwd[strcspn(passwd, "\n")] = 0;

    verificarUsuarios(user, passwd);

    if (usuarioLogadoID != -1)
    {
        printf("Login bem-sucedido. ID do usuário logado: %d\n", usuarioLogadoID);
        return 1;
    } else {
        printf("%d\n", usuarioLogadoID);
        return 0;
    }

}

int main(void) 
{
    while (telaLogin() != 1)
    {
        system("clear");
        printf("Falha no login. Usuário não encontrado ou senha incorreta.\n");
        system("sleep 3");
    }

    menu();
    
}
 