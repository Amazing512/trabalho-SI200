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

    menuRecursivo(fp);

    fclose(fp);
    return 0;
}
