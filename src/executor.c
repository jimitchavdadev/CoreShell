#include "shell.h"

void execute_command(char **args, char *input_file, char *output_file, int is_bg)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    { // Child process
        // Restore default signal handlers for the child
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        // Handle I/O redirection
        if (input_file)
        {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (output_file)
        {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        // First, check if it's a child-runnable built-in
        if (handle_child_builtin(args))
        {
            exit(EXIT_SUCCESS); // Built-in handled, so exit successfully
        }

        // If not a child built-in, execute as an external command
        if (execvp(args[0], args) == -1)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {                      // Parent process
        setpgid(pid, pid); // Give the child its own process group for better job control
        if (is_bg)
        {
            add_job(pid, args[0], 1);
            printf("[%d] %d\n", job_count, pid);
        }
        else
        {
            int status;
            waitpid(pid, &status, WUNTRACED);
            if (WIFSTOPPED(status))
            {
                add_job(pid, args[0], 0);
                printf("\nStopped: %s\n", args[0]);
            }
        }
    }
}

// NOTE: The piping logic requires a more involved rewrite to properly integrate
// with the new built-in handling. The logic in `main` currently bypasses this.
// A full implementation would fork twice and call a modified version of execute_command
// for each child.
void execute_pipe(char **args1, char **args2)
{
    (void)args1; // Suppress unused parameter warning
    (void)args2; // Suppress unused parameter warning
    fprintf(stderr, "Piping feature needs refactoring to support the new built-in handling model.\n");
}