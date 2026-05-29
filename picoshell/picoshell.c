#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	count_cmds(char **cmds[])
{
	int	i;

	i = 0;
	while (cmds[i])
		i++;
	return (i);
}

int	ft_strlen(char *str)
{
	int	i = 0;
	while (str[i])
		i++;
	return i;
}
void	ft_putstr_fd(char *mes, int fd)
{
	write(fd, mes, ft_strlen(mes));
	write(fd, "\n", 1);
}

void	close_fd(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}
void	ft_free_pipes(int **pipes, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		close_fd(&pipes[j][0]);
		close_fd(&pipes[j][1]);
		free(pipes[j]);
		pipes[j] = NULL;
		j++;
	}
	if (i != 0)
	{
		free(pipes);
		pipes = NULL;
	}
		
	
}

int	**create_pipes(int n)
{
	int i = 0;
	int **pipes;

	pipes = malloc(sizeof(int *) * n);
	if (!pipes)
	{
		ft_putstr_fd("malloc pipes", 2);
		return (NULL);
	}
	while (i < n)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			ft_putstr_fd("malloc pipe", 2);
			ft_free_pipes(pipes, i);
			return (NULL);
		}
		if (pipe(pipes[i]) == -1)
		{
			ft_putstr_fd("create pipe", 2);
			ft_free_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);

}

int	dup_files(int **pipes, int i, int count)
{
	int	ret = 1;
	int	fd[2];
	
	if (i == 0)
		fd[0] = 0;
	else 
		fd[0] = pipes[i - 1][0];
	if (i == count - 1)
		fd[1] = 1;
	else
		fd[1] = pipes[i][1];
	
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		ft_putstr_fd("dup in", 2);
		ret = 0;
	}
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("dup out", 2);
		ret = 0;
	}
	return ret;
}

int	run_child_process(int **pipes, int count, int i, char **cmds)
{
	if (count == 1)
		execvp(cmds[0], cmds);
	else
	{
		if (!dup_files(pipes, i, count))
			return (0);
		ft_free_pipes(pipes, count - 1);
		execvp(cmds[0], cmds);
	}
	return (1);
}

int	create_fork(int **pipes, int count, char **cmds[])
{
	int	i = 0;
	int	pid;
	int	code;

	code = 0;
	while (i < count)
	{
		pid = fork();
		if (pid == -1)
		{
			ft_putstr_fd("create fork", 2);
			return (0);
		}
		else if (pid == 0)
		{
			code = run_child_process(pipes, count, i, cmds[i]);
			exit(code);
		}
		i++;
	}
	return (pid);
}

int	picoshell(char **cmds[])
{
	int	count;
	int	**pipes;
	int	status;
	
	count = count_cmds(cmds);
	pipes = NULL;
	status = 0;
	if (count - 1 > 0)
	{
		pipes = create_pipes(count - 1);
		if (!pipes)
			return (EXIT_FAILURE);
	}
	int ret = create_fork(pipes, count, cmds);
	ft_free_pipes(pipes, count - 1);
	if (!ret)
		return (EXIT_FAILURE);
	while (wait(&status) > 0)
		;
	return (ret);
}