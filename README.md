# projectWestn
Cálculo de juros compostos, utilizando SQLite3.

Para compilar o projeto, use o seguinte comando dentro do diretório `src`:
```
gcc main.c sqlite3.c -lm
```

Após compilado, execute o arquivo `a.out` gerado. Caso esteja usando Windows, será necessária algumas alterações no código:
* Adicione no início do arquivo `#include <windows.h>`
* Troque os `system("clear")` por `system("cls")` na função `menu()`.
