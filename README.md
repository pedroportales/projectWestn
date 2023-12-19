# projectWestn
Cálculo de juros compostos, utilizando SQLite3.

Para compilar o projeto, use o seguinte comando dentro do diretório `src`:
```
gcc main.c sqlite3.c -lm
```

Após compilado, execute o arquivo `a.out` gerado. Caso esteja usando Windows, será necessária algumas alterações no código:
* Adicione no início do arquivo `#include <windows.h>`
* Troque os `system("clear")` por `system("cls")` na função `menu()`.
* A execução será de um arquivo `.exe` após compilado.

## Testando via navegador
Para abrir um ambiente no navegador, e testar o sistema pelo mesmo, basta acessar o seguinte link: [GitPod](https://gitpod.io/#/github.com/pedroportales/projectWestn)

* Após acessar, entre com sua conta do GitHub, e crie o ambiente
* Abra o terminal do ambiente teste, e execute o comando `cd src`
* Realizados os passos acima, compile normalmente usando o GCC, como recomendado no início deste README.
* Dessa forma, não será necessária alteração do código, visto que o ambiente é Linux.
