#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefa.h"

int main() {
    FILE *fp = fopen("tarefas.bin", "a+b");

    if (fp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    while (1) {
        char action = renderMenu(fp);

        if (processAction(action, &fp) == -1) {
            break;
        }
    }

    fclose(fp);
    return 0;
}
