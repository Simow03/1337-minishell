/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyassif <ayyassif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:33:10 by ayyassif          #+#    #+#             */
/*   Updated: 2024/06/01 16:10:06 by ayyassif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tree	*get_next_redr(t_token *token)
{
	t_tree	*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	if (!new)
		return (perror("malloc"), NULL);
	new->content = (void *)expanding(token->next->token, token->next->token_type);
	new->left = NULL;
	new->right = NULL;
	if (token->token_type == 2)
		new->node_type = 1;
	if (token->token_type == 4)
		hrdc_tree(new, token);
	if (!new->content)
		return (NULL);
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

int	tree_handler(t_tree	**tree, t_tree *new)
{
	t_tree	*tmp;

	tmp = *tree;
	if (!new || !new->content)
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

t_tree	*pipe_tree(t_token *token, t_tree *tree)
{
	t_tree	*new;

	if (token && token->token_type == 9)
	{
		new = (t_tree *)malloc(sizeof(t_tree));
		if (!new)
			return (perror("malloc"), NULL);
		new->node_type = 5;
		new->left = tree;
		new->right = tree_planting(token->next);
		new->content = NULL;
		return (new);
	}
	return (tree);
}

t_tree	*tree_planting(t_token *token)
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
			append_cmd(cmd, token);
			cmd[i++] = expanding(token->token, token->token_type);
		else
		{
			if (tree_handler(&tree, get_next_redr(token)))
				return (free_tree(tree), NULL);
			token = token->next;
		}
		token = token->next;
	}
	cmd[i] = NULL;
	if (i && tree_handler(&tree, get_next_cmd(cmd)))
		return (free_tree(tree), NULL);
	return (pipe_tree(token, tree));
}
