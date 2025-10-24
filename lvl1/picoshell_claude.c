#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	pid_t	pid;
	int		pipefd[2];
	int		prev_fd = -1;
	int		i = -1;

	while (cmds[++i])
	{
		if (cmds[i + 1] && pipe(pipefd))
			return (1);
		if ((pid = fork()) == -1)
			return (cmds[i + 1] && (close(pipefd[0]), close(pipefd[1])), 1);
		if (pid == 0)
		{
			if (prev_fd != -1 && (dup2(prev_fd, STDIN_FILENO) == -1 || !close(prev_fd)))
				exit(1);
			if (cmds[i + 1] && (close(pipefd[0]), dup2(pipefd[1], STDOUT_FILENO) == -1 || !close(pipefd[1])))
				exit(1);
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		prev_fd != -1 && close(prev_fd);
		cmds[i + 1] && (close(pipefd[1]), prev_fd = pipefd[0]);
	}
	while (wait(NULL) > 0);
	return (0);
}