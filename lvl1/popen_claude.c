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
	int		fds[2];
	pid_t	pid;
	int		std_fd = (type == 'r') ? STDOUT_FILENO : STDIN_FILENO;
	int		ret_fd = (type == 'r') ? 0 : 1;

	if (!file || !argv || (type != 'r' && type != 'w') || pipe(fds) == -1)
		return (-1);
	if ((pid = fork()) == -1)
		return (close_pipe(fds[0], fds[1]), -1);
	if (pid == 0)
	{
		if (dup2(fds[!ret_fd], std_fd) == -1)
			exit(1);
		close_pipe(fds[0], fds[1]);
		execvp(file, argv);
		exit(1);
	}
	return (close(fds[!ret_fd]), fds[ret_fd]);
}