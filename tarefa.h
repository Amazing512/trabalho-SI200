#ifndef TAREFAS_H
#define TAREFAS_H

void renderMenu(FILE *fp);

int processAction(char action, FILE *fp);

void registerTask(FILE *fp);

void removeTask(FILE *fp);

void modifyTask(FILE *fp);

void listTasks(FILE *fp);

void listWeeklyTasks(FILE *fp);

#endif