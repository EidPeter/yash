#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "cd.h"
#include "history.h"

char *builtins[] = {"cd", "history"};

int builtins_count = sizeof builtins / sizeof builtins[0];

bool check_if_builtin(char *input)
{
    for (int i = 0; i < builtins_count; ++i)
    {
        if (!strcmp(builtins[i], input))
        {
            return true;
        }
    }
    return false;
}

void run_builtin(char **command)
{
    if (!strcmp(command[0], "cd"))
    {
        if (cd(command) < 0)
        {
            perror(command[1]);
        }
    }

    if (!strcmp(command[0], "history"))
    {
        display_history();
    }
}