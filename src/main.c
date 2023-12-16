#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
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

float calcularJurosCompostos(float principal, float taxaJuros, int periodos) {
    // Converte a taxa de juros para decimal
    taxaJuros /= 100.0;

    // Calcula a prestação usando a fórmula de amortização
    float prestacao = (principal * taxaJuros * pow(1 + taxaJuros, periodos)) /
                       (pow(1 + taxaJuros, periodos) - 1);

    return prestacao;
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

int consultarUsuario(int usuarioID)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM usuarios WHERE id = ?;";
    int rc;

    rc = sqlite3_open("database.sqlite", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Substitui o marcador de posição ? pelo ID do usuário
    rc = sqlite3_bind_int(stmt, 1, usuarioID);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao fazer bind dos parâmetros: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    // Executa a consulta
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Processa a linha do resultado
        int id = sqlite3_column_int(stmt, 0);
        const char *username = (const char *)sqlite3_column_text(stmt, 1);

        printf("ID: %d, Usuário: %s\n", id, username);
    } else {
        fprintf(stderr, "Usuário não encontrado.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void consultarEmprestimosUsuario(int usuarioID) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM emprestimo WHERE usuario_id = ?;";
    int rc;

    rc = sqlite3_open("database.sqlite", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Substitui o marcador de posição ? pelo ID do usuário
    rc = sqlite3_bind_int(stmt, 1, usuarioID);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao fazer bind dos parâmetros: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    // Executa a consulta
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Processa cada linha do resultado
        int emprestimoID = sqlite3_column_int(stmt, 0);
        double valorEmprestado = sqlite3_column_double(stmt, 2);
        double taxaJuros = sqlite3_column_double(stmt, 3);
        int numeroPrestacoes = sqlite3_column_int(stmt, 4);

        printf("\nEmpréstimo ID: %d\nValor: %.2f\nTaxa de Juros: %.2f%%\nNúmero de Prestações: %d\n",
               emprestimoID, valorEmprestado, taxaJuros, numeroPrestacoes);

        double valorPrestacao = calcularJurosCompostos(valorEmprestado, taxaJuros, numeroPrestacoes);
        
        if (!isnan(valorPrestacao))
            {
                printf("Valor da prestação: %.2f\n", valorPrestacao);
                printf("Valor total: %.2f\n", valorPrestacao * numeroPrestacoes);
            }
            else 
            {
                printf("Erro: Valor da prestacao indefinido.\n");
            }
        
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
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
    int opt = 0, prestacoes;
    double valor, taxa_juros = 11.75;

    while(opt != 4)
    {
        printf("\n-----MENU-----\n");
        printf("1. Simular empréstimo\n");
        printf("2. Histórico empréstimos\n");
        printf("3. Consultar usuário\n");
        printf("4. Sair do sistema...\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            system("clear");
            printf("Insira o valor do empréstimo: ");
            scanf("%lf", &valor);
            printf("Insira em quantos meses deseja pagar: ");
            scanf("%d", &prestacoes);
            inserirEmprestimo(usuarioLogadoID, valor, taxa_juros, prestacoes);
            break;
        case 2:
            system("clear");
            consultarEmprestimosUsuario(usuarioLogadoID);
            break;
        case 3:
            system("clear");
            consultarUsuario(usuarioLogadoID);
            break;
        case 4:
            system("clear");
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
    createUsuarios();
    createEmprestimos();
    inserirUsuario("usuario_teste", "senha123");
    while (telaLogin() != 1)
    {
        system("clear");
        printf("Falha no login. Usuário não encontrado ou senha incorreta.\n");
        system("sleep 3");
    }

    menu();
    
}
 