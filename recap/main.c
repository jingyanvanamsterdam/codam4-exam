#include <stdlib.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_command
{
	int	words_count;
	char **cmd;
}		t_command;

typedef struct	s_commands
{
	int	cmd_amount;
	char	*input;
	t_command* cmds;
}		t_commnds;


int	count_cmd(char *input)
{
	int	n = 1;

	for (int i = 0; input[i]; i++)
	{
		if (input[i] == '|')
			n++;
	}
	return n;
}

int	find_index(char *input)
{
	int	 i = 0;

	while (input[i] && input[i] != ' ' && input[i] != '|')
		i++;
	return (i);
}

int	process_cmd(char *input, t_commnds *cmnds)
{

	int i = 0;
	int cmd_i = 0;
	int	word_i = 0;
	int	j = 0;
	int	count = 0;
	while (cmd_i < cmnds->cmd_amount)
	{
		count = find_index(input + i);
		cmnds->cmds[cmd_i].cmd[word_i] = calloc(count + 1, sizeof(char));
		if (!cmnds->cmds[cmd_i].cmd[word_i])
			return 0;
		while (input[i] && input[i] != ' ' && input[i] != '|')
		{
			cmnds->cmds[cmd_i].cmd[word_i][j] = input[i];
			i++;
			j++;
		}
		word_i++;
		i++;
		if (input[i] == '|')
		{
			cmd_i++;
			word_i = 0;
			j = 0;
		}
	}
	return (1);
}

void	free_2d_arr(char **arr)
{
	int	i = 0;

	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

int	ft_failexit(t_commnds *cmnds, int cmd_index)
{

	size_t	i = 0;
	if (cmnds->input)
		free(cmnds->input);
	while (i < cmd_index)
	{
		free_2d_arr(cmnds->cmds[i].cmd);
		i++;
	}
	free(cmnds->cmds);
	free(cmnds);
	exit(1);
}

int	count_words(char *input, int cmd_amount)
{
	int	i = 0;
	int	j = 0;

	while (input[i] && input[i] != ' ' && input[i] != '|')
		i++;
	if (input[i] = ' ')
	{
		j++;
		while (input[i] = ' ')
			i++;
	}
	if (input[i] = '|')
		return j;

	
}

int main(void)
{
	char *input;

	input = NULL;

	t_commnds *cmnds;
	cmnds = calloc(1, sizeof(t_commnds));
	if (!cmnds)
		exit(1);
	cmnds->cmd_amount = 0;
	cmnds->cmds = NULL;

	while (1)
	{
		input = readline("practice: ");
		if (!input)
			ft_failexit(cmnds, 0);
		
		cmnds->input = input;
		cmnds->cmd_amount = count_cmd(input);
		cmnds->cmds = calloc(cmnds->cmd_amount, sizeof(t_command));
		if (!cmnds->cmds)
			ft_failexit(cmnds, 0);

		int	i = 0;
		while (i < cmnds->cmd_amount)
		{
			cmnds->cmds[i].words_count = count_words(input, cmnds->cmd_amount);
			cmnds->cmds[i].cmd = calloc(cmnds->cmds[i].words_count, sizeof(char *));
			if (!cmnds->cmds[i].cmd)
				ft_failexit(cmnds, i);
			i++;
		}
		
		if (!process_cmd(input, cmnds))
			ft_failexit(input, cmnds->cmd_amount);

		// reset input;
		free(input);
		input = NULL;
	}
	return 0;
}