#ifndef SHELL_H
#define SHELL_H

// Add this line at the very top to enable POSIX extensions like strdup
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64
#define MAX_JOBS 64

// Job statuses
enum JobStatus
{
    JOB_RUNNING,
    JOB_STOPPED,
    JOB_DONE
};

// Job structure
typedef struct
{
    pid_t pid;
    int jid;
    char cmd[MAX_CMD_LEN];
    enum JobStatus status;
} Job;

// Global job list
extern Job jobs[MAX_JOBS];
extern int job_count;

// --- Parser ---
int parse_command(char *cmdline, char **args, char **input_file, char **output_file, int *pipe_pos, int *is_bg);

// --- Executor ---
void execute_command(char **args, char *input_file, char *output_file, int is_bg);
void execute_pipe(char **args1, char **args2);

// --- Built-ins ---
// New classification of built-ins
int is_parent_builtin(char *cmd);
void handle_parent_builtin(char **args);
int handle_child_builtin(char **args);

// The built-in functions themselves
void my_cd(char **args);
void my_exit(char **args);
void sysinfo(char **args);
void countdown(char **args);
void my_echo(char **args);
void list_jobs();
void fg_command(char **args);
void bg_command(char **args);
void kill_command(char **args);
void print_history();

// --- Job Control ---
void init_jobs();
int add_job(pid_t pid, const char *cmd, int is_bg);
void update_job_status();

// --- History ---
void init_history();
void add_to_history(const char *cmd);

#endif // SHELL_H