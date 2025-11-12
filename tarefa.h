#ifndef TAREFA_H
#define TAREFA_H

#include <stdio.h>

/* Estrutura da tarefa */
typedef struct {
    int id;
    char description[100];
    char status; // 'P' = pendente, 'F' = feito
    int expirationDay;
    int expirationMonth;
    int expirationYear;
} Tarefa;

/* Funções utilitárias */
void limparBuffer();


/* Funções principais */
int getTaskCount(FILE *fp);
void registerNewTask(FILE *fp);
void listAllActions(FILE *fp);
void listNotDoneActions(FILE *fp);

/* Funções que modificam o arquivo (usam FILE **) */
void modifyTaskState(FILE **fp);
void updateTask(FILE **fp);
void removeTask(FILE **fp);

/* Funções de menu */
char renderMenu(FILE *fp);
int processAction(char action, FILE **fp);
void menuRecursivo(FILE **fp);

#endif
