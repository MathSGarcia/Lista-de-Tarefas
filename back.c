#include <stdio.h>    // Inclui a biblioteca para fun��es de entrada e sa�da
#include <stdlib.h>   // Inclui fun��es para aloca��o de mem�ria din�mica e manipula��o de arquivos
#include <string.h>   // Inclui fun��es para manipula��o de strings, como strcmp, fgets, etc.

#define DATA_FILE   "tasks.txt"  // Define o nome do arquivo onde as tarefas ser�o armazenadas.
#define USERS_FILE  "users.txt"  // Define o nome do arquivo onde os usu�rios ser�o armazenados.
#define MAX_DESC    100          // Define o tamanho m�ximo da descri��o de uma tarefa.
#define MAX_LINE    256          // Define o tamanho m�ximo de uma linha lida de um arquivo.
#define MAX_USER    32           // Define o tamanho m�ximo para o nome de usu�rio.
#define MAX_PASS    32           // Define o tamanho m�ximo para a senha do usu�rio.

/* Garante que users.txt exista (cria se n�o existir) */
void init_users_file() {
    FILE *f = fopen(USERS_FILE, "r");  // fopen tenta abrir o arquivo "users.txt" para leitura.
    if (f) {                           // Se o arquivo j� existir.
        fclose(f);                     // Fecha o arquivo.
    } else {                           // Se o arquivo n�o existir.
        f = fopen(USERS_FILE, "a");    // Abre o arquivo para adicionar dados (modo anexar) (cria se n�o existir).
        if (!f) {                       // Se n�o conseguir abrir/criar o arquivo.
            perror("Erro ao criar users.txt"); // Exibe uma mensagem de erro.
            exit(EXIT_FAILURE);        // Encerra o programa com erro.
        }
        fclose(f);                     // Fecha o arquivo aberto com o fopen.
    }
}

/* Retorna 1 se usu�rio/senha conferem, 0 caso contr�rio */
int authenticate() {
    char username[MAX_USER];  // Declara uma vari�vel para armazenar o nome de usu�rio.
    char password[MAX_PASS];  // Declara uma vari�vel para armazenar a senha.

    printf("=== Login ===\n");
    printf("Usu�rio: ");
    scanf("%31s", username);  // L� o nome do usu�rio (at� 31 caracteres).

    printf("Senha: ");
    scanf("%31s", password);  // L� a senha (at� 31 caracteres).

    FILE *f = fopen(USERS_FILE, "r");  // Abre o arquivo "users.txt" para leitura.
    if (!f) {                         // Se n�o conseguir abrir o arquivo.
        perror("N�o foi poss�vel abrir users.txt");  // perror � a funcao da biblioteca tdlib.h para exibir uma mensagem de erro no console.
        return 0;                      // Retorna 0, indicando falha na autentica��o.
    }

    char line[MAX_LINE];  // Declara uma vari�vel para armazenar cada linha do arquivo.
    while (fgets(line, sizeof(line), f)) {  // L� o arquivo linha por linha e armazena em um buffer.
        char *sep = strchr(line, ';');  // Procura o caractere ';' que separa o usu�rio e a senha.
        if (!sep) continue;  // Se n�o encontrar o caractere ';', pula para a pr�xima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o usu�rio da senha.
        char *file_user = line;  // Armazena o nome de usu�rio do arquivo.
        char *file_pass = sep + 1;  // Armazena a senha do arquivo.

        file_pass[strcspn(file_pass, "\r\n")] = '\0';  // Remove quebras de linha ou carrige return.

        if (strcmp(username, file_user) == 0 && strcmp(password, file_pass) == 0) {
            fclose(f);  // Fecha o arquivo se as credenciais forem corretas.
            return 1;    // Retorna 1, indicando sucesso na autentica��o.
        }
    }

    fclose(f);  // Fecha o arquivo.
    return 0;    // Se n�o encontrar o usu�rio e senha corretos, retorna 0.
}

/* Retorna pr�ximo ID sequencial */
int next_id() {
    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura.
    int max = 0;  // Inicializa o valor do ID m�ximo como 0.
    if (!f) return 1;  // Se n�o conseguir abrir o arquivo, retorna 1.
    char buf[MAX_LINE];  // Declara uma vari�vel para armazenar cada linha do arquivo.
    while (fgets(buf, sizeof(buf), f)) {  // L� o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descri��o (a funcao procura na string o caracter especificado).
        if (!sep) continue;  // Se n�o encontrar o caractere ';', pula para a pr�xima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descri��o.
        int id = atoi(buf);  // Converte a parte antes do ';' para um n�mero inteiro.

        if (id > max) max = id;  // Se o ID for maior que o m�ximo encontrado, atualiza o m�ximo.
    }

    fclose(f);  // Fecha o arquivo.
    return max + 1;  // Retorna o pr�ximo ID dispon�vel, que � o maior ID encontrado + 1.
}

void create_task() {
    char desc[MAX_DESC];  // Declara uma vari�vel para armazenar a descri��o da tarefa.
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }  // Limpa o buffer de entrada.

    printf("Descri��o da tarefa: ");
    if (!fgets(desc, sizeof(desc), stdin)) return;  // L� a descri��o da tarefa.
    desc[strcspn(desc, "\n")] = '\0';  // Remove a quebra de linha da descri��o.

    int id = next_id();  // Obt�m o pr�ximo ID dispon�vel.

    FILE *f = fopen(DATA_FILE, "a");  // Abre o arquivo "tasks.txt" para adicionar dados.
    if (!f) {  // Se n�o conseguir abrir o arquivo.
        perror("Erro ao abrir tasks.txt");
        return;  // Retorna, sem fazer nada.
    }

    fprintf(f, "%d;%s\n", id, desc);  // Grava a nova tarefa no arquivo com o ID e a descri��o (similar ao printf, mas a saida vai para ao arquivo ao inves do console).
    fclose(f);  // Fecha o arquivo.

    printf("Tarefa %d criada!\n", id);  // Exibe uma mensagem confirmando a cria��o da tarefa.
}

void read_tasks() {
    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura.
    if (!f) {  // Se n�o conseguir abrir o arquivo.
        printf("Nenhuma tarefa cadastrada.\n");  // Exibe uma mensagem indicando que n�o h� tarefas.
        return;  // Retorna, sem fazer nada.
    }

    printf("ID | Descri��o\n");
    printf("---|--------------------------------\n");

    char buf[MAX_LINE];  // Declara uma vari�vel para armazenar cada linha do arquivo.
    while (fgets(buf, sizeof(buf), f)) {  // L� o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descri��o.
        if (!sep) continue;  // Se n�o encontrar o caractere ';', pula para a pr�xima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descri��o.
        int id = atoi(buf);  // Converte a parte antes do ';' para um n�mero inteiro (a funcao converte a string de caracteres em int).
        char *desc = sep + 1;  // Armazena a descri��o.

        desc[strcspn(desc, "\n")] = '\0';  // Remove a quebra de linha da descri��o.
        printf("%2d | %s\n", id, desc);  // Exibe o ID e a descri��o da tarefa.
    }

    fclose(f);  // Fecha o arquivo.
}

void update_task() {
    int id;
    printf("ID da tarefa a atualizar: ");
    if (scanf("%d", &id) != 1) return;  // L� o ID da tarefa a ser atualizada.
    int c; while ((c = getchar()) != '\n' && c != EOF) { }  // Limpa o buffer de entrada.

    printf("Nova descri��o: ");
    char new_desc[MAX_DESC];  // Declara uma vari�vel para a nova descri��o da tarefa.
    if (!fgets(new_desc, sizeof(new_desc), stdin)) return;  // L� a nova descri��o da tarefa.
    new_desc[strcspn(new_desc, "\n")] = '\0';  // Remove a quebra de linha da nova descri��o (a funcao retorna o indice da primeira ocorrencia e substitui pelo parametro).

    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura.
    FILE *tmp = fopen("tmp.txt", "w");  // Cria um arquivo tempor�rio para armazenar as altera��es.
    if (!f || !tmp) {  // Se n�o conseguir abrir um dos arquivos.
        perror("Erro ao abrir tasks.txt");
        if (f)   fclose(f);
        if (tmp) fclose(tmp);
        return;
    }

    char buf[MAX_LINE];  // Declara uma vari�vel para armazenar cada linha do arquivo.
    int found = 0;  // Vari�vel para indicar se a tarefa foi encontrada e atualizada.
    while (fgets(buf, sizeof(buf), f)) {  // L� o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descri��o.
        if (!sep) continue;  // Se n�o encontrar o caractere ';', pula para a pr�xima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descri��o.
        int cur = atoi(buf);  // Converte a parte antes do ';' para um n�mero inteiro.
        if (cur == id) {  // Se o ID da tarefa for o que estamos procurando.
            fprintf(tmp, "%d;%s\n", id, new_desc);  // Atualiza a descri��o da tarefa.
            found = 1;  // Marca que a tarefa foi encontrada e atualizada.
        } else {
            fprintf(tmp, "%d;%s", cur, sep + 1);  // Copia a linha original para o arquivo tempor�rio.
        }
    }

    fclose(f);  // Fecha o arquivo original.
    fclose(tmp);  // Fecha o arquivo tempor�rio.

    remove(DATA_FILE);  // Deleta o arquivo original.
    rename("tmp.txt", DATA_FILE);  // Renomeia o arquivo tempor�rio para substituir o arquivo original.

    if (found)  // Se a tarefa foi encontrada e atualizada.
        printf("Tarefa %d atualizada!\n", id);  // Exibe uma mensagem de sucesso.
    else
        printf("Tarefa %d n�o encontrada.\n", id);  // Se a tarefa n�o foi encontrada, exibe uma mensagem de erro.
}

void delete_task() {
    int id;
    printf("ID da tarefa a deletar: ");
    if (scanf("%d", &id) != 1) return;  // L� o ID da tarefa a ser deletada e armazena em 'id'.
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }  // Limpa o buffer de entrada para evitar problemas com scanf anterior.

    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura de tarefas.
    FILE *tmp = fopen("tmp.txt", "w");  // Cria um arquivo tempor�rio "tmp.txt" para salvar as tarefas sem a deletada.
    if (!f || !tmp) {  // Se n�o conseguir abrir o arquivo "tasks.txt" para leitura ou o arquivo tempor�rio para escrita.
        perror("Erro ao abrir tasks.txt");  // Exibe uma mensagem de erro.
        if (f) fclose(f);  // Se o arquivo "tasks.txt" foi aberto, fecha-o.
        if (tmp) fclose(tmp);  // Se o arquivo tempor�rio foi aberto, fecha-o.
        return;  // Retorna da fun��o sem fazer nada, j� que houve erro ao abrir os arquivos.
    }

    char buf[MAX_LINE];  // Declara uma vari�vel para armazenar cada linha do arquivo.
    int found = 0;  // Vari�vel para indicar se a tarefa foi encontrada e deletada.
    while (fgets(buf, sizeof(buf), f)) {  // L� o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descri��o.
        if (!sep) continue;  // Se n�o encontrar o caractere ';', pula para a pr�xima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descri��o.
        int cur = atoi(buf);  // Converte a parte antes do ';' para um n�mero inteiro.
        if (cur == id) {  // Se o ID da tarefa for o que estamos procurando.
            found = 1;  // Marca que a tarefa foi encontrada e deletada.
            continue;  // N�o copia a linha para o arquivo tempor�rio (deletando a tarefa).
        }
        fprintf(tmp, "%d;%s", cur, sep + 1);  // Copia a linha original para o arquivo tempor�rio.
    }

    fclose(f);  // Fecha o arquivo original.
    fclose(tmp);  // Fecha o arquivo tempor�rio.

    remove(DATA_FILE);  // Deleta o arquivo original.
    rename("tmp.txt", DATA_FILE);  // Renomeia o arquivo tempor�rio para substituir o arquivo original.

    if (found)  // Se a tarefa foi encontrada e deletada.
        printf("Tarefa %d deletada!\n", id);  // Exibe uma mensagem de sucesso.
    else
        printf("Tarefa %d n�o encontrada.\n", id);  // Se a tarefa n�o foi encontrada, exibe uma mensagem de erro.
}
