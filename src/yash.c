#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <setjmp.h>

#include "history.h"
#include "pipes.h"
#include "tokenizer.h"
#include "prompt.h"
#include "builtins.h"
#include "separator.h"

#define DELIMITER ";"

void sigint_setup();
void sigint_handler(int);
static sigjmp_buf env;
static volatile sig_atomic_t jump_active = 0;

int main(int argc, char *argv[])
{
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;

    sigint_setup();

    if (argc > 2)
    {
        printf("0 or 1 arguments expected.\n");
        return 1;
    }
    else if (argv[1] && access(argv[1], F_OK))
    {
        printf("Batch file does not exist\n");
        return 1;
    }
    else if (argv[1] && !access(argv[1], F_OK))
    {
        do
        {
            FILE *fp;
            long lSize;
            char *buffer;

            fp = fopen(argv[1], "rb");
            if (!fp)
                perror(argv[1]), exit(1);

            fseek(fp, 0L, SEEK_END);
            lSize = ftell(fp);
            rewind(fp);

            /* allocate memory for entire content */
            buffer = calloc(1, lSize + 1);
            if (!buffer)
                fclose(fp), fputs("memory alloc fails", stderr), exit(1);

            /* copy the file into the buffer */
            if (1 != fread(buffer, lSize, 1, fp))
                fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

            execute_sequence(buffer, "\n");

            fclose(fp);
            free(buffer);
            break;
        } while (1);
        return 0;
    }

    do
    {
        if (sigsetjmp(env, 1) == 42)
        {
            printf("\n");
            continue;
        }

        jump_active = 1;

        input = prompt();

        // exit on ctrl-D
        if (input == NULL)
        {
            printf("\n");
            exit(0);
        }

        // add to history only if not empty and not whitespaces
        if (strlen(input) != 0)
        {
            for (int i = 0; i < strlen(input); i++)
            {
                if (!isspace(input[i]))
                {
                    add_history(input);
                    break;
                }
            }
        }

        if (strstr(input, DELIMITER) != NULL)
        {
            execute_sequence(input, DELIMITER);
            continue;
        }

        if (strchr(input, '|') != NULL)
        {
            pipes(input);
            continue;
        }

        command = tokenize(input);

        /* Handle empty commands */
        if (!command[0])
        {
            free(input);
            free(command);
            continue;
        }

        if (!strcmp(command[0], "exit") || !strcmp(command[0], "quit"))
        {
            exit(0);
        }

        if (check_if_builtin(command[0]))
        {
            run_builtin(command);
            continue;
        }

        child_pid = fork();

        if (child_pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }
        else if (child_pid == 0)
        {
            sigint_setup();
            if (execvp(command[0], command) < 0)
            {
                perror(command[0]);
                exit(1);
            }
        }
        else
        {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        free_history();

        if (!input)
            free(input);
        if (!command)
            free(command);

    } while (1);

    return 0;
}

void sigint_handler(int signo)
{
    if (!jump_active)
    {
        return;
    }
    siglongjmp(env, 42);
}

// SIGINT setup
void sigint_setup()
{
    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, NULL);
}