/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 21:54:51 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/08 16:54:19 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**malloc_cmd(t_tokens *token)
{
	int			i;
	t_tokens	*tmp;
	char		**cmd;

	tmp = token;
	i = 0;
	while (token && token->token_type != 9)
	{
		if (token->token_type == 0 || token->token_type == 1)
			i++;
		token = token->next;
	}
	cmd = (char **)malloc(sizeof(char *) * (i + 1));	
	if (!cmd)
		return(perror("malloc"), NULL);
	cmd[0] = NULL;
	return (cmd);
}

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
	t_tree	*tmp;
	t_tree	*branch;
	t_tree	*tmp_branch;
	int		i;

	while (tree)
	{
		branch = tree;
		tmp = tree->right;
		while (branch)
		{
			i = -1;
			if (!branch->node_type)
				while (((char **)branch->content)[++i])
					free(((char **)branch->content)[i]);
			free(branch->content);
			tmp_branch = branch->left;
			free(branch);
			branch = tmp_branch;
		}
		tree = tmp;
	}
	tree = NULL;
}

void	error_hrdc(t_tokens *token, int pos)
{
	while (token && token->next && pos--)
	{
		if (!ft_strncmp(token->token, "<<", 2) && !token->next->token_type)
			free(here_doc_handler(token->next->token));
		token = token->next;
	}
}

t_tree	*parsing(void)
{
	int			error;
	char		*line;
	t_tokens	*token;
	t_tree		*tree;

	line = reading_line();
	if (!line)
		return (NULL);
	error = 0;
	token = tokenizer(line, &error);
	free(line);
	if (error)
		return (NULL);
	line = syntax(token, &error);
	if (error)
	{
		error_hrdc(token, error);
		ft_putstr_fd(line, STDERR_FILENO);
		return (free (line), free_token(token), NULL);
	}
	token_retyping(token);
	tree = tree_planting(token);
	return (free_token(token), tree);
}
