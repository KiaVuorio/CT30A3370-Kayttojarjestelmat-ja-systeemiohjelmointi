#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_TOKENS 100

// Different specific error messages
char *error_cmd_not_found = "Error: command not found\n";
char *error_invalid_exit = "Error: exit command takes no arguments\n";
char *error_cd_fail = "Error: cd failed\n";
char *error_cd_args = "Error: cd requires exactly one argument\n";
char *error_redirection = "Error: invalid redirection\n";
char *error_fork_fail = "Error: fork failed\n";
char *error_exec_fail = "Error: exec failed\n";
char *error_file_open = "Error: cannot open file for redirection\n";
char *error_generic = "An error has occurred\n";

char **path_list = NULL;
int path_count = 0;

// Function to print any error message to stderr
void print_custom_error(const char *msg)
{
    write(STDERR_FILENO, msg, strlen(msg));
}

// Sets (or resets) the shell path variable with new directories
void set_path(char **args, int count)
{
    if (path_list != NULL)
    {
        for (int i = 0; i < path_count; i++)
            free(path_list[i]);
        free(path_list);
    }

    if (count == 0)
    {
        path_list = NULL;
        path_count = 0;
        return;
    }

    path_list = malloc(count * sizeof(char *));
    for (int i = 0; i < count; i++)
    {
        path_list[i] = strdup(args[i]);
    }
    path_count = count;
}

// Executes a command, handling redirection if needed
// Returns 0 on success, 1 on error (but doesn't exit shell except for valid exit command)
int exec_command(char **args, int arg_count, char *redirect_file)
{
    if (args[0] == NULL)
        return 0;

    // Built-in commands
    if (strcmp(args[0], "exit") == 0)
    {
        if (arg_count != 1)
        {
            print_custom_error(error_invalid_exit);
            return 1; // error, don't exit shell
        }
        else
        {
            exit(0); // valid exit: terminate shell immediately
        }
    }

    if (strcmp(args[0], "cd") == 0)
    {
        if (arg_count != 2)
        {
            print_custom_error(error_cd_args);
            return 1;
        }

        if (chdir(args[1]) != 0)
        {
            print_custom_error(error_cd_fail);
            return 1;
        }
        return 0;
    }

    if (strcmp(args[0], "path") == 0)
    {
        set_path(args + 1, arg_count - 1);
        return 0;
    }

    // Search path for executable
    for (int i = 0; i < path_count; i++)
    {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path_list[i], args[0]);
        if (access(fullpath, X_OK) == 0)
        {
            const pid_t pid = fork();
            if (pid == 0)
            {
                if (redirect_file != NULL)
                {
                    int fd = open(redirect_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                    if (fd < 0)
                    {
                        print_custom_error(error_file_open);
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    dup2(fd, STDERR_FILENO);
                    close(fd);
                }
                execv(fullpath, args);
                print_custom_error(error_exec_fail);
                exit(1);
            }

            if (pid > 0)
            {
                waitpid(pid, NULL, 0);
                return 0;
            }

            print_custom_error(error_fork_fail);
            return 1;
        }
    }

    // Command not found on any path
    print_custom_error(error_cmd_not_found);
    return 1;
}

// Parses and executes a full line of input (can contain multiple parallel commands)
void parse_line(char *line)
{
    char *parallel_cmds[MAX_TOKENS];
    int cmd_count = 0;

    // Split parallel commands on '&'
    const char *cmd = strtok(line, "&");
    while (cmd != NULL)
    {
        parallel_cmds[cmd_count++] = strdup(cmd);
        cmd = strtok(NULL, "&");
    }

    pid_t children[MAX_TOKENS];
    int child_count = 0;

    for (int i = 0; i < cmd_count; i++)
    {
        char *args[MAX_TOKENS];
        int arg_count = 0;
        char *redirect_file = NULL;

        // Handle redirection (>)
        char *redirection = strchr(parallel_cmds[i], '>');
        if (redirection != NULL)
        {
            *redirection = '\0';
            redirection++;

            // Trim leading whitespace in redirect filename
            while (*redirection == ' ' || *redirection == '\t')
                redirection++;

            char *filename = strtok(redirection, " \t\n");
            char *extra = strtok(NULL, " \t\n");
            if (filename == NULL || extra != NULL)
            {
                print_custom_error(error_redirection);
                free(parallel_cmds[i]);
                continue;
            }
            redirect_file = filename;
        }

        // Parse command arguments
        char *token = strtok(parallel_cmds[i], " \t\n");
        while (token != NULL)
        {
            args[arg_count++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[arg_count] = NULL;

        if (args[0] == NULL)
        {
            free(parallel_cmds[i]);
            continue;
        }

        // Built-in commands executed in parent
        if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "cd") == 0 || strcmp(args[0], "path") == 0)
        {
            exec_command(args, arg_count, redirect_file);
            free(parallel_cmds[i]);
            // exit command handled inside exec_command via exit(0)
        }
        else
        {
            // Fork for other commands
            pid_t pid = fork();
            if (pid == 0)
            {
                exec_command(args, arg_count, redirect_file);
                exit(0);
            }
            else if (pid > 0)
            {
                children[child_count++] = pid;
                free(parallel_cmds[i]);
            }
            else
            {
                print_custom_error(error_fork_fail);
                free(parallel_cmds[i]);
            }
        }
    }

    // Wait for all forked children to finish
    for (int i = 0; i < child_count; i++)
    {
        waitpid(children[i], NULL, 0);
    }
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    int batch_mode = 0;

    if (argc > 2)
    {
        print_custom_error(error_generic);
        exit(1);
    }

    // Batch mode: read from file instead of stdin
    if (argc == 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            print_custom_error(error_generic);
            exit(1);
        }
        batch_mode = 1;
    }

    // Initialize path with /bin by default
    path_list = malloc(sizeof(char *));
    path_list[0] = strdup("/bin");
    path_count = 1;

    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        if (!batch_mode)
        {
            printf("wish> ");
            fflush(stdout);
        }

        if (getline(&line, &len, input) == -1)
        {
            break;
        }

        parse_line(line);
    }

    // Cleanup
    free(line);
    for (int i = 0; i < path_count; i++)
        free(path_list[i]);
    free(path_list);

    return 0;
}

//
// ei toimi:
//- redirection test
//- parallel test