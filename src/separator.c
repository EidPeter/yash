#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "tokenizer.h"
#include "pipes.h"
#include "separator.h"

void execute_sequence(char *input, char* delimiter)
{
    pid_t pid;

    char *token = (char *)malloc((128) * sizeof(char));
    char **command;

    token = strtok_r(input, delimiter, &input);
    do
    {
        if (strchr(token, '|') != NULL)
        {
            pipes(token);
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            command = tokenize(token);
            if (execvp(command[0], command) < 0)
            {
                perror("execution failed");
                exit(1);
            }
        }
        else if (pid < 0)
        {
            perror("fork failed");
        }
        else
        {
            wait(NULL);
        }

    } while (token = strtok_r(NULL, delimiter, &input));

    while (wait(NULL) > 0)
        ;
}