#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitorAqv.h"

#define AUTHORS ("Eduardo Bregalda, Joao Vittor Caumo")

int linha(char **restrict line, size_t *restrict len, FILE *restrict fp)
{
    char chunk[128];
    (*line)[0] = '\0';
    while (fgets(chunk, sizeof(chunk), fp) != NULL)
    {
        if (*len - strlen(*line) < sizeof(chunk)){
            *len *= 2;
        }
        strcat(*line, chunk);
        if ((*line)[strlen(*line) - 1] == '\n')
        {
            return strlen(*line);
        }
    }
    return -1;
}

char **leitor(FILE *fp, size_t *tLinhas)
{
    size_t n = 0;
    size_t tamLinha = 0;
    size_t i = 0;

    char *bufLinha = NULL;
    size_t sizeAdd = 10;
    char **linhas = malloc(sizeAdd * sizeof(char **));

    while ((tamLinha = linha(&bufLinha, &n, fp)) != -1)
    {
        if (i >= sizeAdd)
        {
            sizeAdd += sizeAdd;
            char **tmp = realloc(linhas, sizeAdd * sizeof(char **));
            linhas = tmp;
        }
        bufLinha[strcspn(bufLinha, "\n")] = 0;
        *(linhas + i) = malloc((tamLinha + 1) * sizeof(char));
        *(linhas + i) = bufLinha;
        bufLinha = *(linhas + i);
        i++;
    }
    free(bufLinha);
    *tLinhas = i;
    return linhas;
}
