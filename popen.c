#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

void	close_pipe(int fd0, int fd1)
{
	close(fd0);
	close(fd1);
}

int	ft_popen(const char *file, char *const argv[], char type)
{
	int	fds[2];
	pid_t pid;

	if (!file || !argv || (type != 'r' && type != 'w') || (pipe(fds) == -1))
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close_pipe(fds[0], fds[1]), -1);
	if (pid == 0)
	{
		if (type == 'r' && (dup2(fds[1], STDOUT_FILENO) == -1))
			exit(1);
		else if (type == 'w' && (dup2(fds[0], STDIN_FILENO) == -1))
			exit(1);
		close_pipe(fds[0], fds[1]);
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
		return (close(fds[1]), fds[0]);
	else
		return (close(fds[0]), fds[1]);
}