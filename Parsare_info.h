// Copyright Aldica Maria Miruna
#ifndef PARSARE_INFO_H_
#define PARSARE_INFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 50

// identif argumentele din input & nr lor
int aux_read0(char *line)
{
    char *p;
    int nr;
    p = strtok(line, " ");
    p = strtok(NULL, " ");
    nr = atoi(p);
    return nr;
}

int aux_read1(char *line)
{
    char *p;
    int nr;
    p = strtok(line, " ");
    while (p != NULL)
    {
        nr = atoi(p);
        p = strtok(NULL, " ");
    }
    return nr;
}

int aux_read2(char *line)
{
    int nr;
    nr = atoi(strtok(line, " "));
    return nr;
}

char *aux_read3(char *line)
{
    char *p;
    char *name = malloc(MAX_LINE);
    p = strtok(line, " ");
    while (p != NULL)
    {
        strcpy(name, p);
        p = strtok(NULL, " ");
    }
    return name;
}

int verify_arg(char *line)
{
    char *p;
    int nr = 0;
    p = strtok(line, " ");
    while (p != NULL)
    {
        nr++;
        p = strtok(NULL, " ");
    }
    return nr;
}
#endif  // PARSARE_INFO_H_
