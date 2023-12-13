#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_SIZE 64
#define MAX_ARG 16


void execute_command(char *args[])
{
	pid_t pid;
	int state;

	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("shell");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid < 0)
	{
		perror("shell");
	}
	else
	{
		while (!WIFEXITED(state) && !WIFSIGNALED(state) && (waitpid(pid, &state, WUNTRACED)!= -1));
	}
}

void parse_input(char *input, char **args)
{
	int i = 0;
	char *token = strtok(input, " \t\n");

	while (token != NULL && i < MAX_ARG)
	{
		args[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
}

int main()
{
    char input[MAX_SIZE];
    char *args[MAX_ARG];
    int i;

    while (1) {
        printf("shell> ");
        fgets(input, sizeof(input), stdin);

        if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n')) {
            input[strlen(input) - 1] = '\0';
        }

        if (strcmp(input, "exit") == 0) {
            break;
        }
        parse_input(input, args);
        for (i = 0; args[i] != NULL; i++) {
            printf("args[%d]: %s\n", i, args[i]);
        }
        memset(args, 0, sizeof(args));
    }

    return 0;
}

