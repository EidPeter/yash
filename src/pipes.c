#include "pipes.h"
#include "tokenizer.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int pipes(char *input)
{
    int i, commandc = 0, numpipes = 0, status;
    pid_t pid;
    char **args;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (i > 0)
        {
            if (input[i] == '|' && input[i + 1] != '|' && input[i - 1] != '|')
            {
                numpipes++;
            }
        }
    }
    int *pipefds = (int *)malloc((2 * numpipes) * sizeof(int));
    char *token = (char *)malloc((128) * sizeof(char));
    token = strtok_r(input, "|", &input);
    for (i = 0; i < numpipes; i++)
    {
        if (pipe(pipefds + i * 2) < 0)
        {
            perror("pipe creation failed");
            return 3;
        }
    }
    do
    {
        pid = fork();
        if (pid == 0)
        { // child process
            if (commandc != 0)
            {
                if (dup2(pipefds[(commandc - 1) * 2], 0) < 0)
                {
                    perror("child couldnt get input");
                    exit(1);
                }
            }
            if (commandc != numpipes)
            {
                if (dup2(pipefds[commandc * 2 + 1], 1) < 0)
                {
                    perror("child couldnt output");
                    exit(1);
                }
            }
            for (i = 0; i < 2 * numpipes; i++)
            {
                close(pipefds[i]);
            }
            args = tokenize(token);
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        }
        else if (pid < 0)
        {
            perror("fork() failed");
            return 3;
        }           // fork error
        commandc++; // parent process
    } while (commandc < numpipes + 1 && (token = strtok_r(NULL, "|", &input)));
    for (i = 0; i < 2 * numpipes; i++)
    {
        close(pipefds[i]);
    }
    free(pipefds);
    while (wait(NULL) > 0)
        ;
    return 1;
}