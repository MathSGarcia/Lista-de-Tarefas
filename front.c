#include <stdio.h> // Inclui a biblioteca para funções de entrada e saída
#include "back.c"  // Inclui o arquivo back.c que contem a lógica do programa

int main() {
    /* inicializa arquivo de usuários atraves da funcao do back*/
    init_users_file();

    /* autenticação */
    if (!authenticate()) {
        printf("Falha na autenticação. Saindo.\n");
        return 1;
    }

    /* loop principal do menu CRUD */
    /* exibe o menu e lê a opção do usuário
       chama a função correspondente com base na opção */
    int opc;
    do {
        printf("\n=== To-Do List ===\n");
        printf("1. Criar tarefa\n");
        printf("2. Listar tarefas\n");
        printf("3. Atualizar tarefa\n");
        printf("4. Deletar tarefa\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        if(scanf("%d", &opc) != 1) break; // scanf retorna 1 quando le um inteiro corretamente, se o usuario inserir um simbolo invalido ele retorna 0 e encerra o programa

        switch (opc) {
            case 1: create_task();  break;
            case 2: read_tasks();   break;
            case 3: update_task();  break;
            case 4: delete_task();  break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n"); break;
        }
    } while (opc != 0); // enquanto for diferente de 0 o loop continua

    return 0;
}
