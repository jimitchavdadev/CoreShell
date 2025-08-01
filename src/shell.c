#include "shell.h"

// Globals for job control
Job jobs[MAX_JOBS];
int job_count = 0;
struct termios shell_tmodes;

void print_prompt()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        // Handle root directory case where strrchr might return NULL
        char *last_slash = strrchr(cwd, '/');
        printf("\033[1;32mmy_shell\033[0m:\033[1;34m%s\033[0m$ ", last_slash ? last_slash + 1 : cwd);
    }
    else
    {
        perror("getcwd() error");
        printf("my_shell$ ");
    }
    fflush(stdout);
}

void sigchld_handler(int sig)
{
    (void)sig; // Unused parameter
    update_job_status();
}

int main()
{
    char cmdline[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    init_jobs();
    init_history();
    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    tcgetattr(STDIN_FILENO, &shell_tmodes);

    while (1)
    {
        print_prompt();

        if (fgets(cmdline, sizeof(cmdline), stdin) == NULL)
        {
            printf("\n");
            break; // EOF (Ctrl-D)
        }

        // Ignore empty lines and comments
        if (cmdline[0] == '\n' || cmdline[0] == '#')
        {
            continue;
        }

        cmdline[strcspn(cmdline, "\n")] = 0; // Remove newline
        add_to_history(cmdline);

        char *input_file = NULL;
        char *output_file = NULL;
        int is_bg = 0;
        int pipe_pos = 0;

        // Use a copy for parsing because strtok modifies the original string
        char cmdline_copy[MAX_CMD_LEN];
        strcpy(cmdline_copy, cmdline);

        parse_command(cmdline_copy, args, &input_file, &output_file, &pipe_pos, &is_bg);

        // If parsing results in no command, continue
        if (args[0] == NULL)
        {
            continue;
        }

        // Check for parent-managed built-ins first
        if (is_parent_builtin(args[0]))
        {
            handle_parent_builtin(args);
        }
        else
        {
            // Otherwise, pass to the executor. It handles forking for external
            // commands AND child-runnable built-ins.
            execute_command(args, input_file, output_file, is_bg);
        }
    }

    printf("Exiting shell.\n");
    return 0;
}