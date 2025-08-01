#include "shell.h"

int parse_command(char *cmdline, char **args, char **input_file, char **output_file, int *pipe_pos, int *is_bg)
{
    int i = 0;
    char *token = strtok(cmdline, " \t\n");
    *pipe_pos = 0;

    while (token != NULL && i < MAX_ARGS - 1)
    {
        if (strcmp(token, "<") == 0)
        {
            token = strtok(NULL, " \t\n");
            *input_file = token;
        }
        else if (strcmp(token, ">") == 0)
        {
            token = strtok(NULL, " \t\n");
            *output_file = token;
        }
        else if (strcmp(token, "|") == 0)
        {
            *pipe_pos = i;
        }
        else
        {
            args[i++] = token;
        }
        token = strtok(NULL, " \t\n");
    }

    if (i > 0 && strcmp(args[i - 1], "&") == 0)
    {
        *is_bg = 1;
        args[i - 1] = NULL;
        i--;
    }
    else
    {
        *is_bg = 0;
    }

    args[i] = NULL;
    return i;
}