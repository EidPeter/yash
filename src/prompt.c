#include "prompt.h"

#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KCYN "\x1B[36m"
#define KGRN "\x1B[32m"
#define KWHT "\x1B[37m"
#define PATH_MAX 256

char *input;
char cwd[PATH_MAX];

char *prompt()
{
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%syash: ", KCYN);
        printf("%s%s", KGRN, cwd);
        printf("%s\n", KWHT);
        input = readline("$ ");
        return input;
    }
    else
    {
        printf("%syash: ", KCYN);
        printf("%s", KWHT);
        input = readline("$ ");
        return input;
    }
}