#include <stdio.h>
#include "tarefa.h"

void renderMenu(FILE *fp) {
    char taskCountText[30];
    sprintf(taskCountText, "Existem %d tarefas.", 4);

    printf("----------------------------------\n");
    printf("- %-30s -\n", taskCountText);
    printf("- %-30s -\n", "Escolha uma ação:");
    printf("- %-30s -\n", "");
    printf("- %-30s -\n", "1) Listar tarefas da semana");
    printf("- %-30s -\n", "2) Listar todas as tarefas");
    printf("- %-30s -\n", "3) Cadastrar tarefa");
    printf("- %-30s -\n", "4) Alterar status da tarefa");
    printf("- %-30s -\n", "5) Modificar tarefa");
    printf("- %-30s -\n", "6) Excluir tarefa");
    printf("- %-30s -\n", "0) Sair");
    printf("----------------------------------\n");
}

int processAction(char action, FILE *fp) {
    switch(action) {
        case '1': 
            break;
        case '2': 
            break;
        case '3': 
            break;
        case '4': 
            break;
        case '5': 
            break;
        case '6': 
            break;        
        case '0': 
            return -1;
        default: 
            printf("Ação selecionada inválida.\n");
            break;
    }
}

void registerTask(FILE *fp) {

}

void removeTask(FILE *fp) {

}

void modifyTask(FILE *fp) {

}

void listTasks(FILE *fp) {

}

void listWeeklyTasks(FILE *fp) {

}