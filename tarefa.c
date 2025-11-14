#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "tarefa.h"

#define separator "----------------------------------"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int getTaskCount(FILE *fp) {
    if (fp == NULL) {
        printf("Erro: arquivo não foi aberto corretamente.\n");
        return 0;
    }

    fseek(fp, 0, SEEK_SET);

    int count = 0;
    Tarefa tarefa;

    while (fread(&tarefa, sizeof(Tarefa), 1, fp) == 1) {
        count++;
    }

    fseek(fp, 0, SEEK_SET); // reposiciona o ponteiro

    return count;
}

int getPendingTaskCount(FILE *fp) {
    if (fp == NULL) {
        printf("Erro: arquivo não foi aberto corretamente.\n");
        return 0;
    }

    fseek(fp, 0, SEEK_SET);

    int count = 0;
    Tarefa tarefa;

    while (fread(&tarefa, sizeof(Tarefa), 1, fp) == 1) {
        if (tarefa.status == 'P') {
            count++;
        }
    }

    fseek(fp, 0, SEEK_SET); // reposiciona o ponteiro

    return count;
}


void registerNewTask(FILE *fp) {
    if (fp == NULL) {
        printf("Erro: arquivo não foi aberto corretamente.\n");
        return;
    }

    Tarefa novaTarefa;
    char buffer[50]; // buffer temporário para leitura da data

    novaTarefa.id = getTaskCount(fp) + 1;
    novaTarefa.status = 'P';  // Pendente por padrão

    printf("Digite a descrição da nova tarefa: ");
    fgets(novaTarefa.description, sizeof(novaTarefa.description), stdin);

    // remove o \n final da descrição, se existir
    size_t len = strlen(novaTarefa.description);
    if (len > 0 && novaTarefa.description[len - 1] == '\n') {
        novaTarefa.description[len - 1] = '\0';
    }

    printf("Digite a data de vencimento (dd mm aaaa): ");
    fgets(buffer, sizeof(buffer), stdin);  // lê a linha completa da data
    sscanf(buffer, "%d %d %d", &novaTarefa.expirationDay,
                              &novaTarefa.expirationMonth,
                              &novaTarefa.expirationYear);

    // grava no arquivo
    fseek(fp, 0, SEEK_END);
    fwrite(&novaTarefa, sizeof(Tarefa), 1, fp);
    fflush(fp);

    printf("\nTarefa cadastrada com sucesso!\n");
    printf("%s\n", separator);
}

void listAllActions(FILE *fp) {
    if (fp == NULL) {
        printf("Erro: arquivo não foi aberto corretamente.\n");
        return;
    }

    fseek(fp, 0, SEEK_SET);

    int tasksRead = 0;
    int totalExibidas = 0;
    Tarefa tarefa[10];

    while ((tasksRead = fread(tarefa, sizeof(Tarefa), 10, fp)) != 0) {
        for (int i = 0; i < tasksRead; i++) {
            if (totalExibidas > 0) {
                printf("%s\n", separator);
            }

            printf("Tarefa %d: %s\n", tarefa[i].id, tarefa[i].description);

            switch (tarefa[i].status) {
                case 'P':
                    printf("Status: Pendente\n");
                    break;
                case 'F':
                    printf("Status: Feito\n");
                    break;
                default:
                    printf("Status: Desconhecido\n");
                    break;
            }

            printf("Data de Vencimento: %02d/%02d/%04d\n",
                   tarefa[i].expirationDay,
                   tarefa[i].expirationMonth,
                   tarefa[i].expirationYear);

            totalExibidas++;
        }
    }

    fseek(fp, 0, SEEK_SET);
    limparBuffer();
   
}


void listNotDoneActions(FILE *fp) {
    if (!fp) {
        printf("Erro: arquivo não foi aberto corretamente.\n");
        return;
    }
    
    int count = getPendingTaskCount(fp);
    
    if (count == 0) {
        printf("Nenhuma tarefa pendente encontrada.\n");
        return;
    }

    fseek(fp, 0, SEEK_SET);

    Tarefa t;

    Tarefa *tarefas = (Tarefa*) malloc(sizeof(Tarefa) * count);

    int indexer = 0;
    while (fread(&t, sizeof(Tarefa), 1, fp) == 1) {
        if (t.status != 'P') continue;  // apenas pendentes

        tarefas[indexer] = t;
        indexer++;
    }

    for(int i = 0; i < count; i++) {
        if (i > 0) printf("%s\n", separator);
        printf("Tarefa %d: %s\n", tarefas[i].id, tarefas[i].description);
        printf("Status: Pendente\n");
        printf("Data de Vencimento: %02d/%02d/%04d\n",
               tarefas[i].expirationDay, tarefas[i].expirationMonth, tarefas[i].expirationYear);
    }


    fseek(fp, 0, SEEK_SET); // deixa o ponteiro no início

    limparBuffer();
}




char renderMenu(FILE *fp) {
    char buffer[10];
    char taskCountText[30];
    sprintf(taskCountText, "Existem %d tarefas.", getTaskCount(fp));

    printf("%s\n", separator);
    printf("- %-30s -\n", taskCountText);
    printf("- %-30s -\n", "Escolha uma ação:");
    printf("- %-30s -\n", "");
    printf("- %-30s -\n", "1) Listar todas as tarefas");
    printf("- %-30s -\n", "2) Listar tarefas não feitas");
    printf("- %-30s -\n", "3) Cadastrar tarefa");
    printf("- %-30s -\n", "4) Alterar status da tarefa");
    printf("- %-30s -\n", "5) Modificar tarefa");
    printf("- %-30s -\n", "6) Excluir tarefa");
    printf("- %-30s -\n", "0) Sair");
    printf("%s\n", separator);
    printf("Escolha uma opção: ");

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        return buffer[0];  // retorna apenas o primeiro caractere digitado
    }
    return '0';
}



/* Processador de ações */
int processAction(char action, FILE **fp) {
    switch (action) {

        case '1': listAllActions(*fp); 
        break;

        case '2': listNotDoneActions(*fp); 
        break;

        case '3': registerNewTask(*fp); 
        break;

        case '4': modifyTaskState(fp); 
        break;

        case '5': updateTask(fp); 
        break;

        case '6': removeTask(fp); 
        break;

        case '0': return -1;
        default: printf("Ação inválida.\n"); break;
    }
    return 0;
}

/* Menu recursivo */
void menuRecursivo(FILE *fp) {
    char action = renderMenu(fp);
    int result = processAction(action, &fp);

    if (result != -1) {
        menuRecursivo(fp); // chamada recursiva
    } else {
        printf("Encerrando o programa...\n");
    }
}


void removeTask(FILE **fp) {
    char buffer[50];
    int idRemover;
    int encontrada = 0;
    Tarefa tarefa;
    FILE *temp;

    printf("Digite o ID da tarefa que deseja excluir: ");
    fgets(buffer, sizeof(buffer), stdin);       // lê linha inteira
    sscanf(buffer, "%d", &idRemover);           // converte para inteiro

    fseek(*fp, 0, SEEK_SET);

    temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro: falha ao criar arquivo temporário.\n");
        return;
    }

    // Copia todas as tarefas, exceto a que será removida
    while (fread(&tarefa, sizeof(Tarefa), 1, *fp) == 1) {
        if (tarefa.id != idRemover) {
            fwrite(&tarefa, sizeof(Tarefa), 1, temp);
        } else {
            encontrada = 1;
        }
    }

    fclose(*fp);
    fclose(temp);

    // Backup e substituição
    remove("backup.bin");
    if (rename("tarefas.bin", "backup.bin") != 0) {
        printf("Erro ao criar backup do arquivo original.\n");
        remove("temp.bin");
        return;
    }

    if (rename("temp.bin", "tarefas.bin") != 0) {
        printf("Erro ao substituir arquivo original.\n");
        rename("backup.bin", "tarefas.bin");
        remove("temp.bin");
        return;
    }

    remove("backup.bin");

    if (encontrada)
        printf("Tarefa %d removida com sucesso!\n", idRemover);
    else
        printf("Tarefa não encontrada.\n");

    

    // 🔄 Reabre o arquivo atualizado e atualiza o ponteiro principal
    *fp = fopen("tarefas.bin", "r+b");
    if (!*fp) {
        printf("Erro ao reabrir o arquivo após exclusão.\n");
        return;
    }
}




void modifyTaskState(FILE **fp) {
    char buffer[50];
    int idAlvo;
    char novoEstado;
    int encontrada = 0;
    Tarefa tarefa;
    FILE *temp;

    printf("Digite o ID da tarefa que deseja alterar o estado: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &idAlvo);

    printf("Digite o novo estado ('F' para feito, 'P' para pendente): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, " %c", &novoEstado);

    // Garante leitura desde o início
    fseek(*fp, 0, SEEK_SET);

    // Abre arquivo temporário
    temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro: não foi possível criar arquivo temporário.\n");
        return;
    }

    // Lê todas as tarefas e atualiza a desejada
    while (fread(&tarefa, sizeof(Tarefa), 1, *fp) == 1) {
        if (tarefa.id == idAlvo) {
            tarefa.status = novoEstado;
            encontrada = 1;
        }
        fwrite(&tarefa, sizeof(Tarefa), 1, temp);
    }

    fclose(*fp);
    fclose(temp);

    // Proteção contra perda de dados
    remove("backup.bin");
    if (rename("tarefas.bin", "backup.bin") != 0) {
        printf("Erro ao criar backup do arquivo original.\n");
        remove("temp.bin");
        return;
    }

    if (rename("temp.bin", "tarefas.bin") != 0) {
        printf("Erro ao modificar o estado da tarefa.\n");
        rename("backup.bin", "tarefas.bin"); // restaura backup
        remove("temp.bin");
        return;
    }

    remove("backup.bin");

    if (encontrada)
        printf("Estado da tarefa %d alterado com sucesso!\n", idAlvo);
    else
        printf("Tarefa não encontrada.\n");

    

    // Reabre o arquivo atualizado e atualiza o ponteiro no main
    *fp = fopen("tarefas.bin", "r+b");
    if (!*fp) {
        printf("Erro ao reabrir arquivo após alteração.\n");
        return;
    }
}



void updateTask(FILE **fp) {
    char buffer[100];
    int idAlvo;
    int encontrada = 0;
    Tarefa tarefa;
    FILE *temp;

    printf("Digite o ID da tarefa que deseja atualizar: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &idAlvo);

    fseek(*fp, 0, SEEK_SET); // começa do início
    temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro: não foi possível criar arquivo temporário.\n");
        return;
    }

    while (fread(&tarefa, sizeof(Tarefa), 1, *fp) == 1) {
        if (tarefa.id == idAlvo) {
            encontrada = 1;

            printf("\nTarefa encontrada!\n");
            printf("Descrição atual: %s\n", tarefa.description);
            printf("Status atual: %c\n", tarefa.status);
            printf("Vencimento atual: %02d/%02d/%04d\n",
                   tarefa.expirationDay,
                   tarefa.expirationMonth,
                   tarefa.expirationYear);

            int opcao;
            do {
                printf("\nO que deseja alterar?\n");
                printf("1) Descrição\n");
                printf("2) Status\n");
                printf("3) Data de vencimento\n");
                printf("0) Finalizar alterações\n");
                printf("Escolha: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &opcao);

                switch (opcao) {
                    case 1:
                        printf("Nova descrição: ");
                        fgets(tarefa.description, sizeof(tarefa.description), stdin);
                        tarefa.description[strcspn(tarefa.description, "\n")] = '\0';
                        break;

                    case 2:
                        do {
                            printf("Novo status ('F' - Feita, 'P' - Pendente): ");
                            fgets(buffer, sizeof(buffer), stdin);
                            sscanf(buffer, " %c", &tarefa.status);

                            if (tarefa.status != 'F' && tarefa.status != 'P')
                                printf("Status inválido! Use apenas 'F' ou 'P'.\n");
                        } while (tarefa.status != 'F' && tarefa.status != 'P');
                        break;

                    case 3:
                        printf("Nova data de vencimento (dd mm aaaa): ");
                        fgets(buffer, sizeof(buffer), stdin);
                        sscanf(buffer, "%d %d %d",
                               &tarefa.expirationDay,
                               &tarefa.expirationMonth,
                               &tarefa.expirationYear);
                        break;

                    case 0:
                        printf("Alterações concluídas.\n");
                        break;

                    default:
                        printf("Opção inválida.\n");
                        break;
                }
            } while (opcao != 0);
        }

        fwrite(&tarefa, sizeof(Tarefa), 1, temp);
    }

    fclose(*fp);
    fclose(temp);

    // Backup e substituição segura
    remove("backup.bin");

    if (rename("tarefas.bin", "backup.bin") != 0) {
        printf("Erro ao criar backup do arquivo original.\n");
        remove("temp.bin");
        return;
    }

    if (rename("temp.bin", "tarefas.bin") != 0) {
        printf("Erro ao atualizar tarefa.\n");
        rename("backup.bin", "tarefas.bin");
        remove("temp.bin");
        return;
    }

    remove("backup.bin");

    if (encontrada)
        printf("\nTarefa %d atualizada com sucesso!\n", idAlvo);
    else
        printf("\nTarefa não encontrada.\n");

    

    // Reabre o arquivo atualizado para o programa continuar
    *fp = fopen("tarefas.bin", "r+b");
    if (!*fp) {
        printf("Erro ao reabrir o arquivo após atualização.\n");
        return;
    }
}


