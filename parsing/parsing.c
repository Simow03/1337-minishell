/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:54:51 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/28 10:52:05 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*reading_line(void)
{
	char	*line;

	line = NULL;
	line = readline("my bash$ ");
	rl_on_new_line();
	if (line && *line)
	{
		add_history(line);
		return (line);
	}
	return (free(line), NULL);
}

void	free_tree(t_tree *tree)
{
	int	i;

	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	i = -1;
	if (tree->node_type == TR_COMMAND)
		while (((char **)tree->content)[++i])
			free(((char **)tree->content)[i]);
	free(tree->content);
	free(tree);
}

t_tree	*parsing()
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
		error_hrdc(token, error);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		free_token(token);
		return (free(err_msg), NULL);
	}
	tree = tree_planting(token);
	return (tree);
}
