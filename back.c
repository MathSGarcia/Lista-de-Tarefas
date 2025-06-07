#include <stdio.h>    // Inclui a biblioteca para funções de entrada e saída
#include <stdlib.h>   // Inclui funções para alocação de memória dinâmica e manipulação de arquivos
#include <string.h>   // Inclui funções para manipulação de strings, como strcmp, fgets, etc.

#define DATA_FILE   "tasks.txt"  // Define o nome do arquivo onde as tarefas serão armazenadas.
#define USERS_FILE  "users.txt"  // Define o nome do arquivo onde os usuários serão armazenados.
#define MAX_DESC    100          // Define o tamanho máximo da descrição de uma tarefa.
#define MAX_LINE    256          // Define o tamanho máximo de uma linha lida de um arquivo.
#define MAX_USER    32           // Define o tamanho máximo para o nome de usuário.
#define MAX_PASS    32           // Define o tamanho máximo para a senha do usuário.

/* Garante que users.txt exista (cria se não existir) */
void init_users_file() {
    FILE *f = fopen(USERS_FILE, "r");  // fopen tenta abrir o arquivo "users.txt" para leitura.
    if (f) {                           // Se o arquivo já existir.
        fclose(f);                     // Fecha o arquivo.
    } else {                           // Se o arquivo não existir.
        f = fopen(USERS_FILE, "a");    // Abre o arquivo para adicionar dados (modo anexar) (cria se não existir).
        if (!f) {                       // Se não conseguir abrir/criar o arquivo.
            perror("Erro ao criar users.txt"); // Exibe uma mensagem de erro.
            exit(EXIT_FAILURE);        // Encerra o programa com erro.
        }
        fclose(f);                     // Fecha o arquivo aberto com o fopen.
    }
}

/* Retorna 1 se usuário/senha conferem, 0 caso contrário */
int authenticate() {
    char username[MAX_USER];  // Declara uma variável para armazenar o nome de usuário.
    char password[MAX_PASS];  // Declara uma variável para armazenar a senha.

    printf("=== Login ===\n");
    printf("Usuário: ");
    scanf("%31s", username);  // Lê o nome do usuário (até 31 caracteres).

    printf("Senha: ");
    scanf("%31s", password);  // Lê a senha (até 31 caracteres).

    FILE *f = fopen(USERS_FILE, "r");  // Abre o arquivo "users.txt" para leitura.
    if (!f) {                         // Se não conseguir abrir o arquivo.
        perror("Não foi possível abrir users.txt");  // perror é a funcao da biblioteca tdlib.h para exibir uma mensagem de erro no console.
        return 0;                      // Retorna 0, indicando falha na autenticação.
    }

    char line[MAX_LINE];  // Declara uma variável para armazenar cada linha do arquivo.
    while (fgets(line, sizeof(line), f)) {  // Lê o arquivo linha por linha e armazena em um buffer.
        char *sep = strchr(line, ';');  // Procura o caractere ';' que separa o usuário e a senha.
        if (!sep) continue;  // Se não encontrar o caractere ';', pula para a próxima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o usuário da senha.
        char *file_user = line;  // Armazena o nome de usuário do arquivo.
        char *file_pass = sep + 1;  // Armazena a senha do arquivo.

        file_pass[strcspn(file_pass, "\r\n")] = '\0';  // Remove quebras de linha ou carrige return.

        if (strcmp(username, file_user) == 0 && strcmp(password, file_pass) == 0) {
            fclose(f);  // Fecha o arquivo se as credenciais forem corretas.
            return 1;    // Retorna 1, indicando sucesso na autenticação.
        }
    }

    fclose(f);  // Fecha o arquivo.
    return 0;    // Se não encontrar o usuário e senha corretos, retorna 0.
}

/* Retorna próximo ID sequencial */
int next_id() {
    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura.
    int max = 0;  // Inicializa o valor do ID máximo como 0.
    if (!f) return 1;  // Se não conseguir abrir o arquivo, retorna 1.
    char buf[MAX_LINE];  // Declara uma variável para armazenar cada linha do arquivo.
    while (fgets(buf, sizeof(buf), f)) {  // Lê o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descrição (a funcao procura na string o caracter especificado).
        if (!sep) continue;  // Se não encontrar o caractere ';', pula para a próxima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descrição.
        int id = atoi(buf);  // Converte a parte antes do ';' para um número inteiro.

        if (id > max) max = id;  // Se o ID for maior que o máximo encontrado, atualiza o máximo.
    }

    fclose(f);  // Fecha o arquivo.
    return max + 1;  // Retorna o próximo ID disponível, que é o maior ID encontrado + 1.
}

void create_task() {
    char desc[MAX_DESC];  // Declara uma variável para armazenar a descrição da tarefa.
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }  // Limpa o buffer de entrada.

    printf("Descrição da tarefa: ");
    if (!fgets(desc, sizeof(desc), stdin)) return;  // Lê a descrição da tarefa.
    desc[strcspn(desc, "\n")] = '\0';  // Remove a quebra de linha da descrição.

    int id = next_id();  // Obtém o próximo ID disponível.

    FILE *f = fopen(DATA_FILE, "a");  // Abre o arquivo "tasks.txt" para adicionar dados.
    if (!f) {  // Se não conseguir abrir o arquivo.
        perror("Erro ao abrir tasks.txt");
        return;  // Retorna, sem fazer nada.
    }

    fprintf(f, "%d;%s\n", id, desc);  // Grava a nova tarefa no arquivo com o ID e a descrição (similar ao printf, mas a saida vai para ao arquivo ao inves do console).
    fclose(f);  // Fecha o arquivo.

    printf("Tarefa %d criada!\n", id);  // Exibe uma mensagem confirmando a criação da tarefa.
}

void read_tasks() {
    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura.
    if (!f) {  // Se não conseguir abrir o arquivo.
        printf("Nenhuma tarefa cadastrada.\n");  // Exibe uma mensagem indicando que não há tarefas.
        return;  // Retorna, sem fazer nada.
    }

    printf("ID | Descrição\n");
    printf("---|--------------------------------\n");

    char buf[MAX_LINE];  // Declara uma variável para armazenar cada linha do arquivo.
    while (fgets(buf, sizeof(buf), f)) {  // Lê o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descrição.
        if (!sep) continue;  // Se não encontrar o caractere ';', pula para a próxima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descrição.
        int id = atoi(buf);  // Converte a parte antes do ';' para um número inteiro (a funcao converte a string de caracteres em int).
        char *desc = sep + 1;  // Armazena a descrição.

        desc[strcspn(desc, "\n")] = '\0';  // Remove a quebra de linha da descrição.
        printf("%2d | %s\n", id, desc);  // Exibe o ID e a descrição da tarefa.
    }

    fclose(f);  // Fecha o arquivo.
}

void update_task() {
    int id;
    printf("ID da tarefa a atualizar: ");
    if (scanf("%d", &id) != 1) return;  // Lê o ID da tarefa a ser atualizada.
    int c; while ((c = getchar()) != '\n' && c != EOF) { }  // Limpa o buffer de entrada.

    printf("Nova descrição: ");
    char new_desc[MAX_DESC];  // Declara uma variável para a nova descrição da tarefa.
    if (!fgets(new_desc, sizeof(new_desc), stdin)) return;  // Lê a nova descrição da tarefa.
    new_desc[strcspn(new_desc, "\n")] = '\0';  // Remove a quebra de linha da nova descrição (a funcao retorna o indice da primeira ocorrencia e substitui pelo parametro).

    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura.
    FILE *tmp = fopen("tmp.txt", "w");  // Cria um arquivo temporário para armazenar as alterações.
    if (!f || !tmp) {  // Se não conseguir abrir um dos arquivos.
        perror("Erro ao abrir tasks.txt");
        if (f)   fclose(f);
        if (tmp) fclose(tmp);
        return;
    }

    char buf[MAX_LINE];  // Declara uma variável para armazenar cada linha do arquivo.
    int found = 0;  // Variável para indicar se a tarefa foi encontrada e atualizada.
    while (fgets(buf, sizeof(buf), f)) {  // Lê o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descrição.
        if (!sep) continue;  // Se não encontrar o caractere ';', pula para a próxima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descrição.
        int cur = atoi(buf);  // Converte a parte antes do ';' para um número inteiro.
        if (cur == id) {  // Se o ID da tarefa for o que estamos procurando.
            fprintf(tmp, "%d;%s\n", id, new_desc);  // Atualiza a descrição da tarefa.
            found = 1;  // Marca que a tarefa foi encontrada e atualizada.
        } else {
            fprintf(tmp, "%d;%s", cur, sep + 1);  // Copia a linha original para o arquivo temporário.
        }
    }

    fclose(f);  // Fecha o arquivo original.
    fclose(tmp);  // Fecha o arquivo temporário.

    remove(DATA_FILE);  // Deleta o arquivo original.
    rename("tmp.txt", DATA_FILE);  // Renomeia o arquivo temporário para substituir o arquivo original.

    if (found)  // Se a tarefa foi encontrada e atualizada.
        printf("Tarefa %d atualizada!\n", id);  // Exibe uma mensagem de sucesso.
    else
        printf("Tarefa %d não encontrada.\n", id);  // Se a tarefa não foi encontrada, exibe uma mensagem de erro.
}

void delete_task() {
    int id;
    printf("ID da tarefa a deletar: ");
    if (scanf("%d", &id) != 1) return;  // Lê o ID da tarefa a ser deletada e armazena em 'id'.
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }  // Limpa o buffer de entrada para evitar problemas com scanf anterior.

    FILE *f = fopen(DATA_FILE, "r");  // Abre o arquivo "tasks.txt" para leitura de tarefas.
    FILE *tmp = fopen("tmp.txt", "w");  // Cria um arquivo temporário "tmp.txt" para salvar as tarefas sem a deletada.
    if (!f || !tmp) {  // Se não conseguir abrir o arquivo "tasks.txt" para leitura ou o arquivo temporário para escrita.
        perror("Erro ao abrir tasks.txt");  // Exibe uma mensagem de erro.
        if (f) fclose(f);  // Se o arquivo "tasks.txt" foi aberto, fecha-o.
        if (tmp) fclose(tmp);  // Se o arquivo temporário foi aberto, fecha-o.
        return;  // Retorna da função sem fazer nada, já que houve erro ao abrir os arquivos.
    }

    char buf[MAX_LINE];  // Declara uma variável para armazenar cada linha do arquivo.
    int found = 0;  // Variável para indicar se a tarefa foi encontrada e deletada.
    while (fgets(buf, sizeof(buf), f)) {  // Lê o arquivo linha por linha.
        char *sep = strchr(buf, ';');  // Procura o caractere ';' que separa o ID da descrição.
        if (!sep) continue;  // Se não encontrar o caractere ';', pula para a próxima linha.

        *sep = '\0';  // Coloca um terminador nulo no caractere ';', separando o ID da descrição.
        int cur = atoi(buf);  // Converte a parte antes do ';' para um número inteiro.
        if (cur == id) {  // Se o ID da tarefa for o que estamos procurando.
            found = 1;  // Marca que a tarefa foi encontrada e deletada.
            continue;  // Não copia a linha para o arquivo temporário (deletando a tarefa).
        }
        fprintf(tmp, "%d;%s", cur, sep + 1);  // Copia a linha original para o arquivo temporário.
    }

    fclose(f);  // Fecha o arquivo original.
    fclose(tmp);  // Fecha o arquivo temporário.

    remove(DATA_FILE);  // Deleta o arquivo original.
    rename("tmp.txt", DATA_FILE);  // Renomeia o arquivo temporário para substituir o arquivo original.

    if (found)  // Se a tarefa foi encontrada e deletada.
        printf("Tarefa %d deletada!\n", id);  // Exibe uma mensagem de sucesso.
    else
        printf("Tarefa %d não encontrada.\n", id);  // Se a tarefa não foi encontrada, exibe uma mensagem de erro.
}
