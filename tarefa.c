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
            break;
        case '5': 
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
        printf("Erro: não foi possível criar arquivo temporário.\n");
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

    // substitui o arquivo original pelo novo
    remove("tarefas.bin");
    rename("temp.bin", "tarefas.bin");

    if (encontrada)
        printf("Tarefa %d removida com sucesso!\n", idRemover);
    else
        printf("Tarefa não encontrada.\n");
}


void modifyTask(FILE *fp) {

}

void listTasks(FILE *fp) {

}

void listWeeklyTasks(FILE *fp) {

}