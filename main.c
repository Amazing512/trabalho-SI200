#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// #include <locale.h>

#include "tarefa.h"

int main() {
    // setlocale(LC_ALL, "pt_BR.UTF-8");
    // setlocale(LC_COLLATE, "pt_BR.UTF-8");
    // setlocale(LC_CTYPE, "pt_BR.UTF-8");

    // printf("Açãm é pra lar ->");

    FILE *fp = fopen("tarefas.txt", "a+");

    if(fp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    while(1) {
        renderMenu();

        char action = getchar();

        if(processAction(action, fp) == -1) {
            break;
        }
    }


    system("pause");
    fclose(fp);
    return 0;
}