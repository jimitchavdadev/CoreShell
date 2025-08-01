#include "shell.h"

// --- Lists of built-in commands categorized by execution context ---

// Category 1: Built-ins that MUST run in the parent process
const char *parent_builtins[] = {"my_cd", "my_exit", "jobs", "fg", "bg", "kill"};
void (*parent_funcs[])(char **) = {&my_cd, &my_exit, &list_jobs, &fg_command, &bg_command, &kill_command};

// Category 2: Built-ins that can run in a forked child process
const char *child_builtins[] = {"my_echo", "sysinfo", "countdown", "history"};
void (*child_funcs[])(char **) = {&my_echo, &sysinfo, &countdown, &print_history};

// --- Functions to check for and handle the built-ins ---

// Checks if a command is a parent-managed built-in
int is_parent_builtin(char *cmd)
{
    int n = sizeof(parent_builtins) / sizeof(char *);
    for (int i = 0; i < n; i++)
    {
        if (strcmp(cmd, parent_builtins[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Executes a parent-managed built-in
void handle_parent_builtin(char **args)
{
    int n = sizeof(parent_builtins) / sizeof(char *);
    for (int i = 0; i < n; i++)
    {
        if (strcmp(args[0], parent_builtins[i]) == 0)
        {
            (*parent_funcs[i])(args);
            return;
        }
    }
}

// Executes a child-managed built-in. Returns 1 if handled, 0 otherwise.
int handle_child_builtin(char **args)
{
    int n = sizeof(child_builtins) / sizeof(char *);
    for (int i = 0; i < n; i++)
    {
        if (strcmp(args[0], child_builtins[i]) == 0)
        {
            (*child_funcs[i])(args);
            return 1;
        }
    }
    return 0;
}

// --- Implementations of the built-in commands themselves ---

void my_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "my_cd: expected argument\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("my_cd");
        }
    }
}

void my_exit(char **args)
{
    (void)args; // Unused
    exit(0);
}

void sysinfo(char **args)
{
    (void)args; // Unused
    printf("---- System Information ----\n");
    printf("Shell Version: MyShell v1.1 (Fixed)\n");
    printf("---------------------------\n");
}

void countdown(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "countdown: needs a number argument\n");
        return;
    }
    int count = atoi(args[1]);
    if (count <= 0)
    {
        fprintf(stderr, "countdown: argument must be a positive number\n");
        return;
    }
    for (int i = count; i > 0; i--)
    {
        printf("%d...\n", i);
        fflush(stdout); // Ensure output is printed immediately
        sleep(1);
    }
    printf("Blast off! 🚀\n");
}

void my_echo(char **args)
{
    int i = 1;
    while (args[i] != NULL)
    {
        printf("%s", args[i]);
        if (args[i + 1] != NULL)
        {
            printf(" ");
        }
        i++;
    }
    printf("\n");
}