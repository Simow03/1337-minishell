/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:33:10 by ayyassif          #+#    #+#             */
/*   Updated: 2024/05/11 15:42:44 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell-pars.h"

t_tree	*get_next_redr(t_tokens *token, t_env *env)
{
	t_tree	*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (perror("malloc"), NULL);
	new->content = (void *)expanding(token->next->token, env);
	new->left = NULL;
	new->right = NULL;
	if (token->token_type == 2)
		new->node_type = 1;
	if (token->token_type == 4)
	{
		new->node_type = 2;
		new->content = (void *)here_doc_handler(token->next->token, token->next->is_quoted);
	}
	if (token->token_type == 6)
		new->node_type = 3;
	if (token->token_type == 7)
		new->node_type = 4;
	if (token->token_type == 9)
	{
		new->node_type = 5;
		new->content = NULL;
	}
	return (new);
}

	//0:	command
	//1:	<
	//2:	<<
	//3:	>
	//4:	>>
	//5:	pipe

int	tree_handler(t_tree	**tree, t_tree *new)
{
	t_tree	*tmp;

	tmp = *tree;
	if (!new)
		return (EXIT_FAILURE);
	if (!(*tree))
	{
		*tree = new;
		return (EXIT_SUCCESS);
	}
	while (tmp->left)
		tmp = tmp->left;
	tmp->left = new;
	return (EXIT_SUCCESS);
}

t_tree	*get_next_cmd(char **cmd)
{
	t_tree	*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (perror("malloc"), NULL);
	new->node_type = 0;
	new->left = NULL;
	new->right = NULL;
	new->content = (void *)cmd;
	return (new);
}

t_tree	*pipe_tree(t_tokens *token, t_tree *tree, t_env *env)
{
	t_tree	*new;

	if (token && token->token_type == 9)
	{
		new = (t_tree *)malloc(sizeof(t_tree));
		if (!new)
			return (perror("malloc"), NULL);
		new->node_type = 5;
		new->left = tree;
		new->right = tree_planting(token->next, env);
		new->content = NULL;
		return (new);
	}
	return (tree);
}

t_tree	*tree_planting(t_tokens *token, t_env *env)
{
	t_tree	*tree;
	char	**cmd;
	int		i;

	tree = NULL;
	cmd = malloc_cmd(token);
	if (!cmd)
		return (NULL);
	i = 0;
	while (token && token->token_type != 9)
	{
		if (token->token_type == 0 || token->token_type == 1)
			cmd[i++] = expanding(token->token, env);
		else
		{
			if (tree_handler(&tree, get_next_redr(token, env)))
				return (NULL);
			token = token->next;
		}
		token = token->next;
	}
	cmd[i] = NULL;
	if (i && tree_handler(&tree, get_next_cmd(cmd)))
		return (NULL);
	return (pipe_tree(token, tree, env));
}
