/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstaali <mstaali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:54:51 by ayyassif          #+#    #+#             */
/*   Updated: 2024/07/14 18:29:31 by mstaali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_prompt(void)
{
	char	*prompt;
	char	*tmp;
	int		status;

	tmp = ft_strdup(BOLD BLUE "[" RESET "minishell" BOLD BLUE "]");
	status = global_return_int(0, 0);
	if (status == 0)
		prompt = ft_strjoin(tmp, BLUE "$ " RESET);
	else
		prompt = ft_strjoin(tmp, RED "$ " RESET);
	free(tmp);
	return (prompt);
}

static char	*reading_line(void)
{
	char	*line;
	char	*prompt;

	line = NULL;
	prompt = get_prompt();
	line = readline(prompt);
	free(prompt);
	if (line == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (line && *line)
	{
		add_history(line);
		return (line);
	}
	return (free(line), NULL);
}

t_tree	*parsing(void)
{
	int			error;
	char		*line;
	char		*err_msg;
	t_token		*token;
	t_tree		*tree;

	line = reading_line();
	if (!line)
		return (NULL);
	error = 0;
	token = tokenizer(line, &error);
	free(line);
	if (error)
		return (NULL);
	err_msg = syntax(token, &error);
	if (err_msg)
	{
		global_return_int(1, 258);
		error_hrdc(token, error);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		free_token(token);
		return (free(err_msg), NULL);
	}
	tree = tree_planting(token);
	return (tree);
}
