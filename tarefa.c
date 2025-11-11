#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "tarefa.h"

#define separator "----------------------------------"

int getTaskCount(FILE *fp) {
    fseek(fp, 0, SEEK_SET);

    int count = 0;

    Tarefa tarefa;
    while(fread(&tarefa, sizeof(Tarefa), 1, fp)) {
        count++;
    }

    return count;
}

void registerNewTask(FILE *fp) {
    Tarefa novaTarefa;

    novaTarefa.id = getTaskCount(fp) + 1;
    novaTarefa.status = 'P';  // Pendente por padrão

    printf("Digite a descrição da nova tarefa: ");
    fgets(novaTarefa.description, sizeof(novaTarefa.description), stdin);

    size_t len = strlen(novaTarefa.description);
    if (len > 0 && novaTarefa.description[len - 1] == '\n') {
        novaTarefa.description[len - 1] = '\0'; // Troca o \n se existir por um \0
    }

    printf("Digite a data de vencimento (dd mm aaaa): ");
    scanf("%d %d %d", &novaTarefa.expirationDay, &novaTarefa.expirationMonth, &novaTarefa.expirationYear);

    fseek(fp, 0, SEEK_END);
    fwrite(&novaTarefa, sizeof(Tarefa), 1, fp);

    fflush(fp);
}

void listAllActions(FILE *fp) {
    fseek(fp, 0, SEEK_SET);

    int tasksRead = 0;
    Tarefa tarefa[10];
    while((tasksRead = fread(&tarefa, sizeof(Tarefa), 10, fp)) != 0) {
        for(int i = 0; i < tasksRead; i++) {
            if(i != 0) {
                printf("%s\n", separator);
            }
            printf("Tarefa %d: %s\n", tarefa[i].id, tarefa[i].description);
            switch(tarefa[i].status) {
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
            
        }
    }
    system("pause");
}

void listNotDoneActions(FILE *fp) {
    fseek(fp, 0, SEEK_SET);

    int tasksRead = 0;
    Tarefa tarefa[10];
    while((tasksRead = fread(&tarefa, sizeof(Tarefa), 10, fp)) != 0) {
        printf("Tarefas lidas: %d\n", tasksRead);
        for(int i = 0; i < tasksRead; i++) {
            if(tarefa[i].status == 'F') {
                continue;
            }

            if(i != 0) {
                printf("%s\n", separator);
            }

            printf("Tarefa %d: %s\n", tarefa[i].id, tarefa[i].description);
             switch(tarefa[i].status) {
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
            
        }
    }
    system("pause");
}



void renderMenu(FILE *fp) {
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
}

int processAction(char action, FILE *fp) {
    switch(action) {
        case '1': 
            listAllActions(fp);
            break;
        case '2': 
            listNotDoneActions(fp);
            break;
        case '3': 
            registerNewTask(fp);
            break;
        case '4': 
            void modifyTaskState(FILE *fp);
            break;
        case '5': 
            void updateTask(FILE *fp);
            break;
        case '6': 
            removeTask(fp);
            break;        
        case '0': 
            return -1;
        default: 
            printf("Ação selecionada inválida.\n");
            break;
    }

    return 0;
}

void removeTask(FILE *fp) {
    int idRemover;
    int encontrada = 0;
    Tarefa tarefa;

    FILE *temp;

    // pede o ID
    printf("Digite o ID da tarefa que deseja excluir: ");
    scanf("%d", &idRemover);
    getchar(); // limpa o buffer

    // abre os arquivos
    fp = fopen("tarefas.bin", "rb");
    if (!fp) {
        printf("Erro: não foi possível abrir o arquivo de tarefas.\n");
        return;
    }

    temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro: Falha na tentativa de exclusão. Verifique se há espaço em disco ou permissões de escrita.\n");
        fclose(fp);
        return;
    }

    // lê cada tarefa e copia apenas as que não têm o ID a ser removido
    while (fread(&tarefa, sizeof(Tarefa), 1, fp)) {
        if (tarefa.id != idRemover) {
            fwrite(&tarefa, sizeof(Tarefa), 1, temp);
        } else {
            encontrada = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    // Proteção contra perda de dados:

    // remove backup antigo (caso exista) e cria um novo
    remove("backup.bin"); 

    // 1. renomeia o arquivo original para backup
    if (rename("tarefas.bin", "backup.bin") != 0) {
        printf("Erro ao criar backup do arquivo original.\n");
        remove("temp.bin");
        return;
    }

    // 2. renomeia o temporário para o nome do original
    if (rename("temp.bin", "tarefas.bin") != 0) {
        printf("Erro ao remover tarefa.\n");
        // tenta restaurar o backup
        rename("backup.bin", "tarefas.bin");
        remove("temp.bin");
        return;
    }

    // 3. Se tudo deu certo → remove o backup
    remove("backup.bin");

    if (encontrada)
        printf("Tarefa %d removida com sucesso!\n", idRemover);
    else
        printf("Tarefa não encontrada.\n");
}


void modifyTaskState(FILE *fp) {
    int idAlvo;
    char novoEstado;
    int encontrada = 0;
    Tarefa tarefa;

    FILE *temp;

    printf("Digite o id da Tarefa que deseja alterar o Estado: ");
    scanf("%d", &idAlvo);
    getchar();

    printf("Digite o novo Estado ('F' para feito, 'P' para pendente): \n");
    scanf("%c", &novoEstado);

    //abre os dois arquivos original e temp

    fp = fopen("tarefas.bin" , "rb");
    if(fp == NULL){
        printf("ERRO: não foi possivel abrir o arquivo de tarefas\n");
        return;
    }

    temp = fopen("temp.bin", "wb");
    if(temp == NULL){
        printf("Erro: não foi possível modificar estado da tarefa.\n");
        fclose(fp);
        return;
    }
    //procura a tarefa cujo estado deve ser modificado
    while(fread(&tarefa, sizeof(Tarefa), 1, fp)){
        if(tarefa.id == idAlvo){
            tarefa.status = novoEstado;
            encontrada = 1;
        }
    // Copia todas as tarefas para o arquivo temporário, atualizando a que teve o estado alterado
        fwrite(&tarefa, sizeof(Tarefa), 1,temp);
    }
    fclose(fp);
    fclose(temp);

    // Proteção contra perda de dados:

    // remove backup antigo (caso exista) e cria um novo
    remove("backup.bin"); 

    // 1. renomeia o arquivo original para backup
    if (rename("tarefas.bin", "backup.bin") != 0) {
        printf("Erro ao criar backup do arquivo original.\n");
        remove("temp.bin");
        return;
    }

    // 2. renomeia o temporário para o nome do original
    if (rename("temp.bin", "tarefas.bin") != 0) {
        printf("Erro ao modificar o estado da tarefa.\n");
        // tenta restaurar o backup
        rename("backup.bin", "tarefas.bin");
        remove("temp.bin");
        return;
    }

    // 3. Se tudo deu certo → remove o backup
    remove("backup.bin");

    if (encontrada)
        printf("Estado da Tarefa %d alterado com sucesso!\n", idAlvo);
    else
        printf("Tarefa não encontrada.\n");
}


void updateTask(FILE *fp) {
    int idAlvo;
    int encontrada = 0;
    Tarefa tarefa;
    FILE *temp;

    printf("Digite o ID da tarefa que deseja atualizar: ");
    scanf("%d", &idAlvo);
    getchar(); // limpa o buffer

    // abre arquivos
    fp = fopen("tarefas.bin", "rb");
    if (fp == NULL) {
        printf("Erro: não foi possível abrir o arquivo de tarefas.\n");
        return;
    }

    temp = fopen("temp.bin", "wb");
    if (temp == NULL) {
        printf("Erro: não foi possível Atualizar tarefa\n");
        fclose(fp);
        return;
    }

    // percorre todas as tarefas
    while (fread(&tarefa, sizeof(Tarefa), 1, fp)) {
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
                scanf("%d", &opcao);
                getchar();

                switch (opcao) {
                    case 1:
                        printf("Nova descrição: ");
                        fgets(tarefa.description, sizeof(tarefa.description), stdin);
                        tarefa.description[strcspn(tarefa.description, "\n")] = 0;
                        break;

                    case 2:
                        do {
                            printf("Novo status ('F' - Feita, 'P' - Pendente): ");
                            scanf(" %c", &tarefa.status);
                            getchar();
                            if (tarefa.status != 'F' && tarefa.status != 'P')
                                printf("Status inválido! Use apenas 'F' ou 'P'.\n");
                        } while (tarefa.status != 'F' && tarefa.status != 'P');
                        break;

                    case 3:
                        printf("Nova data de vencimento (dd mm aaaa): ");
                        scanf("%d %d %d", &tarefa.expirationDay, &tarefa.expirationMonth, &tarefa.expirationYear);
                        getchar();
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

    fclose(fp);
    fclose(temp);

    // Proteção contra perda de dados

     // remove backup antigo (caso exista) e cria um novo
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
}


