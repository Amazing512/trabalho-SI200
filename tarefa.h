#ifndef TAREFAS_H
#define TAREFAS_H
#include <stdio.h>

typedef struct tarefa {
  int id;
  char status; // 'F' para feito, 'P' para pendente
  char description[100];
  int expirationDay;
  int expirationMonth;
  int expirationYear;
} Tarefa;

void renderMenu(FILE *fp);

int processAction(char action, FILE *fp);

void listAllActions(FILE *fp);

void listNotDoneActions(FILE *fp);

void registerNewTask(FILE *fp);

void removeTask(FILE *fp);

void modifyTaskState(FILE *fp);

void updateTask(FILE *fp);

#endif