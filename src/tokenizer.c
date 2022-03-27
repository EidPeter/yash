#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **tokenize(char *input)
{
    char **command = malloc(16 * sizeof(char *));
    if (command == NULL)
    {
        perror("malloc failed");
        exit(1);
    }

    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL)
    {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}