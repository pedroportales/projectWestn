-- CREATE DATABASE secao03;

/*CREATE TABLE tipos_produtos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    descricao TEXT NOT NULL
);

CREATE TABLE produtos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    descricao TEXT NOT NULL,
    preco REAL NOT NULL,
    id_tipo_produto INTEGER REFERENCES tipo_produtos(id) NOT NULL
);

CREATE TABLE pacientes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nome TEXT NOT NULL,
    endereco TEXT NOT NULL,
    bairro TEXT NOT NULL,
    cidade TEXT NOT NULL,
    estado TEXT NOT NULL,
    cep TEXT NOT NULL,
    data_nascimento TEXT NOT NULL
);

CREATE TABLE professor (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    telefone TEXT NOT NULL,
    nome TEXT NOT NULL
);

CREATE TABLE turma (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    capacidade INTEGER NOT NULL,
    id_professor INTEGER REFERENCES professor(id) NOT NULL
);*/

CREATE TABLE IF NOT EXISTS usuarios (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL
    password TEXT NOT NULL);

CREATE TABLE IF NOT EXISTS emprestimo(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    usuario_id INTEGER,
    valor REAL NOT NULL,
    taxa_juros REAL NOT NULL,
    meses INTEGER NOT NULL,
    FOREIGN KEY (usuario_id) REFERENCES usuarios(id));
