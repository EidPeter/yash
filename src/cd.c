#include "cd.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int cd(char **args)
{
    if (args[1] == NULL)
    {
        if (chdir(getenv("HOME")) != 0)
        {
            perror("cd failed");
            return 3;
        }
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("cd failed");
            return 3;
        }
    }
    return 1;
}