#include "shell.h"

void init_jobs()
{
    for (int i = 0; i < MAX_JOBS; i++)
    {
        jobs[i].pid = 0;
    }
    job_count = 0;
}

int add_job(pid_t pid, const char *cmd, int is_bg)
{
    if (job_count >= MAX_JOBS)
    {
        fprintf(stderr, "my_shell: too many jobs\n");
        return 0;
    }
    job_count++;
    jobs[job_count - 1].pid = pid;
    jobs[job_count - 1].jid = job_count;
    strncpy(jobs[job_count - 1].cmd, cmd, MAX_CMD_LEN - 1);
    jobs[job_count - 1].status = is_bg ? JOB_RUNNING : JOB_STOPPED;
    return job_count;
}

void list_jobs()
{
    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].pid != 0)
        {
            printf("[%d] %s\t%s\n", jobs[i].jid,
                   jobs[i].status == JOB_RUNNING ? "Running" : "Stopped",
                   jobs[i].cmd);
        }
    }
}

void update_job_status()
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
    {
        for (int i = 0; i < job_count; i++)
        {
            if (jobs[i].pid == pid)
            {
                if (WIFEXITED(status) || WIFSIGNALED(status))
                {
                    printf("\n[%d] Done\t%s\n", jobs[i].jid, jobs[i].cmd);
                    // Remove job by shifting
                    for (int j = i; j < job_count - 1; j++)
                    {
                        jobs[j] = jobs[j + 1];
                    }
                    job_count--;
                }
                else if (WIFSTOPPED(status))
                {
                    jobs[i].status = JOB_STOPPED;
                    printf("\n[%d] Stopped\t%s\n", jobs[i].jid, jobs[i].cmd);
                }
                break;
            }
        }
    }
}

void fg_command(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "fg: job ID required\n");
        return;
    }
    int jid = atoi(args[1] + (args[1][0] == '%' ? 1 : 0));
    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].jid == jid)
        {
            kill(-jobs[i].pid, SIGCONT); // Send signal to process group
            waitpid(jobs[i].pid, NULL, WUNTRACED);
            return;
        }
    }
    fprintf(stderr, "fg: job not found: %d\n", jid);
}

void bg_command(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "bg: job ID required\n");
        return;
    }
    int jid = atoi(args[1] + (args[1][0] == '%' ? 1 : 0));
    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].jid == jid)
        {
            kill(-jobs[i].pid, SIGCONT);
            jobs[i].status = JOB_RUNNING;
            return;
        }
    }
    fprintf(stderr, "bg: job not found: %d\n", jid);
}

void kill_command(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "kill: job ID required\n");
        return;
    }
    int jid = atoi(args[1] + (args[1][0] == '%' ? 1 : 0));
    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].jid == jid)
        {
            kill(-jobs[i].pid, SIGTERM);
            return;
        }
    }
    fprintf(stderr, "kill: job not found: %d\n", jid);
}