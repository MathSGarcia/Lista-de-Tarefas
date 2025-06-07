# Lista-de-Tarefas
To-Do List CLI em C

Este é um pequeno sistema de lista de tarefas operado via terminal.
Ele permite autenticação de usuário e oferece todas as operações básicas de um CRUD (criar, ler, atualizar e deletar tarefas).

Como funciona
Ao executar o programa, o usuário passa por uma tela de login (com credenciais armazenadas no arquivo users.txt no formato usuario;senha).
Depois de autenticado, é exibido um menu onde se escolhe entre criar uma nova tarefa, listar as tarefas existentes, atualizar uma tarefa ou apagá-la.
Cada tarefa é armazenada em tasks.txt, num formato simples de texto: ID;descrição.

Autenticação: o arquivo users.txt é criado automaticamente na primeira execução. O sistema verifica as credenciais usando leitura de arquivo e comparações de string.

Armazenamento: cada tarefa recebe um ID sequencial e é gravada como ID;desc. IDs são calculados lendo o maior valor já existente e somando um.

CRUD completo: você pode criar, listar, atualizar e deletar tarefas. Atualizações e exclusões são feitas usando um arquivo temporário (tmp.txt), que substitui o original ao final da operação.

Requisitos
Basta um compilador C (como gcc) e um ambiente com acesso ao terminal.

Formato dos arquivos de dados

users.txt: cada linha é usuario;senha (por exemplo: matheus;123).

tasks.txt: cada linha é ID;descrição (por exemplo: 1;Comprar leite).
