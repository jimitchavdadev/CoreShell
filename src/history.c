#include "shell.h"

#define HISTORY_SIZE 20
static char *history[HISTORY_SIZE];
static int history_count = 0;
static int history_start = 0;

void init_history()
{
    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        history[i] = NULL;
    }
}

void add_to_history(const char *cmd)
{
    if (history_count > 0 && strcmp(history[(history_start + history_count - 1) % HISTORY_SIZE], cmd) == 0)
    {
        return; // Don't add duplicate consecutive commands
    }

    if (history_count == HISTORY_SIZE)
    {
        free(history[history_start]);
        history_start = (history_start + 1) % HISTORY_SIZE;
        history_count--;
    }
    int next_pos = (history_start + history_count) % HISTORY_SIZE;
    history[next_pos] = strdup(cmd);
    history_count++;
}

void print_history()
{
    for (int i = 0; i < history_count; i++)
    {
        printf(" %d  %s\n", i + 1, history[(history_start + i) % HISTORY_SIZE]);
    }
}